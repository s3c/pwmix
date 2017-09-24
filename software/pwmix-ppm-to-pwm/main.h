/*
 * main.h
 *
 * Created: 9/8/2016 4:50:35 AM
 *  Author: User
 */ 

#ifndef MAIN_H_
#define MAIN_H_

#define STATE_IDLE				0
#define STATE_WAIT				1
#define STATE_PULSE_1_START		2
#define STATE_PULSE_1_MID		3
#define STATE_PULSE_2_START		4
#define STATE_PULSE_2_MID		5

#define IN_CHANNEL_PPM			PB3

#define OUT_CHANNEL_ONE			PB1
#define OUT_CHANNEL_TWO			PB2
#define OUT_CHANNEL_LED			PB0
#define OUT_CHANNEL_PPM			PB4

#endif /* MAIN_H_ */