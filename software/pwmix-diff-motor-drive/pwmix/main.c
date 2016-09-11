/*
 * pwmix2.c
 *
 * Created: 9/10/2016 7:29:07 AM
 * Author : User
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"

volatile uint16_t lastPulseValUI16A[2];
volatile uint16_t mixPulseValUI16A[2];
volatile uint8_t inputStateUI8A[2];
volatile uint8_t flagsUI8;

ISR(TIM0_COMPA_vect){
	PORTB &= ~_BV(OUT_CHANNEL_ONE);
}

ISR(TIM1_COMPA_vect){
	PORTB &= ~_BV(OUT_CHANNEL_TWO);
}

ISR(TIM0_OVF_vect){
	flagsUI8 |= FLAGS_CH_ONE_RECV;
	if(inputStateUI8A[CHANNEL_ONE] == STATE_ACTIVE) //If we had an overflow before the pulse ended
		lastPulseValUI16A[CHANNEL_ONE] = PWM_MAX; //Just save the value as longest valid pulse length
	inputStateUI8A[CHANNEL_ONE] = STATE_IDLE;
	TCCR0B = 0; //Disable TIMR0
}

ISR(TIM1_OVF_vect){
	flagsUI8 |= FLAGS_CH_TWO_RECV;
	if(inputStateUI8A[CHANNEL_TWO] == STATE_ACTIVE) //If we had an overflow before the pulse ended
		lastPulseValUI16A[CHANNEL_TWO] = PWM_MAX; //Just save the value as longest valid pulse length
	inputStateUI8A[CHANNEL_TWO] = STATE_IDLE;
	TCCR1 = 0; //Disable TIMR1
}

ISR(PCINT0_vect){ //Executed in input pin change
	uint8_t channelVarUI8, channelTmpUI8;
	
	for(channelVarUI8 = CHANNEL_ONE; channelVarUI8 <= CHANNEL_TWO; channelVarUI8++){ //Check both input channels
		switch(inputStateUI8A[channelVarUI8]){
			case STATE_IDLE:
				if(PINB & _BV((channelVarUI8 == CHANNEL_ONE)?IN_CHANNEL_ONE:IN_CHANNEL_TWO)){ //If state idle and input signal high
					inputStateUI8A[channelVarUI8] = STATE_ACTIVE;
					mixPulseValUI16A[CHANNEL_ONE] = lastPulseValUI16A[CHANNEL_ONE] + (lastPulseValUI16A[CHANNEL_TWO] - PWM_MIDDLE); //Calculate mixint outputs from previous pulse lengths
					mixPulseValUI16A[CHANNEL_TWO] = lastPulseValUI16A[CHANNEL_ONE] - (lastPulseValUI16A[CHANNEL_TWO] - PWM_MIDDLE); //Calculate mixint outputs from previous pulse lengths
					for(channelTmpUI8 = CHANNEL_ONE; channelTmpUI8 <= CHANNEL_TWO; channelTmpUI8++){ //Make sure the mixed values are within reasonable limits
						if(mixPulseValUI16A[channelTmpUI8] > PWM_MAX)
							mixPulseValUI16A[channelTmpUI8] = PWM_MAX;
						if(mixPulseValUI16A[channelTmpUI8] < PWM_MIN)
							mixPulseValUI16A[channelTmpUI8] = PWM_MIN;
					}
					if(channelVarUI8 == CHANNEL_ONE){
						OCR0A = mixPulseValUI16A[CHANNEL_ONE]; //Set compare match to stop output pulse
						TCNT0 = 0; //Reset TMR0 				
						GTCCR |= _BV(PSR0); //Reset TMR0 prescaler
						TCCR0B |= _BV(CS01) | _BV(CS00); //Start TMR0 with 64 prescaler
						if(flagsUI8 & FLAGS_SYNC_DONE)
							PORTB |= _BV(OUT_CHANNEL_ONE); //Set channel output high
					}else{
						OCR1A = mixPulseValUI16A[CHANNEL_TWO]; //Set compare match to stop output pulse
						TCNT1 = 0; //Reset TMR1 
						GTCCR |= _BV(PSR1); //Reset TMR1 prescaler
						TCCR1 |= _BV(CS12) | _BV(CS11) | _BV(CS10); //Start TMR1 with 64 prescaler
						if(flagsUI8 & FLAGS_SYNC_DONE)
							PORTB |= _BV(OUT_CHANNEL_TWO); //Set channel output high
					}
				}
				break;
			case STATE_ACTIVE:
				if(!(PINB & _BV((channelVarUI8 == CHANNEL_ONE)?IN_CHANNEL_ONE:IN_CHANNEL_TWO))){ //If state active and input signal low
					inputStateUI8A[channelVarUI8] = STATE_IDLE;
					if(channelVarUI8 == CHANNEL_ONE){
						lastPulseValUI16A[CHANNEL_ONE] = TCNT0; //Save the length of last pulse
						flagsUI8 |= FLAGS_CH_ONE_RECV; //Mark that we received a new pulse (for sync)
					}else{
						lastPulseValUI16A[CHANNEL_TWO] = TCNT1; //Save the length of last pulse
						flagsUI8 |= FLAGS_CH_TWO_RECV; //Mark that we received a new pulse (for sync)
					}
				}
				break;
		}
	}
}

void inline waitNextPulse(void){
	flagsUI8 &= ~FLAGS_CH_ONE_RECV;
	while(!(flagsUI8 & FLAGS_CH_ONE_RECV));
	flagsUI8 &= ~FLAGS_CH_ONE_RECV;
	while(!(flagsUI8 & FLAGS_CH_ONE_RECV));
}

void clockSync(void){
	int16_t curDevI16, minDevI16 = INT16_MAX;
	uint8_t stepValueUI8 = 64, trialValueUI8 = _BV(7), minTrialValueUI8 = UINT8_MAX, tmpTrialValueUI8;
	
	do{
		OSCCAL = trialValueUI8 + stepValueUI8;
		waitNextPulse();
		if(lastPulseValUI16A[CHANNEL_ONE] < PWM_MIDDLE)
			trialValueUI8 += stepValueUI8;
		stepValueUI8 >>= 1;
	}while(stepValueUI8);
	
	for(tmpTrialValueUI8 = trialValueUI8 - 1; tmpTrialValueUI8 <= trialValueUI8 + 1; tmpTrialValueUI8++){
		OSCCAL = tmpTrialValueUI8;
		waitNextPulse();
		curDevI16 = abs(lastPulseValUI16A[CHANNEL_ONE] - PWM_MIDDLE);
		if(curDevI16 < minDevI16){
			minDevI16 = curDevI16;
			minTrialValueUI8 = tmpTrialValueUI8;
		}
	}

	OSCCAL = minTrialValueUI8;
	flagsUI8 |= FLAGS_SYNC_DONE;
	PORTB |= _BV(OUT_CHANNEL_LED);
}

void init(void){
	
	DDRB |= _BV(OUT_CHANNEL_ONE) | _BV(OUT_CHANNEL_TWO) | _BV(OUT_CHANNEL_LED); //Set output channels as outputs
	PORTB |= _BV(IN_CHANNEL_ONE) | _BV(IN_CHANNEL_TWO); //Enable pull-ups for input channels
	
	lastPulseValUI16A[CHANNEL_ONE] = lastPulseValUI16A[CHANNEL_TWO] = PWM_MIDDLE; //Clear last pules length values initially
	mixPulseValUI16A[CHANNEL_ONE] = mixPulseValUI16A[CHANNEL_TWO] = PWM_MIDDLE; //Set initial mixing output values
	inputStateUI8A[CHANNEL_ONE] = inputStateUI8A[CHANNEL_TWO] = STATE_IDLE; //Set start states to idle
	flagsUI8 = 0; //Blank initial flags
	
	TIMSK |= _BV(OCIE0A) | _BV(TOIE0) | _BV(OCIE1A) | _BV(TOIE1); //TMR0/1 Interrupt on compare match and overflow
	
	PCMSK |= _BV(IN_CHANNEL_ONE) | _BV(IN_CHANNEL_TWO); //Enable pin change interrupts on input pins
	GIMSK |= _BV(PCIE); //Enable pin change interrupts globally
	
	sei();
}

int main(void){
	
	_delay_ms(100);
	
	init();
	clockSync();

	while(1){

	}
}