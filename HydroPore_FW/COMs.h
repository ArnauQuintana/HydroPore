/*
 * COMs.h
 *
 *  Created on: 17 apr. 2022
 *      Author: Arnau Quintana
 */


#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

#ifndef COMs_H_
#define COMs_H_

#define YES 1
#define NO 0
#define RX_BUFFER_SIZE 512 // buffer UART. Ojo: puede superar los 255 caracteres

uint8_t Trigger_PhotoX ; //Variable to know if we triggered the cameras to start taking photos
uint8_t Trigger_PhotoY; //Variable to know if we triggered the cameras to start taking photos
uint8_t Trigger_PhotoZ; //Variable to know if we triggered the cameras to start taking photos
uint8_t Trigger_PhotoT; //Variable to know if we triggered the cameras to start taking photos
/*Trigger for every camera, if one of them fails, the others can be working correctly, we will change
the value in the COM.c library,  not in control, using the XOR operator.*/

#define LVDT_Actuator 250
#define LVDT_Structure 251
#define CELL_Actuator 252
#define CELL_Structure 253
#define WATER_IN 254

typedef struct RxStruct
{
    uint8_t Packet[RX_BUFFER_SIZE]; //To store received byte
    uint8_t time_out;   //Indicate if a timeout has been done during the reception
}RxStruct;

void init_UART_COM(void);
RxStruct receive_DataUART_COM(uint32_t time_out);
void send_DATAUART_COM(uint8_t bParameterLength, const uint8_t *Parameters);
void init_SPI_LoadCells(void);
uint16_t SPI_Data_LoadCell(uint8_t CELL);
void init_GPIOs_Cams(void);
void Take_Photo(char Camera);

#endif
