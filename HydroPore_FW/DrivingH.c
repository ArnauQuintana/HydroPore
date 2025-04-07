/*
 * DrivingH.c
 *
 *  Created on: 28 apr. 2022
 *      Author: Arnau Quintana
 *
 * This file contains the functions related with de H Bridge
 */

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "DrivingH.h"
#include "ADC.h"

/*********************************ULTRASONIC SENSOR*****************************************/

uint16_t Value1_capture, Value2_capture, Capture_result;
float ReturnData;
uint8_t Captured_data;

/*
 * Function that will initialitzes the timer that control the US sensor
 */
void init_timer_US(void)
{
    TA4CTL |= MC_2 | TASSEL_2 | ID_3 | TACLR; //Mode Continuous. Timer CLK = SMCLK/8 = 2MHz. Clear.
    TA4EX0 |= TAIDEX_7; //Timer CLK = 2MHz / 8 = 250KHz -> Periodo de 4us.
    TA4CCTL1 |= CM_3 | CCIS_0 | SCS | CAP | CCIE;
    /* Capture in the two edges
     * Input CCI1A (P5.5, TA4.1)
     * Synchronous Capture
     * Capture Mode
     * Interruption ON
     */
    TA4CCTL1 &= ~CCIFG; //To ensure correct operation, clear the interruption flag

    P5DIR |= BIT3;  //GPIO that will trigger the sensor
    P5SEL0 |= BIT5; //Alternative function for the pin that will capture the values from the US sensor

    P5OUT &= ~BIT3;
}

/*
 * Function that will control de US sensor
 */
uint8_t US_value(void)
{
    uint8_t Timeout = 0;

    Captured_data = 0; //Will capture 2 values, rising edge and falling edge
    Value1_capture = 0;  //Value captured in rising edge
    Value2_capture = 0; //Value captures in falling edge

    P5OUT |= BIT3;  //Trigger the pulse to get data from the sensor
    __delay_cycles(16000); //MCLK = 16MHz. So 16000 will give a 1ms delay
    P5OUT &= ~BIT3;
    TA4CTL |= TACLR; //Reset el TB3R register

    while(Captured_data < 2) //We need 2 values from the US, so we will wait here until the interrupt shoots two times
    {
        Timeout++;
        if(Timeout > 250)
        {
            Timeout = 250;
            break;
        }
    }

    Capture_result = Value2_capture - Value1_capture; //Clock pulses between the two captured values

    ReturnData = Capture_result * (4e-3); //Period de 4us, yet we want the result in ms, that's why e-3 and no e-6.

    if(Timeout == 250) //Ultrasonic sensor is not connected
    {                  //Is an extra protection so we can use the system without it
        return 0;      //We return a 0 to avoid a infinite loop waiting for data
    }
    else
    {
        if (ReturnData < 0.400) //Range of pulse between 100us-18ms, in this case >5ms.
        {
            return 1; //object detected
        }
        else
        {
            return 0; //No object detected
        }
    }
}

/*
 * Interrupt routine of the timer that control US sensor
 */

#pragma vector = TIMER4_A1_VECTOR
__interrupt void Timer4_A1_ISR (void)
{
    TA4CCTL1 &= ~CCIFG;
    if(Captured_data == 0) Value1_capture = TA4CCR1; //First interrupt will occur with a rising edge
    else Value2_capture = TA4CCR1; //Second interrupt will occuren in a falling edge
    Captured_data++;
}

/******************************ACTUATOR******************************************/

/*
 * Timers to generate the two PWM signals of the high side transistors
 */
void init_timers_GPIO_HBridge(void)
{
    //GPIO for the OV signal
    P3DIR &= ~BIT5; //Pin to read the digital value from the OV circuit
    P3REN |= BIT5; //Enable pullup/pulldown resistor
    P3OUT &= ~BIT5; //Pulldown. In normal working, OV circuit will return 1.

    //GPIO pins for the low-side transitors
    P6DIR |= BIT7 + BIT3; //P6.3 -> transistor D. P6.7 -> transistor B
    P6OUT &= 0x00; //To ensure that initially they remain off

    //PWM pins for the high-side transistors
    P3DIR |= BIT2 + BIT3; //Pins as outputs
    P3SEL1 &= ~BIT2;  //Function of the pins PWM Transistor A
    P3SEL0 |= BIT2;
    P3SEL0 &= ~BIT3;  //Function of the pins PWM Transistor C
    P3SEL1 |= BIT3;

    //PWM for the A transistor in the scheme (Timer A1 block 1)
    TA1CCTL1 = OUTMOD_0; //We start with Mode 0, but we need mode 7 to generate PWM signal
    TA1CCR0 = 1000 - 1; //PWM frequency = 1kHz
    TA1CCR1 = 110; //110 equal to 11% of duty cycle, we can change this value during the program
    TA1CTL = TASSEL_2 + MC_1 + ID_3 + TACLR;//SMCLK, Count Up, ID = /8 (16MHz->8MHz), Clear TA0R register
    TA1EX0 = TAIDEX_1;//8Mhz/2 -> 4Mhz

    //PWM for the C transistor in the scheme (Timer B0 block 3)
    TB0CCTL3 = OUTMOD_0; //We start with Mode 0, but we need mode 7 to generate PWM signal
    TB0CCR0 = 1000 - 1; //PWM frequency = 1kHz
    TB0CCR3 = 110; //110 equal to 11% of duty cycle, we can change this value during the program
    TB0CTL = TBSSEL_2 + MC_1 + ID_3 + TBCLR;//SMCLK, Count Up, ID = /1, Clear TB0R register
    TB0EX0 = TAIDEX_1; //8Mhz/2 -> 4Mhz
}

/*
 * Function that will decide the movement of the actuator
 */
void Actuator_Move(char Direction, char Speed)
{
    switch(Direction)
    {
    case FORWARD:
        TA1CCTL1 = OUTMOD_7; //Enable transistor A
        TB0CCTL3 = OUTMOD_0; //Disable transistor C
        P6OUT |= 0x08; //P6.3 to activate transistor D
        break;
    case BACK:
        TA1CCTL1 = OUTMOD_0; //Disable transistor A
        TB0CCTL3 = OUTMOD_7; //Enable transistor C
        P6OUT |= 0x80; //P6.7 to activate transistor B
        break;
    case STOP:
        TA1CCTL1 = OUTMOD_0; //Disable two high side transistors
        TB0CCTL3 = OUTMOD_0;
        P6OUT &= 0x77; //Disable two low side transistors
        break;
    }

    switch(Speed)
    {
    case FAST:
        if(TA1CCR1 < 191) //If it is slower than 19%, we can increase the duty cycle up to 20%
        {
            TA1CCR1 += 10;
            TB0CCR3 += 10;
        }
        else //If we enter to this condition, will mean that we are working at 20%, is the maximum duty cycle value permitted
        {
            TA1CCR1 = TA1CCR1;//We won't change the duty cycle
            TB0CCR3 = TB0CCR3;
        }
        break;
    case SLOW: //If we want to go slow
        if(TA1CCR1 > 119) //We will check if the duty is higher than 11%
        {
            TA1CCR1 -= 10; //We will decrease the duty cycle
            TB0CCR3 -= 10;
        }
        else //if is is lower than 11%, it means that we are working at 10% of duty, it is the slowest value permitted
        {
            TA1CCR1 = TA1CCR1; //We won't change the duty cycle
            TB0CCR3 = TB0CCR3;
        }
        break;
    default: //default case is to do anything
        TA1CCR1 = TA1CCR1; //We won't change the duty cycle
        TB0CCR3 = TB0CCR3;
        break;
    }
}

