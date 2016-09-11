/*
 * pwmix.c
 *
 * Created: 9/8/2016 3:37:51 AM
 * Author : User
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"

volatile uint16_t channelValLastUI16A[2];
volatile uint8_t channelValCurUI8A[2];
volatile uint8_t channelStateUI8A[2];
volatile int16_t mixedChannelsI16A[2];
volatile uint8_t activeTimeUI8A[2];

ISR(TIM0_COMPA_vect){ //Executed every 10uS
	uint8_t channelVarUI8, channelTmpUI8;
	static uint16_t tempLEDVarUI16 = 0;

	//add delay here

	if(++tempLEDVarUI16 == 50000){
		PORTB ^= _BV(OUT_CHANNEL_LED);
		tempLEDVarUI16 = 0;
	}

	for(channelVarUI8 = CHANNEL_ONE; channelVarUI8 <= CHANNEL_TWO; channelVarUI8++){ //Check both input channels
		switch(channelStateUI8A[channelVarUI8]){
			case STATE_IDLE:
				if(PINB & _BV((channelVarUI8 == CHANNEL_ONE)?IN_CHANNEL_ONE:IN_CHANNEL_TWO)){ //If state idle and input signal high
					channelStateUI8A[channelVarUI8] = STATE_ACTIVE;
					channelValCurUI8A[channelVarUI8] = 0;
					activeTimeUI8A[channelVarUI8] = mixedChannelsI16A[channelVarUI8];
					PORTB |= _BV((channelVarUI8 == CHANNEL_ONE)?OUT_CHANNEL_ONE:OUT_CHANNEL_TWO);
				}
				break;
			case STATE_ACTIVE:
				if(PINB & _BV((channelVarUI8 == CHANNEL_ONE)?IN_CHANNEL_ONE:IN_CHANNEL_TWO)){ //If state active and input signal high
					if(channelValCurUI8A[channelVarUI8] <= PWM_INVALID_MAX)
						channelValCurUI8A[channelVarUI8]++;
				}else{ //If state active and input signal low
					if((channelValCurUI8A[channelVarUI8] <= PWM_INVALID_MAX) && (channelValCurUI8A[channelVarUI8] >= PWM_INVALID_MIN)){ //Save pulse length if it was valid
						channelValLastUI16A[channelVarUI8] = channelValCurUI8A[channelVarUI8];
						
						//mixedChannelsI16A[CHANNEL_ONE] = channelValLastUI16A[CHANNEL_ONE] + (channelValLastUI16A[CHANNEL_TWO] - PWM_MIDDLE);
						//mixedChannelsI16A[CHANNEL_ONE] = channelValLastUI16A[CHANNEL_ONE];
						mixedChannelsI16A[CHANNEL_ONE] = PWM_MIDDLE;
						//mixedChannelsI16A[CHANNEL_TWO] = channelValLastUI16A[CHANNEL_ONE] - (channelValLastUI16A[CHANNEL_TWO] - PWM_MIDDLE);
						//mixedChannelsI16A[CHANNEL_TWO] = channelValLastUI16A[CHANNEL_TWO];
						mixedChannelsI16A[CHANNEL_TWO] = PWM_MIDDLE;
						
						//for(channelTmpUI8 = CHANNEL_ONE; channelTmpUI8 <= CHANNEL_TWO; channelTmpUI8++){
						//	if(mixedChannelsI16A[channelTmpUI8] > PWM_INVALID_MAX)
						//		mixedChannelsI16A[channelTmpUI8] = PWM_INVALID_MAX;	
						//	if(mixedChannelsI16A[channelTmpUI8] < PWM_INVALID_MIN)
						//		mixedChannelsI16A[channelTmpUI8] = PWM_INVALID_MIN;
						//}
					}
					channelStateUI8A[channelVarUI8] = STATE_IDLE;
				}
				break;
		}
		if(activeTimeUI8A[channelVarUI8])
			activeTimeUI8A[channelVarUI8]--;
		else
			PORTB &= ~_BV((channelVarUI8 == CHANNEL_ONE)?OUT_CHANNEL_ONE:OUT_CHANNEL_TWO);
	}
}

void init(void){
	
	DDRB |= _BV(OUT_CHANNEL_ONE) | _BV(OUT_CHANNEL_TWO) | _BV(OUT_CHANNEL_LED); //Set output channels as outputs
	PORTB |= _BV(IN_CHANNEL_ONE) | _BV(IN_CHANNEL_TWO); //Enable pull-ups for input channels
	
	channelValLastUI16A[CHANNEL_ONE] = channelValLastUI16A[CHANNEL_TWO] = PWM_MIDDLE; //Set initial PWM values to idle
	channelValCurUI8A[CHANNEL_ONE] = channelValCurUI8A[CHANNEL_TWO] = PWM_MIDDLE; //Set initial PWM values to idle
	channelStateUI8A[CHANNEL_ONE] = channelStateUI8A[CHANNEL_TWO] = STATE_IDLE; //Set initial channel state to idle
	mixedChannelsI16A[CHANNEL_ONE] = mixedChannelsI16A[CHANNEL_TWO] = PWM_MIDDLE; //Set initial PWM values to idle
	activeTimeUI8A[CHANNEL_ONE] = activeTimeUI8A[CHANNEL_TWO] = PWM_MIDDLE; //Set initial PWM values to idle
	
	TCCR0A = _BV(WGM01); //Enable CTC on timer 0
	TCCR0B = _BV(CS01); //Set prescaler to 8
	OCR0A = 20; //Set to overflow on 10uS
	TIMSK = _BV(OCIE0A); //interrupt on compare match
	
	sei();
}

int main(void){
	
	init();

    while(1){

	}
}

