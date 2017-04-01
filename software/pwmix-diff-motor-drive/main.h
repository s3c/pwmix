/*
 * main.h
 *
 * Created: 9/8/2016 4:50:35 AM
 *  Author: User
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>

#define abs(x) (((x)>0)?(x):(-x))

#define FLAGS_NONE			0
#define FLAGS_CH_ONE_RECV   1
#define FLAGS_CH_TWO_RECV   2
#define FLAGS_SYNC_DONE		4
#define FLAGS_PIN_CHANGE	8

#define CHANNEL_ONE			0
#define CHANNEL_TWO			1

#define PWM_MIN				125
#define PWM_MIDDLE			188
#define PWM_MAX				250
#define PWM_SYNC			188

#define STATE_IDLE			0
#define STATE_ACTIVE		1

#define IN_CHANNEL_ONE		PB3
#define IN_CHANNEL_TWO		PB4

#define OUT_CHANNEL_ONE		PB1
#define OUT_CHANNEL_TWO		PB2
#define OUT_CHANNEL_LED		PB0

#endif /* MAIN_H_ */