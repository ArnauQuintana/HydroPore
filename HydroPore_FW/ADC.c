/*
 * ADC.c
 *
 *  Created on: 17 apr. 2022
 *  Last edit: 09 sep. 2022
 *      Author: Arnau Quintana
 *
 * This file contains all the functions related to ADC
 */

#include <msp430.h>
#include "ADC.h"
#include <stdint.h>
#include <stdio.h>

unsigned int ADC_RESULT;

void init_ADC(void)
{
    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
                         // to activate previously configured port settings
    P1SEL0 |= BIT4 | BIT3 | BIT2; // Enable A/D channel inputs
    P1SEL1 |= BIT4 | BIT3 | BIT2; // Enable A/D channel inputs
    /*
     * P1.4 -> LVDT1. P1.3 -> LVDT2. P1.2 -> Water Preassure Sensor
     */
    ADC12CTL0 |= ADC12ON;
    /*
     * ADC ON
     */
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
    /*
     * Disable ADC conversions
     * No sample-and-conversion start
     */
    ADC12CTL0 |= (ADC12SHT0_2);
    /*
     *Sample and hold time -> 16 ADC12CLK cycles
     */
    ADC12CTL1 |= ADC12SHP | ADC12SSEL_0;
    /*
     * SAMPCON sourced from sampling timer
     * Clock source -> MODOSC
     */

    ADC12CTL2 |= ADC12RES_2;
    /*
     * ADC resolution -> 12 bits
     */
    ADC12MCTL0 |= ADC12INCH_2;
    /*
     * ADC channel, we will need to reprogram this register in every conversion because we use various channels
     * ADC12MCTLx where x will indicate in which ADCMEMx register will go the converted data
     */
    ADC12IER0 |= ADC12IE0;
    /*
     * Enable interrupt for a conversion complete for ADCMEM0
     */
}

/*
 * Functions for LVDT1 (LVDT connected horizontally with the structure)
 * This function will return the value to be sent to PC
 */
uint16_t measure_LVDT1(void)
{
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
    ADC12MCTL0 = ADC12INCH_2; //Channel selector
    ADC12IFGR0 &= ~ADC12IFG0; //Clear the flag to ensure correct operation
    ADC12CTL0 |= ADC12ENC | ADC12SC; //Enable i Start Conversion

    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();

    return ADC_RESULT;
}
/*
 * Function for LVDT2 (LVDT connected vertically with the structure)
 * This function will return the value to be sent to PC
 */

uint16_t measure_LVDT2(void)
{
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
    ADC12MCTL0 = ADC12INCH_9; //Channel selector
    ADC12IFGR0 &= ~ADC12IFG0; //Clear the flag to ensure correct operation
    ADC12CTL0 |= ADC12ENC | ADC12SC; //Enable i Start Conversion

    __bis_SR_register(LPM0_bits + GIE);
    __no_operation();

    return ADC_RESULT;
}

/*
 * Function for Water Preassure Sensor
 * This function will return the value to be sent to PC
 */
uint16_t measure_WaterSensor(void)
{
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
    ADC12MCTL0 = ADC12INCH_8; //Channel selector
    ADC12IFGR0 &= ~ADC12IFG0; //Clear the flag to ensure correct operation
    ADC12CTL0 |= ADC12ENC | ADC12SC; //Enable i Start Conversion

    __bis_SR_register(LPM0_bits + GIE); //'go to sleep' until an interrupt is produced
    __no_operation();

    return ADC_RESULT;
}

/*
 * This function will measure the feedback from the actuator
 * When actuator is completely retracted, the output will be 0, if is in maximum extension (50mm), the output will be
 * 4096 (12bits of resolution).
 */
uint16_t measure_feedbackActuator(void)
{
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
    ADC12MCTL0 = ADC12INCH_14; //Channel selector
    ADC12IFGR0 &= ~ADC12IFG0; //Clear the flag to ensure correct operation
    ADC12CTL0 |= ADC12ENC | ADC12SC; //Enable i Start Conversion

    __bis_SR_register(LPM0_bits + GIE); //'go to sleep' until an interrupt is produced
    __no_operation();

    return ADC_RESULT;
}

/*
 * Interrupt routine for the ADC
 */
#pragma vector = ADC12_VECTOR
__interrupt void ADC_ISR(void)
{
    switch(__even_in_range(ADC12IV,ADC12IV_ADC12IFG0))
    {
        case ADC12IV_NONE:
        break;
        case ADC12IV_ADC12OVIFG:
        break;
        case ADC12IV_ADC12TOVIFG:
        break;
        case ADC12IV_ADC12HIIFG:
        break;
        case ADC12IV_ADC12LOIFG:
        break;
        case ADC12IV_ADC12INIFG:
        break;
        case ADC12IV_ADC12IFG0: //Flag that indicates a completed conversion
            ADC_RESULT = ADC12MEM0; //Save conversion result in a variable
            ADC12CTL0 &= ~(ADC12ENC | ADC12SC); //Disable the ADC to stop conversions

            __bic_SR_register_on_exit(LPM0_bits); //Wake up, once we have all data collected

        break;
        default:
        break;
    }
}
