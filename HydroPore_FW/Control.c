/*
 * Control.c
 *
 *  Created on: 07 may 2022
 *      Author: Arnau Quintana
 *
 * This file contains the functions related with control of the system
 */

#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#include "COMs.h"
#include "DrivingH.h"
#include "ADC.h"
#include "Control.h"

struct System_Data Last_system_data; //Will save last adc results
struct System_Data Actual_system_data = {0,0,0,0,0}; //Will save actual adc results
uint8_t First_LVDT1_Data = 1;  //Variable that avoids movement of the actuator if the user hasn't take a LVDT1 measurement
uint8_t First_Feedback_Data = 1; //Variable that avoids movement of the actuator if the user hasn't take a feedback measurement

uint8_t Data_to_Send[2]; //Variable to save data that we will send to PC

uint16_t Feedback_Data[64];
uint16_t LVDT1_Data[64];
uint16_t Load_Data[64];
uint16_t LVDT2_Data[64];
uint16_t Water_Data[64]; //Array where data measures from the Pressure Sensor will be saved

uint8_t object_detected = 0; //If = 1, US sensor has detected an object
uint8_t i; //Variable for loops

/*
 * This function will execute a complete routine of the system
 */
void Complete_Program()
{
    uint8_t P = 0; //Variable to indicate the array position where the data will be saved
    uint8_t L = 0; //Variable to know how many data from the LVDT1 we have
    uint8_t Steady_state_Water = 0;//To count how many times the Water Pressure sensor measures the same value
    uint16_t Timeout = 0; //To avoid an infinite loop if the actuator or LVDT are not connected

    object_detected = US_value();//We take data from the US sensor to know if the actuator is in maximum position
    Actual_system_data.Feedback = measure_feedbackActuator(); //We take data from the feedback to ensure that we can go forward
    __delay_cycles(16000);
    Actual_system_data.LVDT1 = measure_LVDT1(); //We get data from the LVDT
    if(!object_detected && Actual_system_data.Feedback < 800 && Actual_system_data.LVDT1 < 3200 && !First_LVDT1_Data /*&& Actual_system_data.LVDT1 > 5*/) //Actuator isn't in maximum position, and we already taken the first
    {                                                                       //LVDT1 data, we can move 1mm forward
        Actuator_Move(FORWARD, 0); //Move the actuator forward
        while((Actual_system_data.LVDT1 < Last_system_data.LVDT1 + 260) && ((P3IN & BIT5) == BIT5)) //While LVDT data doesn't change, the actuator hasn't arrived yet.
        {                                                                                           //Or and OC situation has been produced
            Actual_system_data.LVDT1 = measure_LVDT1(); //We get data from the LVDT
            __delay_cycles(1600);//Little delay between ADC conversions (x[s]/MCLK)
            L++;
            if(Actual_system_data.LVDT1 <= Last_system_data.LVDT1 + 5) //If the measurement of the LVDT doesn't change
            {                                                          //We will suppose that the actuator or the LVDT
                Timeout++;                                             //are not connected
                if(Timeout == 35000) break;
            }
            if(L == 50 && P < 10) //Will take 10 measures from the CELL1, the Pressure Sensor and the LVDT2
            {
                Water_Data[P]= measure_WaterSensor();
                __delay_cycles(1600);//Little delay between ADC conversions (x[s]/MCLK)
                Load_Data[P] = SPI_Data_LoadCell(1); //Get data from the Load Cell 1
                __delay_cycles(1600);//Little delay between ADC conversions (x[s]/MCLK)
                LVDT2_Data[P] = measure_LVDT2(); //We get data from the LVDT2
                L = 0;
                P++;

                init_UART_COM(); //We need to reconfigure the module in order to use it in UART mode
                Data_to_Send[0] = 'T';
                send_DATAUART_COM(1, Data_to_Send); //Send a trigger to PC to make a timestamp
            }
        }
        Actuator_Move(STOP, 0); //Actuator has arrived to the new position

        Actual_system_data.Load2 = SPI_Data_LoadCell(2); //Get data from the Load Cell 2

        init_UART_COM();
        Data_to_Send[0] = 'S';
        send_DATAUART_COM(1, Data_to_Send); //Send a stop command to PC to let it know when it has to stop waiting for trigger

        if(Actual_system_data.LVDT1 < Last_system_data.LVDT1 + 260) //If we left the last while without a change on LVDT data
        {                                                           //OC has produced, we need to advice the user
            if(Timeout != 35000) //We stopped the movement because of an OC situation
            {
                for(i = 0; i < 20; i++)
                {
                    P3OUT ^= BIT6; //Two leds with toggle 20 times to advice the user there's some error in circuit
                    P3OUT ^= BIT7;
                    __delay_cycles(1600000);
                }
            }
            else //The measures are the same for a long time, the LVDT or the actuator aren't connected
            {
                for(i = 0; i < 20; i++)
                {
                    P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
                    P3OUT ^= BIT7;
                    __delay_cycles(1600000);
                }
            }
        }

         while(Steady_state_Water < 15) //We have to check when the system is in the steady state
         {
             Last_system_data.Water = Actual_system_data.Water;
             Actual_system_data.Water = measure_WaterSensor();
             if(Actual_system_data.Water > 5) //Measures must be greater than 5, if are not, we suppose that thse sensor is not connected
             {
                 if(Actual_system_data.Water < Last_system_data.Water + 50 && Actual_system_data.Water > Last_system_data.Water - 50)
                 {
                     Steady_state_Water += 1;
                 }
             __delay_cycles(16000); //1ms between measures
             }
             else{break;} //Sensor is not connected, we avoid an infinite loop just skipping this part
         }

         init_UART_COM();
         Data_to_Send[0] = 'R'; //We send 'Ready' command to let the PC know when we are going to start sending data
         send_DATAUART_COM(1, Data_to_Send);

         LVDT1_Data[0] = Actual_system_data.LVDT1;

         Data_to_Send[0] =  LVDT1_Data[0] >> 8;//Upper part of the 16 bit number
         Data_to_Send[1] =  LVDT1_Data[0] & 0xFF; //Lower part of the 16 bit number
         send_DATAUART_COM(2, Data_to_Send);

         for(i = 0; i < P; i++) //Send to the PC all data
         {
             Data_to_Send[0] =  LVDT2_Data[i] >> 8;//Upper part of the 16 bit number
             Data_to_Send[1] =  LVDT2_Data[i] & 0xFF; //Lower part of the 16 bit number
             send_DATAUART_COM(2, Data_to_Send);
         }

         for(i = 0; i < P; i++) //Send to the PC all data
         {
             Data_to_Send[0] =  Water_Data[i] >> 8;//Upper part of the 16 bit number
             Data_to_Send[1] =  Water_Data[i] & 0xFF; //Lower part of the 16 bit number
             send_DATAUART_COM(2, Data_to_Send);
         }

         Feedback_Data[0] = Actual_system_data.Feedback;

         Data_to_Send[0] =  Feedback_Data[0] >> 8;//Upper part of the 16 bit number
         Data_to_Send[1] =  Feedback_Data[0] & 0xFF; //Lower part of the 16 bit number
         send_DATAUART_COM(2, Data_to_Send);

         for(i = 0; i < P; i++) //Send to the PC all data
         {
             Data_to_Send[0] =  Load_Data[i] >> 8;//Upper part of the 16 bit number
             Data_to_Send[1] =  Load_Data[i] & 0xFF; //Lower part of the 16 bit number
             send_DATAUART_COM(2, Data_to_Send);
         }

         Load_Data[0] = Actual_system_data.Load2;

         Data_to_Send[0] =  Load_Data[0] >> 8;//Upper part of the 16 bit number
         Data_to_Send[1] =  Load_Data[0] & 0xFF; //Lower part of the 16 bit number
         send_DATAUART_COM(2, Data_to_Send);
    }
    else //The actuator is in maximum position or the user hasn't calibrate the system yet
    {
        for(i = 0; i < 20; i++)
        {
            P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
            P3OUT ^= BIT7;
            __delay_cycles(1600000);
        }
    }
}

/*
 * Function that will analyze data received from the PC and will generate a response
 */
void Analyze_received_data(const uint8_t *Parameters)
{
    uint16_t Timeout = 0; //To avoid an infinite loop if the actuator or LVDT are not connected
    struct RxStruct UART_Data;
    UART_Data.Packet[0] = 0;

    Last_system_data = Actual_system_data; //Save Last Data to compare with actual data

    switch(Parameters[0])
    {
    case 'A': //User want something from the actuator
        while(1)
        {
            UART_Data = receive_DataUART_COM(5); //Wait for data from UART with a timeout of 10ms
            if(UART_Data.Packet[0] != 0 && UART_Data.Packet[0] != 13 && UART_Data.Packet[0] != 'A')
            {
                P3OUT &= ~BIT6; //Green led will turn off
                P3OUT |= BIT7; //Red led will be ON to indicate that that system is working
                switch(UART_Data.Packet[0])
                {
                case 'F': //This command equals to 'Forward', the actuator have to go ahead
                    object_detected = US_value();//We take data firstly from the US sensor to know if the actuator is in maximum position
                    Actual_system_data.Feedback = measure_feedbackActuator(); //We take data from the feedback to ensure that we can go forward
                    __delay_cycles(16000);
                    Actual_system_data.LVDT1 = measure_LVDT1(); //We get data from the LVDT
                    if(!object_detected && Actual_system_data.Feedback < 810 && Actual_system_data.LVDT1 < 3200 && !First_LVDT1_Data /*&& Actual_system_data.LVDT1 > 5*/) //If the actuator isn't in maximum position
                    {                                                                                                                                                   //and we already have the first LVDT data, we can move
                        Actuator_Move(FORWARD, 0); //Go forward without modifying the speed
                        while((Actual_system_data.LVDT1 < Last_system_data.LVDT1 + 260) && ((P3IN & BIT5) == BIT5)) //While LVDT data doesn't change, the actuator hasn't arrived yet.
                        {                                                                                           //We check OC too, for protection.
                            Actual_system_data.LVDT1 = measure_LVDT1(); //We get data from the LVDT
                            __delay_cycles(1600);//Little delay of 0.5ms between ADC conversions (x[s]/MCLK)
                            if(Actual_system_data.LVDT1 <= Last_system_data.LVDT1 + 5) //If the measurement of the LVDT doesn't change
                            {                                                          //We will suppose that the actuator or the LVDT
                                Timeout++;                                             //are not connected
                                if(Timeout == 35000) break;
                            }
                        }
                        Actuator_Move(STOP, 0); //Actuator has arrived to the new position or an IC situation has been produced
                        if(Actual_system_data.LVDT1 < Last_system_data.LVDT1 + 260) //If we left the last while without a change on LVDT data OC has been produced
                        {                                                           //or the actuator or the LVDT are not connected, we need to advice the user
                            if(Timeout != 35000) //We stopped the movement because of an OC situation
                            {
                                for(i = 0; i < 20; i++)
                                {
                                    P3OUT ^= BIT6; //Two leds with toggle 20 times to advice the user there's some error in circuit
                                    P3OUT ^= BIT7;
                                    __delay_cycles(1600000);
                                }
                            }
                            else //The measures are the same for a long time, the LVDT or the actuator aren't connected
                            {
                                for(i = 0; i < 20; i++)
                                {
                                    P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
                                    P3OUT ^= BIT7;
                                    __delay_cycles(1600000);
                                }
                            }
                        }
                    }
                    else //Advice the user that the actuator can't move anymore
                    {
                        for(i = 0; i < 20; i++)
                        {
                            P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
                            P3OUT ^= BIT7;
                            __delay_cycles(1600000);
                        }
                    }
                    P3OUT &= ~BIT6; //At this point, whatever have happened, state of the leds will be green off and red ON
                    break;
                case 'B': //This command equals to 'Back', the actuator has to go back
                    Actual_system_data.Feedback = measure_feedbackActuator(); //We take data from the feedback to ensure that we can go forward
                    if(Actual_system_data.Feedback < 1200 && Actual_system_data.Feedback > 90 && !First_Feedback_Data) //If the actuator isn't in minimum position
                    {
                        Actuator_Move(BACK, 0); //Go back without modifying the speed
                        i = 0;
                        while(i < 10 && ((P3IN & BIT5) == BIT5)) //While the actuator isn't in minimum position and there's no OC
                        {
                            Actual_system_data.Feedback = measure_feedbackActuator();
                            __delay_cycles(16000);//Little delay of 1ms between ADC conversions (x[s]/MCLK)
                            if(Actual_system_data.Feedback < Last_system_data.Feedback - 85)
                            {
                                i++;
                            }
                        }
                        Actuator_Move(STOP, 0); //Actuator has arrived to the new position or an IC situation has been produced
                        First_LVDT1_Data = 1; //Because we moved the actuator without an LVDT1 measure, so we need a recalibration
                        if(Actual_system_data.Feedback > Last_system_data.Feedback - 85) //If we left the last while without a change on LVDT data
                        {                                                               //OC has produced, we need to advice the user
                            for(i = 0; i < 20; i++)
                            {
                                P3OUT ^= BIT6; //Two leds with toggle 20 times to advice the user there's some error in circuit
                                P3OUT ^= BIT7;
                                __delay_cycles(1600000);
                            }
                        }
                    }
                    else //Advice the user that the actuator can't move anymore or the user didn't calibrate the system yet
                    {
                        for(i = 0; i < 20; i++)
                        {
                            P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
                            P3OUT ^= BIT7;
                            __delay_cycles(1600000);
                        }
                    }
                    P3OUT &= ~BIT6; //At this point, whatever have happened, state of the leds will be green off and red ON
                    break;
                case 'R'://This command equals to 'Return', the actuator will return to position 0 -> X = 0
                    Actual_system_data.Feedback = measure_feedbackActuator(); //First, we check the position of the actuator
                    if(Actual_system_data.Feedback < 1200 && Actual_system_data.Feedback > 10 && !First_Feedback_Data) // If the actuator isn't in minimum position we can move it
                    {
                        Actuator_Move(BACK, 0); //Go back without modifying the speed
                        i = 0;
                        while(i < 10 && ((P3IN & BIT5) == BIT5)) //While the actuator isn't in minimum position and there's no OC
                        {
                            Actual_system_data.Feedback = measure_feedbackActuator();
                            __delay_cycles(16000);//Little delay of 1ms between ADC conversions (x[s]/MCLK)
                            if(Actual_system_data.Feedback < 10)
                            {
                                i++;
                            }
                        }
                        Actuator_Move(STOP, 0); //Actuator has arrived to the new position or OC situation has been produced
                        First_LVDT1_Data = 1; //Because we moved the actuator without an LVDT1 measure, so we need a recalibration
                        if(Actual_system_data.Feedback > 20) //If an OC situation has been produced, we have to advice the user
                        {
                            P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
                            P3OUT ^= BIT7;
                            __delay_cycles(1600000);
                        }
                    }
                    else //Actuator can't return to initial position because is already there or the user didn't calibrate the system yet
                    {
                        for(i = 0; i < 20; i++)
                        {
                            P3OUT |= BIT6; //Green Led ON and Red blinking to indicate that actuator won't move
                            P3OUT ^= BIT7;
                            __delay_cycles(1600000);
                        }
                    }
                    P3OUT &= ~BIT6; //At this point, whatever have happened, state of the leds will be green off and red ON
                    break;
                case '+': //This command equals to 'Fast', the actuator has to increase speed
                    Actuator_Move(STOP, FAST);
                    break;
                case '-': //This command equals to 'Slow', the actuator has to decrease speed
                    Actuator_Move(STOP, SLOW);
                    break;
                default: //The user has sent another command, so it will be interpreted as an error
                    for(i = 0; i < 20; i++)
                    {
                        P3OUT ^= BIT7; //Blink red led to indicate that command is incorrect
                        __delay_cycles(1600000);
                    }
                    break;
                }
                break; //Will stop the infinity while loop generated by while(1)
            }
            P3OUT |= BIT7; //Red LED will be ON with green to indicate that the system is waiting for another command
            __delay_cycles(160);
        }
        break;
    case 'S': //User want something about the sensors
        while(1)
        {
            UART_Data = receive_DataUART_COM(5); //Wait for data from UART with a timeout of 5ms
            if(UART_Data.Packet[0] != 0 && UART_Data.Packet[0] != 13 & UART_Data.Packet[0] != 'S')
            {
                P3OUT &= ~BIT6; //Turn off green LED
                P3OUT |= BIT7; //Red led will be ON to indicate that that system is working
                switch(UART_Data.Packet[0])
                {
                case 'V': //User want information of the vertical LVDT
                    for(i = 0; i < 5; i++)
                    {
                        LVDT2_Data[i] = measure_LVDT2();
                        __delay_cycles(16000); //Delay of 2ms between measures
                    }
                    for(i = 0; i < 5; i++) //Send to the PC all data
                    {
                        Data_to_Send[0] =  LVDT2_Data[i] >> 8;//Upper part of the 16 bit number
                        Data_to_Send[1] =  LVDT2_Data[i] & 0xFF; //Lower part of the 16 bit number
                        send_DATAUART_COM(2, Data_to_Send);
                    }
                    break;
                case 'H': //User want information of the horizontal LVDT
                    for(i = 0; i < 5; i++)
                    {
                        LVDT1_Data[i] = measure_LVDT1();
                        __delay_cycles(16000);
                    }
                    for(i = 0; i < 5; i++) //Send to the PC all data
                    {
                        Data_to_Send[0] =  LVDT1_Data[i] >> 8;//Upper part of the 16 bit number
                        Data_to_Send[1] =  LVDT1_Data[i] & 0xFF; //Lower part of the 16 bit number
                        send_DATAUART_COM(2, Data_to_Send);
                    }
                    Actual_system_data.LVDT1 = LVDT1_Data[4]; //Save last data taken as actual state
                    if(First_LVDT1_Data == 1)
                    {
                        First_LVDT1_Data = 0;
                    }
                    break;
                case 'W': //User want information of the water pressure sensor
                    for(i = 0; i < 5; i++)
                    {

                        Water_Data[i] = measure_WaterSensor();
                        __delay_cycles(16000);
                    }
                    for(i = 0; i < 5; i++) //Send to the PC all data
                    {
                        Data_to_Send[0] =  Water_Data[i] >> 8;//Upper part of the 16 bit number
                        Data_to_Send[1] =  Water_Data[i] & 0xFF; //Lower part of the 16 bit number
                        send_DATAUART_COM(2, Data_to_Send);
                    }
                    Actual_system_data.Water = Water_Data[4];
                    break;
                case 'X': //User want information of the load cell measuring the actuator
                    for(i = 0; i < 5; i++)
                    {
                        Load_Data[i] = SPI_Data_LoadCell(1);
                        __delay_cycles(160000);
                    }
                    init_UART_COM(); //We need to reconfigure the module in order to use it in UART mode
                    Data_to_Send[0] = 'R'; //When we reconfigure the module, the PC can interpret the start as a word so we
                    send_DATAUART_COM(1, Data_to_Send); //send an 'R' to let the PC know when we are going to start sending data
                    for(i = 0; i < 5; i++) //Send to the PC all data
                        {
                            Data_to_Send[0] =  Load_Data[i] >> 8;//Upper part of the 16 bit number
                            Data_to_Send[1] =  Load_Data[i] & 0xFF; //Lower part of the 16 bit number
                            send_DATAUART_COM(2, Data_to_Send);
                        }
                    Actual_system_data.Load1 = Load_Data[4];
                    break;
                case 'Y': //User want information of the load cell measuring the death weight
                    for(i = 0; i < 5; i++)
                    {
                        Load_Data[i] = SPI_Data_LoadCell(2);
                        __delay_cycles(160000);
                    }
                    init_UART_COM(); //We need to reconfigure the module in order to use it in UART mode
                    Data_to_Send[0] = 'R';
                    send_DATAUART_COM(1, Data_to_Send);
                    for(i = 0; i < 5; i++) //Send to the PC all data
                        {
                            Data_to_Send[0] =  Load_Data[i] >> 8;//Upper part of the 16 bit number
                            Data_to_Send[1] =  Load_Data[i] & 0xFF; //Lower part of the 16 bit number
                            send_DATAUART_COM(2, Data_to_Send);
                        }
                    Actual_system_data.Load2 = Load_Data[4];
                    break;
                case 'F': //User want information of actuator feedback
                    for(i = 0; i < 5; i++)
                    {
                        Feedback_Data[i] = measure_feedbackActuator();
                        __delay_cycles(160000);
                    }
                    for(i = 0; i < 5; i++) //Send to the PC all data
                    {
                        Data_to_Send[0] =  Feedback_Data[i] >> 8;//Upper part of the 16 bit number
                        Data_to_Send[1] =  Feedback_Data[i] & 0xFF; //Lower part of the 16 bit number
                        send_DATAUART_COM(2, Data_to_Send);
                    }
                    Actual_system_data.Feedback = Feedback_Data[4];
                    if(First_Feedback_Data == 1)
                    {
                        First_Feedback_Data = 0;
                    }
                    break;
                default: //User has sent an unknown command and it will be interpreted as an error
                    for(i = 0; i < 20; i++)
                    {
                        P3OUT ^= BIT7;
                        __delay_cycles(1600000);
                    }
                    P3OUT |= BIT6; //Turn on green led to demand another time a command
                    break;
                }
                break; //To exit the infinite loop
            }
            P3OUT |= BIT7; //Red LED will be ON with green to indicate that the system is waiting for another command
            __delay_cycles(160);
        }
        break;
    case 'C': //User want something about cameras
        while(1)
        {
            UART_Data = receive_DataUART_COM(5); //Wait for data from UART with a timeout of 10ms
            if(UART_Data.Packet[0] != 0 && UART_Data.Packet[0] != 13 && UART_Data.Packet[0] != 'C')
            {
                P3OUT &= ~BIT6; //Green led will turn off
                P3OUT |= BIT7; //Red led will be ON to indicate that that system is working
                switch(UART_Data.Packet[0])
                {
                case 'X': //User want to send a trigger to camera X
                    Take_Photo('X');
                    break;
                case 'Y': //User want to send a trigger to camera Y
                    Take_Photo('Y');
                    break;
                case 'Z': //User want to send a trigger to camera Z
                    Take_Photo('Z');
                    break;
                case 'T': //User want to send a trigger to camera T
                    Take_Photo('T');
                    break;
                case 'A'://User want to send a trigger to all the cameras
                    if(Trigger_PhotoX == Trigger_PhotoY == Trigger_PhotoZ == Trigger_PhotoT) //All cameras are in the same state
                    {
                        Take_Photo('X');
                        Take_Photo('Y');
                        Take_Photo('Z');
                        Take_Photo('T');
                    }
                    else //Not all the cameras are triggered, so we can't send the general trigger
                    {
                        for(i = 0; i < 20; i++)
                        {
                            P3OUT ^= BIT7;
                            __delay_cycles(1600000);
                        }
                    }
                    break;
                default: //User has sent an unknown command and it will be interpreted as an error
                    for(i = 0; i < 20; i++)
                    {
                        P3OUT ^= BIT7;
                        __delay_cycles(1600000);
                    }
                    P3OUT |= BIT6; //Turn on green LED to indicate that the system is waiting for another command
                    break;
                }
                break;
            }
            P3OUT |= BIT7; //Green LED will be ON with red led to indicate that the system is waiting for another command
            __delay_cycles(160);
        }
        break;
    case 'P': //User want an execution of the entire program (actuator, photos and sensors data)
        P3OUT &= ~BIT6; //Green led will turn off
        P3OUT |= BIT7; //Red led will be ON to indicate that that system is working
        Complete_Program(); //Execute a complete program
        break;
    case 'T': //User want a measure of all the sensors
        P3OUT &= ~BIT6; //Green led will turn off
        P3OUT |= BIT7; //Red led will be ON to indicate that that system is working

        LVDT1_Data[0] = measure_LVDT1(); //We get data from the LVDT
        __delay_cycles(160000);//Little delay of 10ms between ADC conversions (x[s]/MCLK)
        LVDT2_Data[0] = measure_LVDT2(); //We get data from the LVDT2
        __delay_cycles(160000);//Little delay of 10ms between ADC conversions (x[s]/MCLK)
        Load_Data[0] = SPI_Data_LoadCell(1); //Get data from the Load Cell 1
        __delay_cycles(160000);//Little delay of 10ms between ADC conversions (x[s]/MCLK)
        Load_Data[1] = SPI_Data_LoadCell(2); //Get data from the Load Cell 2
        __delay_cycles(160000);//Little delay of 10ms between ADC conversions (x[s]/MCLK)
        Water_Data[0] = measure_WaterSensor();
        __delay_cycles(160000);//Little delay of 10ms between ADC conversions (x[s]/MCLK)
        Feedback_Data[0] = measure_feedbackActuator();

        init_UART_COM(); //We need to reconfigure the module in order to use it in UART mode
        Data_to_Send[0] = 'R';
        send_DATAUART_COM(1, Data_to_Send);

        Data_to_Send[0] =  LVDT1_Data[0] >> 8;//Upper part of the 16 bit number
        Data_to_Send[1] =  LVDT1_Data[0] & 0xFF; //Lower part of the 16 bit number
        send_DATAUART_COM(2, Data_to_Send);

        Data_to_Send[0] =  LVDT2_Data[0] >> 8;//Upper part of the 16 bit number
        Data_to_Send[1] =  LVDT2_Data[0] & 0xFF; //Lower part of the 16 bit number
        send_DATAUART_COM(2, Data_to_Send);

        Data_to_Send[0] =  Water_Data[0] >> 8;//Upper part of the 16 bit number
        Data_to_Send[1] =  Water_Data[0] & 0xFF; //Lower part of the 16 bit number
        send_DATAUART_COM(2, Data_to_Send);

        __delay_cycles(16000); //Little delay to avoid strange behaviour of the clock

        Data_to_Send[0] =  Feedback_Data[0] >> 8;//Upper part of the 16 bit number
        Data_to_Send[1] =  Feedback_Data[0] & 0xFF; //Lower part of the 16 bit number
        send_DATAUART_COM(2, Data_to_Send);

        Data_to_Send[0] =  Load_Data[0] >> 8;//Upper part of the 16 bit number
        Data_to_Send[1] =  Load_Data[0] & 0xFF; //Lower part of the 16 bit number
        send_DATAUART_COM(2, Data_to_Send);

        Data_to_Send[0] =  Load_Data[1] >> 8;//Upper part of the 16 bit number
        Data_to_Send[1] =  Load_Data[1] & 0xFF; //Lower part of the 16 bit number
        send_DATAUART_COM(2, Data_to_Send);

        Actual_system_data.LVDT1 = LVDT1_Data[0];
        Actual_system_data.Load1 = Load_Data[0];
        Actual_system_data.Load2 = Load_Data[1];
        Actual_system_data.Water = Water_Data[0];
        Actual_system_data.Feedback = Feedback_Data[0];

        if(First_LVDT1_Data == 1 || First_Feedback_Data == 1) //Test command let the user calibrate the system
        {
            First_Feedback_Data = 0;
            First_LVDT1_Data = 0;
        }
        break;
    default: //User has sent an unknown command, it will be interpreted as an error
        P3OUT &= ~BIT6; //Turn off green LED
        for(i = 0; i < 20; i++)
        {
            P3OUT ^= BIT7; //Blink red LED to indicate that command is incorrect
            __delay_cycles(1600000);
        }
        break;
    }
}
