/*
 * DrivingH.h
 *
 *  Created on: 28 apr. 2022
 *      Author: Arnau Quintana
 */

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>


#ifndef DrivingH_H_
#define DrivingH_H_

#define FORWARD 0
#define BACK 1
#define STOP 2
#define FAST 3
#define SLOW 4

void init_timer_US(void);
uint8_t US_value(void);
void init_timers_GPIO_HBridge(void);
void Actuator_Move(char Direction, char Speed);

#endif
