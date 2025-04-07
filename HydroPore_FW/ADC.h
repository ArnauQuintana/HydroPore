/*
 * ADC.h
 *
 *  Created on: 17 apr. 2022
 *      Author: Arnau Quintana
 */

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#ifndef ADC_H_
#define ADC_H_

#define YES 1
#define NO 0

void init_ADC(void);
uint16_t measure_LVDT1(void);
uint16_t measure_LVDT2(void);
uint16_t measure_WaterSensor(void);
uint16_t measure_feedbackActuator(void);

#endif
