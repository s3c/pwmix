/*
 * pwmix2.c
 *
 * Created: 9/10/2016 7:29:07 AM
 * Author : User
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"

volatile uint8_t systemStateUI8; //Stores the current state

void copy_output_input(void){
	if(PINB & _BV(IN_CHANNEL_PPM))
		PORTB |= _BV(OUT_CHANNEL_PPM);
	else
		PORTB &= ~_BV(OUT_CHANNEL_PPM);
}


ISR(TIM1_OVF_vect){
	systemStateUI8 = STATE_WAIT;
	PORTB ^= _BV(OUT_CHANNEL_LED);
	TCCR1 = 0; //Disable TMR1
}

ISR(PCINT0_vect){

	TCNT1 = 0; //Reset TMR1
	GTCCR |= _BV(PSR1); //Reset TMR1 prescaler

	switch(systemStateUI8){
		case(STATE_IDLE):
			copy_output_input();
			break;
		case(STATE_WAIT):
			systemStateUI8 = STATE_PULSE_1_START;
			PORTB |= _BV(OUT_CHANNEL_ONE);
			break;
		case(STATE_PULSE_1_START):
			systemStateUI8 = STATE_PULSE_1_MID;
			break;
		case(STATE_PULSE_1_MID):
			systemStateUI8 = STATE_PULSE_2_START;
			PORTB &= ~_BV(OUT_CHANNEL_ONE);
			PORTB |= _BV(OUT_CHANNEL_TWO);
			break;
		case(STATE_PULSE_2_START):
			systemStateUI8 = STATE_PULSE_2_MID;
			break;
		case(STATE_PULSE_2_MID):
			systemStateUI8 = STATE_IDLE;
			PORTB &= ~_BV(OUT_CHANNEL_TWO);
			copy_output_input();
			TCCR1 |= _BV(CS13); //TMR1 128 prescaler
			break;
	}
}

void init(void){
	
	systemStateUI8 = STATE_IDLE; //Set start state to idle
	
	DDRB |= _BV(OUT_CHANNEL_PPM) | _BV(OUT_CHANNEL_ONE) | _BV(OUT_CHANNEL_TWO) | _BV(OUT_CHANNEL_LED); //Set output channels as outputs
	PORTB |= _BV(IN_CHANNEL_PPM); //Enable pull-ups for input channel
	copy_output_input();
	
	TIMSK |= _BV(TOIE1); //TMR1 Interrupt on overflow
	TCCR1 |= _BV(CS13); //TMR1 128 prescaler
	
	PCMSK |= _BV(IN_CHANNEL_PPM); //Enable pin change interrupts on input pin
	GIMSK |= _BV(PCIE); //Enable pin change interrupts globally
	
	sei();
}

int main(void){
	
	init(); //Initialize all peripherals and variables

	while(1){}
}