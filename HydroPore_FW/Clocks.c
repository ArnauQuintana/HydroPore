/*
 * Clocks.c
 *
 *  Created on: 28 abr. 2022
 *      Author: Arnau Quintana
 */

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>

#include "Clocks.h"

void init_clocks(void)
{
    PJSEL0 |= BIT4 | BIT5; // For XT1

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCORSEL | DCOFSEL_4;          // Set DCO to 16MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; //Clock Sources
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers to 1
    CSCTL4 |= LFXTDRIVE_3;
    CSCTL4 &= ~(LFXTOFF);
    do
    {
        CSCTL5 &= ~(LFXTOFFG);   // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);   // Test oscillator fault flag*/
    CSCTL0_H = 0;
}


