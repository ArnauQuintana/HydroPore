/*
 * COMs.c
 *
 *  Created on: 17 apr. 2022
 *      Author: Arnau Quintana
 *
 * This file contains the functions related with communications
 */

#include <msp430.h>
#include <stdint.h>
#include "COMs.h"
#include "DrivingH.h"

uint8_t Received_byte = 0;
uint8_t UART_Data_COM;
uint8_t Data_from_UART = 0;
/*We will use the same module as SPI or UART. In SPI mode we don't use interruptions yet we ensure correct operation
 * setting this variable to 1 when the module is in UART mode
 */
uint8_t SPI_Data[3]; //Data received via SPI
uint8_t Received_Data_SPI = 0; //Variable that will indicate how many bytes have we received

/*********************************TIMEOUT UART**************************************************/

unsigned long TimeOut_completed;

/*
 * Function that clears the counter register and activates interruptions of the timer
 */
void Enable_Interruption_Timer_Timeout()
{
    TA0CTL = TASSEL_2 | MC_0 | TACLR; //SMCLK (16MHz), Mode STOP, CLEAR
    TA0CCTL0 = CCIE; //enable interruptions
    TA0CCR0 = 1600; //16MHz/10000 -> interruption every 100us
}

/*
 * Function that stops counting
 */
void Reset_Timeout()
{
    TimeOut_completed = 0; //Reset the variable that we compares with desirable timeout
    TA0CTL ^= MC_1; //Stop the counter
}

/*
 *Function that will return 1 if a timeout is produced
 */
uint8_t TimeOut(uint32_t time_out)
{
    uint8_t Back;

    TA0CTL |= MC_1; //activamos el timer en modo UP
    Back = 0;
    while(TimeOut_completed < time_out) //mientras no se cumpla el timeout seguimos en el bucle
    {
     if (Received_byte == 1)break; //en caso de que se reciba un byte salimos del while
    }
    if(TimeOut_completed == time_out) Back = 1; //únicamente si se cumple el timeout debemos devolver un 1
    return Back;
}

/*
 * Function to deactivate interruptions of the timer
 */
void Disable_Interruption_Timer_Timeout()
{
    TA0CCTL0 &= ~CCIE; //desactivamos interrupciones
    TA0CTL ^= MC_1; //pasamos a modo STOP
}

/*
 * Timer interruption routine
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    TimeOut_completed++;
    TA0CCTL0 &= ~CCIFG;
}

/************************** UART communication with PC***************************************/

/*
 * This function initializes UART module for communication with the PC
 */
void init_UART_COM(void) //UART configuration: UCA0, 115200bps
{
    UCA3CTLW0 |= UCSWRST;
    UCA3CTLW0 = 0;
    UCA3CTLW0 |= UCSWRST; //Reset for the USCI to remain it 'disabled'
    UCA3CTLW0 |= UCSSEL__SMCLK;
                                // UCSYNC=0 Asynchronous mode
                                // UCMODEx=0 UART mode
                                // UCSPB=0 One stop bit
                                // UC7BIT=0 8 bit data
                                // UCMSB=0 LSB first
                                // UCPAR=x Parity bit not used
                                // UCPEN=0 Parity disables
                                // SMCLK (16MHz) as clock source BRCLK
    UCA3MCTLW =  0; //Pag 783 user's guide for 115200baudrate.
    UCA3MCTLW = UCOS16; // oversampling => bit 0 = UCOS16 = 1.
    UCA3BR0 = 0;
    UCA3BR0 = 104; // Prescaler de BRCLK.
    UCA3MCTLW |= (2 << 4);
    UCA3MCTLW |= (0xD6 << 8);
    //Configuration of UART pins
    P2SEL1 |= BIT1 | BIT0; // I/O function: P2.0 = UART_TX, P2.1 = UART_RX
    P2SEL0 &= ~(BIT1 | BIT0);
    //Reconfiguration of SPI pins to avoid using it when module in UART mode
    P4SEL0 &= ~BIT1;
    P1SEL0 &= ~BIT7;
    P4SEL1 &= ~BIT1;
    P1SEL1 &= ~BIT7;

    UCA3CTLW0 &= ~UCSWRST; //Enable serial communications
    UCA3IE = 0;
    UCA3IE |= UCRXIE; // Enable interruptions of the UART reception
    UCA3IFG &= ~UCRXIFG; // To ensure correct operation, clean interruption flags
    Data_from_UART = 1; //Indicate that when the program enter to the interrupt routine we are in UART mode
}

/*
 * Function to wait and process data received from the PC via UART
 */
RxStruct receive_DataUART_COM(uint32_t time_out)
{
    struct RxStruct Received_Data;
    uint8_t Rx_Timeout = 0;
    Received_byte = 0;
    Enable_Interruption_Timer_Timeout();
    for (Received_byte = 0; Received_byte < 1;Received_byte++) //While we don't receive any byte, remain in the function
    {
        Reset_Timeout();
        while(!Received_byte)
        {
        Rx_Timeout = TimeOut(time_out);
        if (Rx_Timeout)break;
        }
    }
    Received_Data.Packet[0] = UART_Data_COM;
    Received_Data.time_out = Rx_Timeout;
    Disable_Interruption_Timer_Timeout();
    return Received_Data;
}

/*
 * Function to send data to PC via UART
 */
void send_DATAUART_COM(uint8_t bParameterLength, const uint8_t *Parameters)
{
    unsigned char bCount;

    for(bCount = 0; bCount < bParameterLength; bCount++)
    {
        while(!(UCA3IFG & UCTXIFG)); //be sure that last byte has been sent
        UCA3TXBUF = Parameters[bCount]; //put in the buffer byte that we want to send
    }
    while(UCA3STATW == 0x01); //UCBUSY = bit 0 of UCAxSTATW. Check if the line is busy
}

 /**************************** SPI communication with ADCs for Load Cells***************************/

/*
 *This function initializes the SPI module that communicates with the external ADCs, connected to the load cells
 */
void init_SPI_LoadCells(void)
{
    P4DIR |= BIT6 | BIT5; //Bit 5 for the load cell 2 and bit 6 for the load cell 1 (pins as CS)
    P4OUT |= BIT6 | BIT5; //OUT = 1 to maintain ADCs in off state

    //From master we don't send anything but we need to trigger the clock, so we have to configure SIMO pins
    //P4SEL0 |= BIT2;
    //P4SEL1 &= ~BIT2;

    UCA3CTLW0 = 0;
    UCA3CTLW0 = UCSWRST;
    /*
     * Disable module while configuration
     */

    UCA3CTLW0 = UCCKPH | UCMSB | UCMST | UCSYNC | UCSSEL_2;
    /*
     * Data is captures on the first UCLK edge and changed on the following edge (UCCKPH = 1)
     * Polarity: Inactive state is low (UCCKPL = 0)
     * MSB first (UCMSB = 1)
     * 8bit data (UC7BIT = 0)
     * Master mode (UCMST = 1)
     * 3-pin SPI (UCMODEx = 0)
     * Synchronous mode (UCSYN = 1)
     * SMCLK used in master mode (UCSSEL = 2)
     * STE pin is used to prevent conflicts with others masters (we don't use this pin) (UCSTEM = 0)
     * Software reset disabled (UCSWRST = 0)
     */
    UCA3BRW = 160; //SMCLK = 16MHz. SPI clock = SMCLK / 16.
    /*
     * Clock prescaler -> 16MHz / 4 = 100kHz.
     */

    //Configuration of SPI pins
    P4SEL0 |= BIT1;         /* P1.7, P4.1 for UCA3CLK, UCA3SOMI */
    P1SEL0 |= BIT7;
    P4SEL1 |= BIT1;
    P1SEL1 &= ~BIT7;
    //Reconfigure UART pins to avoid using it during SPI communication
    P2SEL1 &= ~(BIT1 | BIT0);
    P2SEL0 &= ~(BIT1 | BIT0);

    UCA3MCTLW= 0;
    UCA3CTLW0 &= ~UCSWRST;
    /*
     * Enable module after configuration
     */
    UCA3IE = 0; //Disable interrupts for SPI
    //UCA3IE |= UCRXIE; //Receive interrupt enabled
    Data_from_UART = 0; //Indicate that we are in SPI mode
}

/*
 * Function to claim and send data received from a LoadCell ADC.
 */
uint16_t SPI_Data_LoadCell(uint8_t CELL)
{
    uint16_t Data; //16 bits of data that we receive from the ADC

    init_SPI_LoadCells(); //We need to reconfigure the module

    SPI_Data[0] = 0; //Initialize the array that will save the data from external ADCs
    SPI_Data[1] = 0;
    SPI_Data[2] = 0;

    if(CELL == 1)
    {
        P4OUT &= ~BIT6; //Setting the CS low will activate the ADC
    }
    else
    {
        P4OUT &= ~BIT5; //Setting the CS low with activate the ADC
    }

    Received_Data_SPI = 0;

    while(Received_Data_SPI != 3) //While data received is below 3 bytes the program will stay in the loop
    {
        while (!(UCA3IFG & UCTXIFG)); // USCI_A0 TX buffer ready?
        UCA3TXBUF = 0xAA; //Send a random byte to TXBUF just to trigger the clock
        while(!(UCA3IFG & UCRXIFG)); //Byte received?
        SPI_Data[Received_Data_SPI] = UCA3RXBUF; //Save bytes received via SPI
        Received_Data_SPI++;
    }

    if(CELL == 1)
    {
        P4OUT |= BIT6; //We ensure that ADC will remain off until we want another lecture with the CS
    }
    else //Load Cell 2 (Death Weight)
    {
        P4OUT |= BIT5; //We ensure that ADC will remain off until we want another lecture
    }

    SPI_Data[2] = SPI_Data[2] & 0xFC; //Set to 0 last two bits.

    Data =  SPI_Data[1] << 8 | SPI_Data[2]; //Set bytes in correct position to be sent
    Data = Data >> 2;

    return Data;
}

/*
 * Interruption routine for the COMS module.
 * We use it only in UART mode
 */
#pragma vector = EUSCI_A3_VECTOR
__interrupt void EUSCIA3_IRQHandler(void)
{
    UCA3IE &= ~UCRXIE; //Disabled interruption on RX to avoid another one
    if(Data_from_UART) //If we are in UART mode of the module
    {
        UART_Data_COM = UCA3RXBUF; //Read UART buffer where the data is stored
        Received_byte += 1; //To indicate that we have received a byte
    }
    UCA3IE |= UCRXIE; //Enable interruptions on RX
    UCA3IFG &= ~UCRXIFG; //Erase flag interruption
}

/****************************** Communication with cameras********************************/

/*
 * This function will initialize GPIOs that will advice Arduinos to start or stop taking photos
 */
void init_GPIOs_Cams(void)
{
    P4DIR |= BIT3  + BIT7;
    P4DIR &= ~BIT4;
    P5DIR |= BIT0 + BIT2;
    P5DIR &= ~BIT1;
    //P5.2 will select cam 1 or 2, P4.7 will select cam 3 or 4.
    //P5.0 = 1 will take a photo. P5.1 = 1 photo taken
    //P4.3 = 1 will take a photo, P4.4 = 1 photo taken.

    P4REN |= BIT4; //Enable pullup/down
    P4OUT &= ~BIT4;
    P5REN |= BIT1; //Pulldown selected
    P5OUT &= ~BIT1;

    P4OUT &= 0x77; //=0 to initialize
    P5OUT &= 0xFA;

    //LEDs cameras
    P6DIR |= BIT4 + BIT5 + BIT0; //4 -> CAM2, 5 -> CAM1, 0 -> CAM3
    P5DIR |= BIT7; //CAM4
    P6OUT &= 0xCE; //LEDs OFF at init
    P5OUT &= 0x7F;

    Trigger_PhotoX = 0; //Initialize variables that save if we have sent a trigger or not to the cameras
    Trigger_PhotoY = 0;
    Trigger_PhotoZ = 0;
    Trigger_PhotoT = 0;
}

/*
 * Function to send a trigger to specified camera
 */
void Take_Photo(char Camera)
{
    uint8_t i;
    uint8_t OK = 0;

    switch(Camera)
    {
    case 'X': //CAM1
        P5OUT |= BIT2 + BIT0; //Select which Cam + take a photo

        __delay_cycles(1600000); //Little delay to wait for the cameras to assume the new state

        if ((P5IN & BIT1) != BIT1) OK = 1; //Check tthe port to know is the trigger has arrived correctly to the Arduinos
        else OK = 0;

        P5OUT &= ~BIT0; //To turn off the trigger

        if (OK == 1) //We haven't get a response from Arduinos, so we assume that the trigger hasn't arrived correctly
        {
            for(i = 0; i < 20; i++) //Led will blink 20 times advising that the trigger hasn't arrived correctly
            {
                P6OUT ^= BIT5;
                __delay_cycles(1600000);
            }
        }
        else
        {
            P6OUT |= BIT5;
            __delay_cycles(32000000); //2 seconds to advice that the trigger has been arrived correctly
            P6OUT &= ~BIT5;
            Trigger_PhotoX ^= 1;
        }
        P5OUT &= ~BIT2; //Turn off GPIO that selects the CAM
        break;
    case 'Y': //CAM2
        P5OUT |= BIT0; //Select which Cam + take a photo
        P5OUT &= ~BIT2;

        __delay_cycles(1600000);

        if ((P5IN & BIT1) != BIT1) OK = 1;
        else OK = 0;

        P5OUT &= ~BIT0; //Turn off the trigger

        if (OK == 1) //We haven't get a response from Arduinos
        {
            for(i = 0; i < 20; i++) //Led will blink 20 times advising that the trigger hasn't arrived correctly
            {
                P6OUT ^= BIT4;
                __delay_cycles(1600000);
            }
        }
        else
        {
            P6OUT |= BIT4;
            __delay_cycles(32000000); //2 seconds to advice that the trigger has been arrived correctly
            P6OUT &= ~BIT4;
            Trigger_PhotoY ^= 1;
        }
        break;
    case 'Z': //CAM3
        P4OUT |= BIT3 + BIT7; //Select which Cam + take a photo

        __delay_cycles(1600000);

        if ((P4IN & BIT4) != BIT4) OK = 1;
        else OK = 0;

        P4OUT &= ~BIT3; //Turn off the trigger

        if (OK == 1) //If a timeout has been produced, we haven't get a response from Arduinos
        {
            for(i = 0; i < 20; i++) //Led will blink 20 times advising that the trigger hasn't arrived correctly
            {
                P6OUT ^= BIT0;
                __delay_cycles(1600000);
            }
        }
        else
        {
            P6OUT |= BIT0;
            __delay_cycles(32000000); //2 seconds to advice that the trigger has been arrived correctly
            P6OUT &= ~BIT0;
            Trigger_PhotoZ ^= 1;
        }
        P4OUT &= ~BIT7; //Turn off the GPIO that selects the CAM
        break;
    case 'T': //CAM4
        P4OUT |= BIT3; //Select which Cam + take a photo
        P4OUT &= ~BIT7;

        __delay_cycles(1600000);

        if ((P4IN & BIT4) != BIT4) OK = 1;
        else OK = 0;

        P4OUT &= ~BIT3;

        if (OK == 1) //If we haven't get a response from Arduinos
        {
            for(i = 0; i < 20; i++) //Led will blink 20 times advising that the trigger hasn't arrived correctly
            {
                P5OUT ^= BIT7;
                __delay_cycles(1600000);
            }
        }
        else
        {
            P5OUT |= BIT7;
            __delay_cycles(32000000); //2 seconds to advice that the trigger has been arrived correctly
            P5OUT &= ~BIT7;
            Trigger_PhotoT ^= 1;
        }
        break;
    default: //We shouldn't enter here in any case, yet is good practice to add a default case
        break;
    }
}

/*
 * I2C extra communication (no device connected nowadays)
 */

//Here we can define functions that control a possible I2C device.
