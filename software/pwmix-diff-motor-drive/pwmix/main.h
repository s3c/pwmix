/*
 * main.h
 *
 * Created: 9/8/2016 4:50:35 AM
 *  Author: User
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>

#define CHANNEL_ONE			0
#define CHANNEL_TWO			1

#define PWM_INVALID_MIN		80
#define PWM_INVALID_MAX		220
#define PWM_MIDDLE			150

#define STATE_IDLE			0
#define STATE_ACTIVE		1

#define IN_CHANNEL_ONE		PB3
#define IN_CHANNEL_TWO		PB4

#define OUT_CHANNEL_ONE		PB1
#define OUT_CHANNEL_TWO		PB2
#define OUT_CHANNEL_LED		PB0

#endif /* MAIN_H_ */