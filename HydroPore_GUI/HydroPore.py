import serial as serial
from tkinter import *
from time import sleep
from tkinter import messagebox
import sys
from datetime import datetime
import os

path = os.path.abspath(os.getcwd())

#Different ways the program can take

#Module = 1 -> Actuator
#Module = 2 -> Sensors
#Module = 3 -> Cameras

#Module_part = 1 and Module = 1 -> Forward Actuator
#Module_part = 2 and Module = 1 -> Back Actuator
#Module_part = 3 and Module = 1 -> +Speed Actuator
#Module_part = 4 and Module = 1 -> -Speed Actuator
#Module_part = 5 and Module = 1 -> Initial position Actuator

#Module_part = 1 and Module = 2 -> LVDT horitzontal
#Module_part = 2 and Module = 2 -> LVDT vertical
#Module_part = 3 and Module = 2 -> Load cell horitzontal
#Module_part = 4 and Module = 2 -> Load cell vertical
#Module_part = 5 and Module = 2 -> Water pressure sensor
#Module_part = 6 and Module = 2 -> Actuator Feedback

#Module_part = 1 and Module = 3 -> Camera 1
#Module_part = 2 and Module = 3 -> Camera 2
#Module_part = 3 and Module = 3 -> Camera 3
#Module_part = 4 and Module = 3 -> Camera 4

#Module_part = 1 and Module = 4 -> Complete Program

#Module_part = 1 and Module = 5 -> TEST


###########################################Actuator defs#######################################################################
def click_Actuator(): #When 'Actuator' Button is pressed
    Phrase.grid_forget() #To hide labels and  buttons
    Phrase2.grid_forget()    
    button_Actuator.grid_forget()
    button_Sensors.grid_forget()
    button_Cameras.grid_forget()
    button_Complete.grid_forget()
    button_TEST.grid_forget()
    Phrase4.grid_forget()
    button_sendcommandForward.grid_forget()
    button_sendcommandBack.grid_forget()
    button_sendcommandMoreSpeed.grid_forget()
    button_sendcommandLessSpeed.grid_forget()
    button_sendcommandInitial.grid_forget()
    button_MainMenu.grid_forget()
    button_returnActuator.grid_forget()
    COM_PORT.grid_forget()
    
    Phrase3.grid(column=22, row=19, sticky=(W,E))
    button_Forward.grid(column=22, row=20)
    button_Back.grid(column=22, row=21)
    button_SpeedMore.grid(column=22, row=22)
    button_SpeedLess.grid(column=22, row=23)
    button_Initial.grid(column=22, row=24)
    button_MainMenu.grid(column=26, row=22)

def click_Forward(): #When 'Forward' Button is pressed
    Phrase3.grid_forget()
    button_Forward.grid_forget()
    button_Back.grid_forget()
    button_SpeedMore.grid_forget()
    button_SpeedLess.grid_forget()
    button_Initial.grid_forget()
    button_MainMenu.grid_forget()

    click_UART(1,1)

def click_Back(): #When 'Back' Button is pressed
    Phrase3.grid_forget()
    button_Forward.grid_forget()
    button_Back.grid_forget()
    button_SpeedMore.grid_forget()
    button_SpeedLess.grid_forget()
    button_Initial.grid_forget()
    button_MainMenu.grid_forget()

    click_UART(1,2)
    
    
def click_SpeedMore(): #When 'Speed More' Button is pressed
    Phrase3.grid_forget()
    button_Forward.grid_forget()
    button_Back.grid_forget()
    button_SpeedMore.grid_forget()
    button_SpeedLess.grid_forget()
    button_Initial.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(1,3)
    
def click_SpeedLess(): #When 'Speed Less' Button is pressed
    Phrase3.grid_forget()
    button_Forward.grid_forget()
    button_Back.grid_forget()
    button_SpeedMore.grid_forget()
    button_SpeedLess.grid_forget()
    button_Initial.grid_forget()
    button_MainMenu.grid_forget()

    click_UART(1,4)
    
def click_Initial(): #When 'Initial' Button is pressed
    Phrase3.grid_forget()
    button_Forward.grid_forget()
    button_Back.grid_forget()
    button_SpeedMore.grid_forget()
    button_SpeedLess.grid_forget()
    button_Initial.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(1,5) 
    
################################################################################################################################
    
def click_Sensors():#When 'Sensors' Button is pressed
    Phrase.grid_forget() #To hide labels and  buttons
    Phrase2.grid_forget()    
    button_Actuator.grid_forget()
    button_Sensors.grid_forget()
    button_Cameras.grid_forget()
    button_Complete.grid_forget()
    button_TEST.grid_forget()
    Phrase4.grid_forget()
    button_sendcommandLVDT1.grid_forget()
    button_sendcommandLVDT2.grid_forget()
    button_sendcommandLoad1.grid_forget()
    button_sendcommandLoad2.grid_forget()
    button_sendcommandWater.grid_forget()
    button_sendcommandFeedback.grid_forget()
    button_MainMenu.grid_forget()
    button_returnSensors.grid_forget()
    COM_PORT.grid_forget()
    
    Phrase6.grid(column=22, row=19, sticky=(W,E))
    button_LVDTActuator.grid(column=22, row=20)
    button_LVDTVertical.grid(column=22, row=21)
    button_LoadActuator.grid(column=22, row=22)
    button_LoadWeight.grid(column=22, row=23)
    button_Water.grid(column=22, row=24)
    button_Feedback.grid(column=22, row=25)
    button_MainMenu.grid(column=26, row=22)
    
def click_LVDTActuator(): #When 'LVDT Actuator' Button is pressed
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(2,1)
    
def click_LVDTVertical():#When 'LVDT Vertical' Button is pressed
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(2,2)
    
def click_LoadActuator(): #When 'Load Actuator' Button is pressed
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_MainMenu.grid_forget()

    click_UART(2,3)

def click_LoadWeight(): #When 'Load Weight' Button is pressed
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(2,4)
    
def click_Water(): #When 'Water sensor' Button is pressed
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(2,5)
    
def click_Feedback(): #When 'Water sensor' Button is pressed
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_MainMenu.grid_forget()
    
    click_UART(2,6)
    
###############################################################################################################################
    
def click_Cameras(): #When 'Cameras' Button is pressed
    Phrase.grid_forget() #To hide labels and  buttons
    Phrase2.grid_forget()    
    button_Actuator.grid_forget()
    button_Sensors.grid_forget()
    button_Cameras.grid_forget()
    button_Complete.grid_forget()
    button_TEST.grid_forget()
    Phrase4.grid_forget()
    button_sendcommandCam1.grid_forget()
    button_sendcommandCam2.grid_forget()
    button_sendcommandCam3.grid_forget()
    button_sendcommandCam4.grid_forget()
    button_sendcommandAllCams.grid_forget()
    button_MainMenu.grid_forget()
    button_returnCameras.grid_forget()
    COM_PORT.grid_forget()
    
    Phrase7.grid(column=22, row=19, sticky=(W,E))
    button_Cam1.grid(column=22, row=20)
    button_Cam2.grid(column=22, row=21)
    button_Cam3.grid(column=22, row=22)
    button_Cam4.grid(column=22, row=23)
    button_AllCams.grid(column=22, row=24)
    button_MainMenu.grid(column=26, row=22)

def click_Cam1():#When 'Camera 1' Button is pressed
    Phrase7.grid_forget()
    button_Cam1.grid_forget()
    button_Cam2.grid_forget()
    button_Cam3.grid_forget()
    button_Cam4.grid_forget()
    button_AllCams.grid_forget()
    button_MainMenu.grid_forget()

    click_UART(3,1)
    
def click_Cam2():#When 'Camera 2' Button is pressed
    Phrase7.grid_forget()
    button_Cam1.grid_forget()
    button_Cam2.grid_forget()
    button_Cam3.grid_forget()
    button_Cam4.grid_forget()
    button_AllCams.grid_forget()
    button_MainMenu.grid_forget() 
    
    click_UART(3,2)
    
def click_Cam3():#When 'Camera 3' Button is pressed
    Phrase7.grid_forget()
    button_Cam1.grid_forget()
    button_Cam2.grid_forget()
    button_Cam3.grid_forget()
    button_Cam4.grid_forget()
    button_AllCams.grid_forget()
    button_MainMenu.grid_forget() 

    click_UART(3,3)
    
def click_Cam4(): #When 'Camera4' Button is pressed
    Phrase7.grid_forget()
    button_Cam1.grid_forget()
    button_Cam2.grid_forget()
    button_Cam3.grid_forget()
    button_Cam4.grid_forget()
    button_AllCams.grid_forget()
    button_MainMenu.grid_forget() 

    click_UART(3,4)
    
def click_AllCams(): #When 'Camera4' Button is pressed
    Phrase7.grid_forget()
    button_Cam1.grid_forget()
    button_Cam2.grid_forget()
    button_Cam3.grid_forget()
    button_Cam4.grid_forget()
    button_AllCams.grid_forget()
    button_MainMenu.grid_forget() 

    click_UART(3,5)
    
##############################################################################################################################

def click_Complete(): #When 'Complete Program' Button is pressed
    Phrase.grid_forget() #To hide labels and  buttons
    Phrase2.grid_forget()    
    button_Actuator.grid_forget()
    button_Sensors.grid_forget()
    button_Cameras.grid_forget()
    button_Complete.grid_forget()
    button_TEST.grid_forget()
    Phrase4.grid_forget()
    button_MainMenu.grid_forget()
    COM_PORT.grid_forget()
    
    click_UART(4,0)
    
##############################################################################################################################

def click_TEST(): #When 'TEST' Button is pressed
    Phrase.grid_forget() #To hide labels and  buttons
    Phrase2.grid_forget()    
    button_Actuator.grid_forget()
    button_Sensors.grid_forget()
    button_Cameras.grid_forget()
    button_Complete.grid_forget()
    button_TEST.grid_forget()
    Phrase4.grid_forget()
    button_MainMenu.grid_forget()
    COM_PORT.grid_forget()
    
    click_UART(5,0)
    
###############################################################################################################################

def click_UART(Module, Module_part): #This function will show 'Send Command' Button depending on what instruction been selected

    Phrase4.grid(column=2, row=2, sticky=(W,E))
    button_MainMenu.grid(column=10, row=4)
    if Module == 1:
        button_returnActuator.grid(column=10, row=3)
        if Module_part == 1:  
            button_sendcommandForward.grid(column=2, row=3)
        elif Module_part == 2:  
            button_sendcommandBack.grid(column=2, row=3)
        elif Module_part == 3:  
            button_sendcommandMoreSpeed.grid(column=2, row=3)
        elif Module_part == 4:  
            button_sendcommandLessSpeed.grid(column=2, row=3)
        elif Module_part == 5:  
            button_sendcommandInitial.grid(column=2, row=3)
    if Module == 2:
        button_returnSensors.grid(column=10, row=3)
        if Module_part == 1:  
            button_sendcommandLVDT1.grid(column=2, row=3)
        elif Module_part == 2:  
            button_sendcommandLVDT2.grid(column=2, row=3)
        elif Module_part == 3:  
            button_sendcommandLoad1.grid(column=2, row=3)
        elif Module_part == 4:  
            button_sendcommandLoad2.grid(column=2, row=3)
        elif Module_part == 5:  
            button_sendcommandWater.grid(column=2, row=3)
        elif Module_part == 6:  
            button_sendcommandFeedback.grid(column=2, row=3)
    if Module == 3:
        button_returnCameras.grid(column=10, row=3)
        if Module_part == 1:  
            button_sendcommandCam1.grid(column=2, row=3)
        elif Module_part == 2:  
            button_sendcommandCam2.grid(column=2, row=3)
        elif Module_part == 3:  
            button_sendcommandCam3.grid(column=2, row=3)
        elif Module_part == 4:  
            button_sendcommandCam4.grid(column=2, row=3)
        elif Module_part == 5:
            button_sendcommandAllCams.grid(column=2,row=3)
    if Module == 4:
        button_sendcommandComplete.grid(column=2, row=3)
    if Module == 5:
        button_sendcommandTest.grid(column=2, row=3)
        
#######################################
def click_sendCommandForward(): #When 'Send Command' Button is pressed and Forward is the instruction selected
    UART_PORT = getTextInput()
    try: 
        ser = serial.Serial(port =  'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('A'))
        sleep(1)
        ser.write(str.encode('F'))
        sleep(1)  
        ser.write(str.encode('0'))
        ser.close() 
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()        
    button_returnActuator.grid_forget() 
    button_sendcommandForward.grid_forget()
    
def click_sendCommandBack(): #When 'Send Command' Button is pressed and Back is the instruction selected
    UART_PORT = getTextInput()
    try: 
        ser = serial.Serial(port =  'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('A'))
        sleep(1)
        ser.write(str.encode('B'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
    
    Phrase4.grid_forget()
    button_returnActuator.grid_forget() 
    button_sendcommandBack.grid_forget()
    
def click_sendCommandMoreSpeed(): #When 'Send Command' Button is pressed and +Speed is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('A'))
        sleep(1)
        ser.write(str.encode('+'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnActuator.grid_forget() 
    button_sendcommandMoreSpeed.grid_forget()
    
def click_sendCommandLessSpeed(): #When 'Send Command' Button is pressed and -Speed is the instruction selected
    UART_PORT = getTextInput()
    try: 
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('A'))
        sleep(1)
        ser.write(str.encode('-'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget() 
    button_returnActuator.grid_forget() 
    button_sendcommandLessSpeed.grid_forget()
    
def click_sendCommandInitial(): #When 'Send Command' Button is pressed and Initial is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('A'))
        sleep(1)
        ser.write(str.encode('R'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget() 
    button_returnActuator.grid_forget() 
    button_sendcommandInitial.grid_forget()
######################################
       
def click_sendCommandLVDT1(): #When 'Send Command' Button is pressed and LVDT1 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port ='COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('S'))
        sleep(0.5)
        ser.write(str.encode('H'))
        sleep(1)
        ser.write(str.encode('0'))
        sleep(1)
        
        try:
            while ser.in_waiting:
                
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
                
                a = open(f'{path}/RESULTS.txt', 'a')
                
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LVDT((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;LVDT_Actuator;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";mm")
                
                a.close()
                
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()   
    button_returnSensors.grid_forget() 
    button_sendcommandLVDT1.grid_forget()    
    
def click_sendCommandLVDT2(): #When 'Send Command' Button is pressed and LVDT2 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('S'))
        sleep(0.5)
        ser.write(str.encode('V'))
        sleep(1)
        ser.write(str.encode('0'))
        sleep(1)
        
        try:
            while ser.in_waiting:
              
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
        
                a = open(f'{path}/RESULTS.txt', 'a')
                i = 0
                while i < 5:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_LVDT((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;LVDT_Vertical;Time;")
                    a.write(current_time)
                    a.write(";Data;")
                    a.write(Converted_Data)   
                    a.write(";mm")
                    i = i + 1
                    
                a.close()   
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()    
    button_returnSensors.grid_forget() 
    button_sendcommandLVDT2.grid_forget()

#This function will analize received data and return a position of the LVDT2
def ConvertedData_LVDT(Data):
    Voltage = 0.0
    Result = 0.0
    
    Voltage = (Data * 3.3) / 4096
    Result = (Voltage * 1) / 0.215   #Every 1mm the LVDT output changes 0.220V
    
    return str(float(round(Result,3)))  
    
def click_sendCommandLoad1(): #When 'Send Command' Button is pressed and Load1 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('S'))
        sleep(0.5)
        ser.write(str.encode('X'))
        sleep(1)
        ser.write(str.encode('0'))
        sleep(0.5)
        
        try:
            while ser.in_waiting:
                
                data = 0
                while(data != b'R'):
                    data = ser.read(1)
              
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
              
                a = open(f'{path}/RESULTS.txt', 'a')
                i = 0
                while i < 5:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_LoadCell((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;Load_Actuator;Time;")
                    a.write(current_time)
                    a.write(";Data;")
                    a.write(Converted_Data)
                    a.write(";N")
                    i = i + 1
                    
                a.close()   
            ser.close()       
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnSensors.grid_forget() 
    button_sendcommandLoad1.grid_forget()    
    
def click_sendCommandLoad2(): #When 'Send Command' Button is pressed and Load2 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('S'))
        sleep(0.5)
        ser.write(str.encode('Y'))
        sleep(1)
        ser.write(str.encode('0'))
        sleep(0.5)
        
        try:
            while ser.in_waiting:
                
                data = 0
                while(data != b'R'): #until we don't receive a correct character we keep listening the port
                    data = ser.read(1)
              
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
              
                a = open(f'{path}/RESULTS.txt', 'a')
                i = 0
                while i < 5:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_LoadCell((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;Load_DeathWeigth;Time;")
                    a.write(current_time)
                    a.write(";Data;")
                    a.write(Converted_Data)
                    a.write(";N")
                    i = i + 1
                    
                a.close()   
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()    
    button_returnSensors.grid_forget() 
    button_sendcommandLoad2.grid_forget()
    
def ConvertedData_LoadCell(Data):   
    Voltage = 0.0
    Result = 0.0
    
    Voltage = ((Data-8192) * 5) / 16384 #Data from ADC * ref supply voltage / resoltion bits of ADC
                                        #We need to substract 8192 cause at 0V, Load is returning 2.5V (see documentation for more info)
    Result = (Voltage * 200) / 1.6   #When the cell is measuring 200N, output will be 1.6V (10mV cell out * 160 of gain)
    
    return str(float(round(Result,3))) 
    
def click_sendCommandWater(): #When 'Send Command' Button is pressed and Water Pressure sensor is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('S'))
        sleep(0.5)
        ser.write(str.encode('W'))
        sleep(1)
        ser.write(str.encode('0'))
        sleep(1)
        
        try:
            while ser.in_waiting:
                
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
              
                a = open(f'{path}/RESULTS.txt', 'a')
                i = 0
                while i < 5:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_Water((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;Water;Time;")
                    a.write(current_time)
                    a.write(";Data;")
                    a.write(Converted_Data)
                    a.write(";bar")
                    i = i + 1              
                
                a.close()
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnSensors.grid_forget() 
    button_sendcommandWater.grid_forget()
    
def ConvertedData_Water(Data):   

    Voltage = 0.0
    Result = 0.0
    
    Voltage = (Data * 3.3) / 4096  #ADC measure * ADC suppky / resolution bits
    Result = (Voltage * 10) / 3.3  #If 3.3V is 10bar, x voltage is... 
    
    return str(float(round(Result,5)))   

def click_sendCommandFeedback(): #When 'Send Command' Button is pressed and Water Pressure sensor is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('S'))
        sleep(0.5)
        ser.write(str.encode('F'))
        sleep(1)
        ser.write(str.encode('0'))
        sleep(0.5)
        
        try:
            while ser.in_waiting:
                
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
              
                a = open(f'{path}/RESULTS.txt', 'a')
                i = 0
                while i < 5:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_Feedback((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;Feedback;Time;")
                    a.write(current_time)
                    a.write(";Data;")
                    a.write(Converted_Data)
                    a.write(";mm")
                    i = i + 1              
                
                a.close()
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnSensors.grid_forget() 
    button_sendcommandFeedback.grid_forget()
    
def ConvertedData_Feedback(Data):   

    Voltage = 0.0
    Result = 0.0
    
    Voltage = (Data * 3.3) / 4096 #Data from ADC * ADC supply voltage / bits of resolution
    Result = (Voltage * 50) / 3.3 #At 50mm, the feedback will return 3.3V
    
    return str(float(round(Result,3)))   
    
#####################################

def click_sendCommandCam1(): #When 'Send Command' Button is pressed and Cam1 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('C'))
        sleep(1)
        ser.write(str.encode('X'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
    
    Phrase4.grid_forget()   
    button_returnCameras.grid_forget() 
    button_sendcommandCam1.grid_forget()
    
def click_sendCommandCam2(): #When 'Send Command' Button is pressed and Cam2 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('C'))
        sleep(1)
        ser.write(str.encode('Y'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnCameras.grid_forget() 
    button_sendcommandCam2.grid_forget()
    
def click_sendCommandCam3(): #When 'Send Command' Button is pressed and Cam3 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('C'))
        sleep(1)
        ser.write(str.encode('Z'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
       
    Phrase4.grid_forget() 
    button_returnCameras.grid_forget() 
    button_sendcommandCam3.grid_forget()
    
def click_sendCommandCam4(): #When 'Send Command' Button is pressed and Cam4 is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('C'))
        sleep(1)
        ser.write(str.encode('T'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnCameras.grid_forget() 
    button_sendcommandCam4.grid_forget()
    
def click_sendCommandAllCams(): #When 'Send Command' Button is pressed and All Cams is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('C'))
        sleep(1)
        ser.write(str.encode('A'))
        sleep(1)
        ser.write(str.encode('0'))
        ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget()  
    button_returnCameras.grid_forget() 
    button_sendcommandAllCams.grid_forget()
    
#####################################

def click_sendCommandComplete(): #When 'Send Command' Button is pressed and Complete Program is the instruction selected
    UART_PORT = getTextInput()
    Timestamps = [0]*1024
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 10)
        ser.write(str.encode('P'))
        ser.write(str.encode('0'))
        sleep(1)
        
        try:
            while ser.in_waiting:
                
                data = 0
                x = 0                
                while(data != b'S'): #while we don't receive a 'stop' command
                    data = ser.read(1) #we keep listening to the port
                    if(data == b'T'): #When we receive the 'Trigger' Command, 
                        now = datetime.now() #Make a Timestamp
                        current_time = now.strftime("%H:%M:%S") 
                        Timestamps[x] = current_time[0:8] #Save the time in an array
                        x = x + 1 #sum 1 to the variable to save the next timestamp in the next position                    
                
                a = open(f'{path}/RESULTS.txt', 'a')               
                
                data = 0
                while(data != b'R'):
                    data = ser.read(1)

                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LVDT((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;LVDT_Actuator;Time;")
                a.write(Timestamps[0])
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";mm")
                
                i = 0
                while i < x:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_LVDT((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;LVDT_Vertical;Time;")
                    a.write(Timestamps[i])
                    a.write(";Data;")
                    a.write(Converted_Data)   
                    a.write(";mm")
                    i = i + 1
                    
                i = 0  
                while i < x:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_Water((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                    a.write("\nSensor;Water;Time;")
                    a.write(Timestamps[i])
                    a.write(";Data;")
                    a.write(Converted_Data)
                    a.write(";bar")
                    i = i + 1 
                    
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_Feedback((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;Feedback;Time;")
                a.write(Timestamps[0])
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";mm")
                
                i = 0
                while i < x:
                    data = ser.read(1)
                    data2 = ser.read(1)
                    Data = [data,data2]
                    Converted_Data = ConvertedData_LoadCell((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))       
                    a.write("\nSensor;Load_Actuator;Time;")
                    a.write(Timestamps[i])
                    a.write(";Data;")
                    a.write(Converted_Data)
                    a.write(";N")
                    i = i + 1

                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LoadCell((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;Load_DeathWeigth;Time;")
                a.write(Timestamps[0])
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";N")  
                    
                a.close()                
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget() 
    button_sendcommandComplete.grid_forget()

#####################################

def click_sendCommandTest():#When 'Send Command' Button is pressed and TEST is the instruction selected
    UART_PORT = getTextInput()
    try:
        ser = serial.Serial(port = 'COM' + str(UART_PORT),baudrate  = 9600, parity = serial.PARITY_NONE, stopbits = serial.STOPBITS_ONE, bytesize = serial.EIGHTBITS,timeout = 0)
        ser.write(str.encode('T'))
        ser.write(str.encode('0'))
        sleep(0.5)
        ser.write(str.encode('0'))
        sleep(0.5)
        
        try:
            while ser.in_waiting:
                
                data = 0
                while(data != b'R'):
                    data = ser.read(1)
                
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
              
                a = open(f'{path}/RESULTS.txt', 'a')

                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LVDT((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;LVDT_Actuator;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";mm")
                
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LVDT((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;LVDT_Vertical;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";mm")
                
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_Water((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))   
                a.write("\nSensor;Water;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";bar")
                
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_Feedback((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))
                a.write("\nSensor;Feedback;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";mm")
                
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LoadCell((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))   
                a.write("\nSensor;Load_Actuator;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";N")
                
                data = ser.read(1)
                data2 = ser.read(1)
                Data = [data,data2]
                Converted_Data = ConvertedData_LoadCell((int.from_bytes(b''.join(Data), byteorder='big', signed=False)))   
                a.write("\nSensor;Load_Death_Weight;Time;")
                a.write(current_time)
                a.write(";Data;")
                a.write(Converted_Data)
                a.write(";N")
            
                a.close()    
            ser.close()
        except:
            print('Transaction gone wrong, try again')
            ser.close()
        
        Phrase5.grid(column=0, row=4, sticky=(W,E))
    except:
        Phrase8.grid(column=2, row=6, sticky=(W,E))
        
    Phrase4.grid_forget() 
    button_sendcommandTest.grid_forget() 
    
############################################MAIN MENU#########################################################################

def click_MainMenu(): #When 'Main Menu' Button is pressed       
    button_Forward.grid_forget() #To hide labels and  buttons
    button_Back.grid_forget()    
    button_SpeedMore.grid_forget()
    button_SpeedLess.grid_forget()
    button_Initial.grid_forget()
    button_MainMenu.grid_forget()
    Phrase3.grid_forget()    
    Phrase4.grid_forget() 
    Phrase5.grid_forget()
    button_sendcommandForward.grid_forget()
    button_sendcommandBack.grid_forget()
    button_sendcommandMoreSpeed.grid_forget()
    button_sendcommandLessSpeed.grid_forget()
    button_sendcommandInitial.grid_forget()
    button_sendcommandLVDT1.grid_forget()
    button_sendcommandLVDT2.grid_forget()
    button_sendcommandLoad1.grid_forget()
    button_sendcommandLoad2.grid_forget()
    button_sendcommandWater.grid_forget()
    button_sendcommandFeedback.grid_forget()
    button_sendcommandCam1.grid_forget()
    button_sendcommandCam2.grid_forget()
    button_sendcommandCam3.grid_forget()
    button_sendcommandCam4.grid_forget()
    button_sendcommandAllCams.grid_forget()
    button_sendcommandComplete.grid_forget()
    button_returnActuator.grid_forget()
    Phrase6.grid_forget()
    button_LVDTActuator.grid_forget()
    button_LVDTVertical.grid_forget()
    button_LoadActuator.grid_forget()
    button_LoadWeight.grid_forget()
    button_Water.grid_forget()
    button_Feedback.grid_forget()
    button_returnSensors.grid_forget()
    Phrase7.grid_forget()
    button_Cam1.grid_forget()
    button_Cam2.grid_forget()
    button_Cam3.grid_forget()
    button_Cam4.grid_forget()
    button_AllCams.grid_forget()
    button_returnCameras.grid_forget()
    Phrase8.grid_forget()
    button_TEST.grid_forget()
    button_sendcommandTest.grid_forget()
    button_returnTEST.grid_forget()
    
    Phrase.grid(column=22, row=2, sticky=(W,E))
    Phrase2.grid(column=40, row=2, sticky=(W,E))
    
    button_Actuator.grid(column=22, row=20)
    button_Sensors.grid(column=22, row=24)
    button_Cameras.grid(column=22, row=28)
    button_Complete.grid(column=40, row=20)
    button_TEST.grid(column=40, row=24)
    COM_PORT.grid(column=40, row=39, sticky=(W,E))

###############################################################################################################################

#Window generation
app = Tk()
app.title("HydroPore")
app.geometry("400x250")

#Main window
vp = Frame(app)
vp.grid(column=0, row=0, padx=(50,50), pady=(50,50))
vp.columnconfigure(0, weight=1)
vp.rowconfigure(0, weight=1)

Phrase = Label(vp, text="Module to communicate with   or")
Phrase.grid(column=22, row=2, sticky=(W,E))

Phrase2 = Label(vp, text=" Execute entire program")
Phrase2.grid(column=40, row=2, sticky=(W,E))

#Widget generation for Actuator Part
button_Actuator = Button(vp, text="Actuator", command=click_Actuator)
button_Actuator.grid(column=22, row=20)
button_Forward = Button(vp, text="Forward 1mm", command=click_Forward)
button_Back = Button(vp, text="Back 1mm", command=click_Back)
button_SpeedMore = Button(vp, text="+Speed", command=click_SpeedMore)
button_SpeedLess = Button(vp, text="-Speed", command=click_SpeedLess)
button_Initial = Button(vp, text="Initial Position", command=click_Initial)

Phrase3 = Label(vp, text= "What do you want to do with the actuator?")

#Widgets generation for Sensors Part
button_Sensors = Button(vp, text="Sensors", command=click_Sensors)
button_Sensors.grid(column=22, row=24)
Phrase6 = Label(vp, text="From which sensor do you want to get data?")

button_LVDTActuator = Button(vp, text="Actuator LVDT", command=click_LVDTActuator)
button_LVDTVertical = Button(vp, text="Vertical LVDT", command=click_LVDTVertical)
button_LoadActuator = Button(vp, text="Load Cell Actuator", command=click_LoadActuator)
button_LoadWeight = Button(vp, text="Load Cell Weight", command=click_LoadWeight)
button_Water= Button(vp, text="Water Pressure", command=click_Water)
button_Feedback= Button(vp, text="Actuator Feedback", command=click_Feedback)

#Button generation for Cameras Part
button_Cameras = Button(vp, text="Cameras", command=click_Cameras)
button_Cameras.grid(column=22, row=25)

Phrase7 = Label(vp, text="From which camera do you want a photo?")
button_Cam1= Button(vp, text="Camera 1", command=click_Cam1)
button_Cam2 = Button(vp, text="Camera 2", command=click_Cam2)
button_Cam3 = Button(vp, text="Camera 3", command=click_Cam3)
button_Cam4 = Button(vp, text="Camera 4", command=click_Cam4)
button_AllCams = Button(vp, text="All Cameras", command=click_AllCams)

#Button generation for Complete Program
button_Complete = Button(vp, text="Complete Program", command=click_Complete)
button_Complete.grid(column=40, row=20)

#Button generation for Test Program
button_TEST = Button(vp, text="TEST", command=click_TEST)
button_TEST.grid(column=40, row=24)

####################################################################

#Button generation for Main Menu
button_MainMenu = Button(vp, text="Main Menu", command=click_MainMenu)

#Button generation for Com Port

options = ['1','2','3','4','5','6','7','8','9']
value_inside = StringVar(vp)
value_inside.set("Select a Port")
COM_PORT = OptionMenu(vp, value_inside,*options)
COM_PORT.grid(column=40, row=39, sticky=(W,E))

def getTextInput():
    try: 
        COMPORT = value_inside.get()
        COM_PORT.grid_forget()
        return str(COMPORT)
    except: 
        COM_PORT.grid_forget()
    

#Widget generation for UART part    
Phrase4 = Label(vp, text="Ready? Click the button")

button_sendcommandForward = Button(vp, text="Send Command", command=click_sendCommandForward)
button_sendcommandBack = Button(vp, text="Send Command", command=click_sendCommandBack)
button_sendcommandMoreSpeed = Button(vp, text="Send Command", command=click_sendCommandMoreSpeed)
button_sendcommandLessSpeed = Button(vp, text="Send Command", command=click_sendCommandLessSpeed)
button_sendcommandInitial = Button(vp, text="Send Command", command=click_sendCommandInitial)

button_sendcommandLVDT1 = Button(vp, text="Send Command", command=click_sendCommandLVDT1)
button_sendcommandLVDT2 = Button(vp, text="Send Command", command=click_sendCommandLVDT2)
button_sendcommandLoad1 = Button(vp, text="Send Command", command=click_sendCommandLoad1)
button_sendcommandLoad2 = Button(vp, text="Send Command", command=click_sendCommandLoad2)
button_sendcommandWater = Button(vp, text="Send Command", command=click_sendCommandWater)
button_sendcommandFeedback = Button(vp, text="Send Command", command=click_sendCommandFeedback)

button_sendcommandCam1 = Button(vp, text="Send Command", command=click_sendCommandCam1)
button_sendcommandCam2 = Button(vp, text="Send Command", command=click_sendCommandCam2)
button_sendcommandCam3 = Button(vp, text="Send Command", command=click_sendCommandCam3)
button_sendcommandCam4 = Button(vp, text="Send Command", command=click_sendCommandCam4)
button_sendcommandAllCams = Button(vp, text="Send Command", command=click_sendCommandAllCams)

button_sendcommandComplete = Button(vp, text="Send Command", command=click_sendCommandComplete)

button_sendcommandTest = Button(vp, text="Send Command", command=click_sendCommandTest)

Phrase8 = Label(vp, text="Port selected and port where you have \n connected the FTDI doesn't match. Check it. \n Then try to send a command again.")

Phrase5 = Label(vp, text="Command has been sent!")
button_returnActuator = Button(vp, text="Return", command=click_Actuator)
button_returnSensors = Button(vp, text="Return", command=click_Sensors)
button_returnCameras = Button(vp, text="Return", command=click_Cameras)
button_returnTEST = Button(vp, text="Return", command=click_TEST)

app.mainloop()