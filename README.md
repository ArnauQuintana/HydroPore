# HydroPore
This project was realized in 2022 as my BSc thesis. 
The objective was to design and implement an embedded system for the control of a setup which looked like the following image:  

![image](https://github.com/user-attachments/assets/7f72113d-a750-4aa0-9126-0ac1bf6549a1)  

Where (1) are LVDT sensors for displacement measure, (2) a load cell for measuring pressure pushed by an actuator, (3) water pressure sensor and last but not least, 4 cameras in each side of the experiment, like this: 

![image](https://github.com/user-attachments/assets/f5277b30-9eba-4a4a-9c70-0bbe22fae95f)


My mission was to control the actuator while getting data from all sensors. I designed and implemented the PCB with all conditioning and required circuits for this application.  
Here is a photo of the final solution:  

![image](https://github.com/user-attachments/assets/99faa0fd-3b0a-4028-aaa7-1a3ef6e5c4a5)

## 📁 Project 1 - [HydroPore_FW]

**Description:**  
Implementation of the bare-metal firmware in a MSP430 microcontroller from TI.

**Key Features:**
- Key feature 1
- Key feature 2
- Key feature 3

**Location:** `/Project1/`

---

## 📁 Project 2 - [HydroPore_Cameras]

**Description:**  
This project was intended to control 1 serial camera with Arduino. Its objective was to trigger the camera, read the photo information and save it in an SD card. 

**Key Features:**
- Key feature 1
- Key feature 2

**Location:** `/Project2/`

---

## 📁 Project 3 - [HydroPore_GUI]

**Description:**  
Simple Python GUI using 'tkinter' for sending commands and receiving data from sensors. 

**Key Features:**
- Serial communication with UART
- It generates a 'RESULTS.txt' file with data from sensors.

**Location:** `/Project3/`

---

## 📁 Project 4 - [PCB_Project_HydroPore]

**Description:**  
This is the PCB where all conditioning circuits and the microcontroller are allocated. 

**Key Features:**
- Supplied by 2 sources, one for the power part (actuator) and one for the conditioning of sensors data. 
- UART communication for exchanging data with GUI in Python. 
- Key feature 3

**Location:** `/PCB_Project_HydroPore/`

---
## 📌 Additional Notes

- It is important to consider that this was my first 'big' project. It was entirely realized in 6 months, the unexperience and short deadline made me do some parts not as optimal as possible or containing bugs and innacuraccies. 
- Each project includes its own detailed `README.md` file inside its folder.



---

## 📄 License

This repository is licensed under the [MIT License]. Please see the `LICENSE` file for more details.
