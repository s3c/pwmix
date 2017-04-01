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

volatile int16_t lastPulseValI16A[2]; //Stores the last pulse received for each channel
volatile int16_t mixPulseValI16A[2]; //Stores the output pulse after mixing
volatile uint8_t inputStateUI8A[2]; //Stores the state of each channel
volatile uint8_t flagsUI8; //Stores sync flags 

ISR(TIM0_COMPA_vect){ //Turn off channel one output on compare match
	PORTB &= ~_BV(OUT_CHANNEL_ONE);
}

ISR(TIM1_COMPA_vect){ //Turn off channel two output on compare match
	PORTB &= ~_BV(OUT_CHANNEL_TWO);
}

ISR(TIM0_OVF_vect){
	//flagsUI8 |= FLAGS_CH_ONE_RECV;
	if(inputStateUI8A[CHANNEL_ONE] == STATE_ACTIVE) //If we had an overflow before the pulse ended
		lastPulseValI16A[CHANNEL_ONE] = PWM_MAX; //Just save the value as longest valid pulse length
	inputStateUI8A[CHANNEL_ONE] = STATE_IDLE;
	PORTB &= ~_BV(OUT_CHANNEL_ONE); //Should never be needed
	TCCR0B = 0; //Disable TIMR0
}

ISR(TIM1_OVF_vect){
	//flagsUI8 |= FLAGS_CH_TWO_RECV;
	if(inputStateUI8A[CHANNEL_TWO] == STATE_ACTIVE) //If we had an overflow before the pulse ended
		lastPulseValI16A[CHANNEL_TWO] = PWM_MAX; //Just save the value as longest valid pulse length
	inputStateUI8A[CHANNEL_TWO] = STATE_IDLE;
	PORTB &= ~_BV(OUT_CHANNEL_TWO); //Should never be needed
	TCCR1 = 0; //Disable TIMR1
}

ISR(PCINT0_vect, ISR_NOBLOCK){ //Executed in input pin change
	uint8_t channelVarUI8, channelTmpUI8;
	
	if(flagsUI8 & FLAGS_PIN_CHANGE)
		return;
	flagsUI8 |= FLAGS_PIN_CHANGE;
	
	for(channelVarUI8 = CHANNEL_ONE; channelVarUI8 <= CHANNEL_TWO; channelVarUI8++){ //Check both input channels
		switch(inputStateUI8A[channelVarUI8]){
			case STATE_IDLE:
				if(PINB & _BV((channelVarUI8 == CHANNEL_ONE)?IN_CHANNEL_ONE:IN_CHANNEL_TWO)){ //If state idle and input signal high
					inputStateUI8A[channelVarUI8] = STATE_ACTIVE;
					//lastPulseValI16A[CHANNEL_ONE] = PWM_MIDDLE;
					//lastPulseValI16A[CHANNEL_TWO] = PWM_MIDDLE;
					mixPulseValI16A[CHANNEL_ONE] = lastPulseValI16A[CHANNEL_ONE] + (lastPulseValI16A[CHANNEL_TWO] - PWM_MIDDLE); //Calculate mixint outputs from previous pulse lengths
					mixPulseValI16A[CHANNEL_TWO] = lastPulseValI16A[CHANNEL_ONE] - (lastPulseValI16A[CHANNEL_TWO] - PWM_MIDDLE); //Calculate mixint outputs from previous pulse lengths
					for(channelTmpUI8 = CHANNEL_ONE; channelTmpUI8 <= CHANNEL_TWO; channelTmpUI8++){ //Make sure the mixed values are within reasonable limits
						if(mixPulseValI16A[channelTmpUI8] > PWM_MAX)
							mixPulseValI16A[channelTmpUI8] = PWM_MAX;
						/*else*/ if(mixPulseValI16A[channelTmpUI8] < PWM_MIN)
							mixPulseValI16A[channelTmpUI8] = PWM_MIN;
					}
					if(channelVarUI8 == CHANNEL_ONE){
						OCR0A = mixPulseValI16A[CHANNEL_ONE]; //Set compare match to stop output pulse
						TCNT0 = 0; //Reset TMR0 				
						GTCCR |= _BV(PSR0); //Reset TMR0 prescaler
						TCCR0B |= _BV(CS01) | _BV(CS00); //Start TMR0 with 64 prescaler
						if(flagsUI8 & FLAGS_SYNC_DONE)
							PORTB |= _BV(OUT_CHANNEL_ONE); //Set channel output high
						//break; //Some weird edge case can happen to make signals jump, think this happens when an int fires while we're in here, not convinced this fixes it yet...
					}else{
						OCR1A = mixPulseValI16A[CHANNEL_TWO]; //Set compare match to stop output pulse
						TCNT1 = 0; //Reset TMR1 
						GTCCR |= _BV(PSR1); //Reset TMR1 prescaler
						TCCR1 |= _BV(CS12) | _BV(CS11) | _BV(CS10); //Start TMR1 with 64 prescaler
						if(flagsUI8 & FLAGS_SYNC_DONE)
							PORTB |= _BV(OUT_CHANNEL_TWO); //Set channel output high
						//break; //Some weird edge case can happen to make signals jump, think this happens when an int fires while we're in here, not convinced this fixes it yet...
					}
				}
				break;
			case STATE_ACTIVE:
				if(!(PINB & _BV((channelVarUI8 == CHANNEL_ONE)?IN_CHANNEL_ONE:IN_CHANNEL_TWO))){ //If state active and input signal low
					inputStateUI8A[channelVarUI8] = STATE_IDLE;
					if(channelVarUI8 == CHANNEL_ONE){
						lastPulseValI16A[CHANNEL_ONE] = TCNT0; //Save the length of last pulse
						flagsUI8 |= FLAGS_CH_ONE_RECV; //Mark that we received a new pulse (for sync)
						//break; //Some weird edge case can happen to make signals jump, think this happens when an int fires while we're in here, not convinced this fixes it yet...
					}else{
						lastPulseValI16A[CHANNEL_TWO] = TCNT1; //Save the length of last pulse
						flagsUI8 |= FLAGS_CH_TWO_RECV; //Mark that we received a new pulse (for sync)
						//break; //Some weird edge case can happen to make signals jump, think this happens when an int fires while we're in here, not convinced this fixes it yet...
					}
				}
				break;
		}
	}
	
	flagsUI8 &= ~FLAGS_PIN_CHANGE;
}

void inline waitNextPulse(void){
	flagsUI8 &= ~FLAGS_CH_ONE_RECV; //First pulse might be incomplete do to changing clock mid pulse, so disregard
	while(!(flagsUI8 & FLAGS_CH_ONE_RECV)); //Wait till we have a new pulse length
	flagsUI8 &= ~FLAGS_CH_ONE_RECV; //Redo and get a valid pulse length
	while(!(flagsUI8 & FLAGS_CH_ONE_RECV)); //Wait till pulse is done
}

void clockSync(void){ //Sync clock to receiver, assuming first couple of pulses from receiver is at mid stick, ie, 1.5mS
	int16_t curDevI16, minDevI16 = INT16_MAX;
	uint8_t stepValueUI8 = 64, trialValueUI8 = _BV(7), minTrialValueUI8 = UINT8_MAX, tmpTrialValueUI8;
	
	do{ //Binary search over 7 bits of OSCCAL, bit 7 is high range bit
		OSCCAL = trialValueUI8 + stepValueUI8;
		waitNextPulse();
		if(lastPulseValI16A[CHANNEL_ONE] < PWM_SYNC)
			trialValueUI8 += stepValueUI8;
		stepValueUI8 >>= 1;
	}while(stepValueUI8);
	
	for(tmpTrialValueUI8 = trialValueUI8 - 1; tmpTrialValueUI8 <= trialValueUI8 + 1; tmpTrialValueUI8++){ //Local search to identify closest match
		OSCCAL = tmpTrialValueUI8;
		waitNextPulse();
		curDevI16 = abs(lastPulseValI16A[CHANNEL_ONE] - PWM_SYNC);
		if(curDevI16 < minDevI16){
			minDevI16 = curDevI16;
			minTrialValueUI8 = tmpTrialValueUI8;
		}
	}

	OSCCAL = minTrialValueUI8; //Set closest clock value
	flagsUI8 |= FLAGS_SYNC_DONE; //Set sync as complete, so output module can start generating pulses
	PORTB |= _BV(OUT_CHANNEL_LED); //Turn on LED to notify setup complete
}

void init(void){
	
	DDRB |= _BV(OUT_CHANNEL_ONE) | _BV(OUT_CHANNEL_TWO) | _BV(OUT_CHANNEL_LED); //Set output channels as outputs
	PORTB |= _BV(IN_CHANNEL_ONE) | _BV(IN_CHANNEL_TWO); //Enable pull-ups for input channels
	
	lastPulseValI16A[CHANNEL_ONE] = lastPulseValI16A[CHANNEL_TWO] = PWM_MIDDLE; //Clear last pules length values initially
	mixPulseValI16A[CHANNEL_ONE] = mixPulseValI16A[CHANNEL_TWO] = PWM_MIDDLE; //Set initial mixing output values
	inputStateUI8A[CHANNEL_ONE] = inputStateUI8A[CHANNEL_TWO] = STATE_IDLE; //Set start states to idle
	flagsUI8 = FLAGS_NONE; //Blank initial flags
	
	TIMSK |= _BV(OCIE0A) | _BV(TOIE0) | _BV(OCIE1A) | _BV(TOIE1); //TMR0/1 Interrupt on compare match and overflow
	
	PCMSK |= _BV(IN_CHANNEL_ONE) | _BV(IN_CHANNEL_TWO); //Enable pin change interrupts on input pins
	GIMSK |= _BV(PCIE); //Enable pin change interrupts globally
	
	sei();
}

int main(void){
	
	_delay_ms(100); //Make sure we don't get any half/stray pulses
	
	init(); //Initialize all peripherals and variables
	clockSync(); //Perform the clock sync, as INT RC OSC is pretty crap, assume receiver is correct

	while(1){

	}
}