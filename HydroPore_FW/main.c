/*
 * Project: HydroPore
 *
 * This project is realized by Arnau Quintana, for the University of Barcelona.
 * Its distribution is totally prohibited without permission of its author or the project members.
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#include "ADC.h"
#include "COMs.h"
#include "Clocks.h"
#include "DrivingH.h"
#include "Control.h"

int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	__disable_interrupt();

	/*GPIOs that will control activation of the external ADCs*/
    P4DIR |= BIT6 + BIT5; //Bit 5 for the load cell 2 and bit for the load cell 1
    P4OUT |= BIT6 + BIT5; //High state will maintain ADC in shutdown

    /*We use the I2C pins as GPIOs for indicator LEDs*/
    P3DIR |= BIT6 + BIT7;
    P3OUT |= BIT6;
    P3OUT &= ~BIT7;

	init_clocks(); //Initialize clock module
	init_GPIOs_Cams();//Initialize GPIOs that control cameras
	init_UART_COM(); //Initialize module in UART mode in order to receive commands
	init_timer_US(); //Initialize timer that controls the ultrasonic sensor
	init_timers_GPIO_HBridge(); //Initialize all PWM and GPIos that control the H bridge
	init_ADC(); //Initialize ADC

	__enable_interrupt();

	RxStruct UART_DATA; //Struct where we will save data received

	while(1)
	{
	    P3OUT |= BIT6; //Green LED ON to indicate that the system is waiting for a command
	    P3OUT &= ~BIT7; //Red Led OFF to indicate that the system is waiting for a command and no errors has been produced

	    UART_DATA = receive_DataUART_COM(5); //Wait for data from UART with a timeout of 10ms
	    if(UART_DATA.Packet[0] == 'A' || UART_DATA.Packet[0] == 'S' || UART_DATA.Packet[0] == 'C' || UART_DATA.Packet[0] == 'P'
	       || UART_DATA.Packet[0] == 'T') //Only if we receive permitted commands, we will execute the principal function
	    {
	       Analyze_received_data(UART_DATA.Packet);
	    }
        __delay_cycles(1600); //To introduce a little delay of 10ms between checking the port
	}
}
