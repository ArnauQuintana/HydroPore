/*
 * COMs.h
 *
 *  Created on: 17 apr. 2022
 *      Author: Arnau Quintana
 */


#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#ifndef Control_H_
#define Control_H_

typedef struct System_Data
{
    uint16_t LVDT1;
    uint16_t Water;
    uint16_t Load1;
    uint16_t Load2;
    uint16_t Feedback;
}System_Data;

void Analyze_received_data(const uint8_t *Parameters);

#endif
