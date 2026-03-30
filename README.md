# Metamer Experiment GUI and Teensy Firmware

![C#](https://img.shields.io/badge/C%23-Compatible-blue)

A graphical user interface (GUI) for running behavioral and EEG-based metamer experiments with a **Teensy 4.0 microcontroller**.  
This GUI was used in the **Metamers project** to log behavioral and EEG data for the study:  

📄 [Brain-Computer Interface (BCI)-Based Identification of Congenital Red–Green Color Vision Deficiencies](https://www.techrxiv.org/users/902197/articles/1362255-brain-computer-interface-bci-based-identification-of-congenital-red-green-color-vision-deficiencies)  
📄 DOI: 10.36227/techrxiv.176417950.04135341/v1

---

## Overview

<img width="446" height="464" alt="image" src="https://github.com/user-attachments/assets/3b860e91-0f7e-41d3-914a-6b39fec13786" />

This application provides experimenters with a streamlined interface for **metamer perception experiments**, integrating:

- Hardware control via serial communication with a Teensy 4.0  
- Session setup and subject management  
- Execution of behavioral and EEG protocols  
- Automated data logging and organization  

The GUI is designed to work **directly from the executable** in the `bin` folder—no compilation required.  

---

## GUI Features

### 🔌 Serial Communication
- Connects to Teensy 4.0 via a configurable serial port  
- Handles real-time communication for triggering experiment events  

<img width="284" height="362" alt="image" src="https://github.com/user-attachments/assets/1b8401cc-ac30-4668-8ede-634ee0d89bda" />

---

### 🧑‍🔬 Session Management
- Assign participant IDs  
- Select and manage data directories  

<img width="1090" height="325" alt="image" src="https://github.com/user-attachments/assets/57df42ab-3155-4f3a-a6b0-5ca96444dae3" />

---

### 🧪 Experiment Modes
- **Behavioral Experiments:** Manual adjustment via input knobs  
- **EEG Experiments:** GUI triggers experiment events only; EEG is logged externally  

---

### 🎨 Color Paradigms

| Condition   | Constant Light | Adjustable Lights |
|------------|----------------|------------------|
| Red–Green  | Yellow         | Red, Green       |
| Blue–Green | Cyan           | Blue, Green      |

<img width="1085" height="319" alt="image" src="https://github.com/user-attachments/assets/d05abbe1-3d92-405b-9f4d-4687e19ae468" />

---

### 💾 Automated Data Handling
- Generates `.txt` files for behavioral experiments  
- Automatically increments run numbers  
- Organizes data per participant/session  

Behavioral experiment filenames include a **short code** indicating the color paradigm:  
- `RG` = Red–Green with Yellow reference  
- `BG` = Blue–Green with Cyan reference  

Sample behavioral output:

TriggerCue TrialNumber Amber red   green Press
0          0           2400  1892  696   0


TriggerCue TrialNumber Cyan  blue  green Press
0          0           1400  892   394   0


---

### 🧪 Experiment Types

**Behavioral Experiments**
- Participants adjust knobs to achieve a perceptual metamer  
- Responses are recorded as `.txt` files  
- Each run is automatically indexed and stored  

**EEG Experiments**
- GUI triggers and controls experiment timing via Teensy  
- No behavioral data is saved (EEG acquisition handled externally)  

---

### 💾 Data Output
- All behavioral experiments generate `.txt` files  
- Files are saved in the selected directory  
- Run numbers are automatically incremented  

---

## ⚙️ How to Use

1. **Run the executable** in the `bin` folder  
2. Ensure the target machine has the correct **.NET Runtime** installed (or publish as self-contained):  
   [Microsoft .NET Runtime](https://dotnet.microsoft.com/en-us/learn/dotnet/what-is-dotnet)  
3. Connect Teensy 4.0 to the computer via USB  
4. Launch the GUI, select participant/session, and choose experiment type  
5. Start the experiment and monitor automated data logging  

> Note: No installation or compilation of the GUI source code is required. The `.exe` is ready to run.

---

### 🛠️ Built With
- **C#** (.NET 6 or later recommended)  
- Serial communication with Teensy 4.0  
- Windows-compatible GUI  

---

### 📄 Related Repository
- Teensy Firmware: [TeensyMetamersFirmware](link-to-firmware-repo)  
  Handles hardware-level LED control, trigger cues, and interaction with the GUI.

---

### ⚠️ Notes
- The GUI is designed for **research and experimental use only**  
- Behavioral data is logged locally; EEG acquisition requires additional hardware  
- Ensure the Teensy firmware matches the GUI version for proper communication  

---

### 📸 Screenshots
1. Serial connection status:

<img width="284" height="362" alt="image" src="https://github.com/user-attachments/assets/1b8401cc-ac30-4668-8ede-634ee0d89bda" />

2. Session management:

<img width="1090" height="325" alt="image" src="https://github.com/user-attachments/assets/57df42ab-3155-4f3a-a6b0-5ca96444dae3" />

3. Color paradigms:

<img width="1085" height="319" alt="image" src="https://github.com/user-attachments/assets/d05abbe1-3d92-405b-9f4d-4687e19ae468" />

4. Example behavioral output:

<img width="950" height="113" alt="image" src="https://github.com/user-attachments/assets/352edd3a-2072-4b39-988c-bcb7ca0e7769" />


## FIRMWARE Features
## Teensy 4.0 Firmware for Metamer Experiments

This firmware runs on the **Teensy 4.0 microcontroller** and serves as the **real-time control layer** for metamer perception experiments.  
It works hand-in-hand with the [Metamer Experiment GUI](#) to control LED-based visual stimuli, read participant inputs, and generate precise timing signals for EEG recordings.

---

## 🧠 Overview

The Teensy firmware communicates with the GUI via serial commands and ensures precise stimulus timing using hardware timers.  

**Core responsibilities include:**
- Generating LED stimulation signals via PWM  
- Reading participant input from knobs/sliders (analog inputs)  
- Executing behavioral and EEG experiment protocols  
- Sending synchronization signals to external EEG acquisition systems  

---

## ✨ Features

### ⏱️ Precise Timing Control
- Uses **hardware timers** to maintain accurate stimulus frequencies  
- Ensures low jitter for EEG-triggered events (e.g., SSVEP paradigms)  

### 💡 LED Control via PWM
- Drives LED channels (Red, Green, Blue) through external drivers  
- Supports **continuous** and **modulated light output**  

### 🎛️ Analog Input Handling
- Reads participant adjustments via ADC (knobs/sliders)  
- Enables **real-time control** during behavioral experiments  

### 🔌 Serial Communication
- Receives commands from the GUI  
- Sends experiment state and participant data when required  

### 📡 Digital Trigger Outputs
- Sends precise digital signals to EEG amplifiers for synchronization  
- Marks each stimulus event with a digital pulse for accurate EEG alignment  

---

## 🧪 Experiment Modes

### Behavioral (Manual) Experiments
- Participants adjust knobs/sliders to achieve a perceptual metamer  
- Analog inputs continuously read via ADC  
- LED intensities updated in **real-time via PWM**  
- Data sent to GUI for logging  

### EEG Experiments
- Predefined stimulus sequences executed automatically  
- Each stimulus associated with a **digital trigger output**  
- Timing controlled using **hardware timers** for precise frequency and phase  
- Digital triggers recorded by EEG amplifiers for synchronization  

---

## 🎨 Stimulus Control

- LED channels controlled via PWM signals  
- Compatible with external LED drivers  
- Supports multiple **color paradigms**:  
  - **Red–Green:** Yellow reference  
  - **Blue–Green:** Cyan reference  

---

## 🔧 Hardware Interfaces

| Interface | Description |
|-----------|-------------|
| Analog Inputs (ADC) | Knobs or sliders for participant control |
| PWM Outputs | LED intensity control via external drivers |
| Digital Outputs | Trigger signals for EEG acquisition |
| Serial (USB) | Communication with experiment GUI |

---

## ⚙️ Firmware Architecture

- **Main Loop:** Handles serial commands and experiment state  
- **Timer Interrupts:** Ensure precise stimulus timing  
- **ADC Module:** Reads analog inputs continuously or on demand  
- **PWM Control:** Updates LED intensities in real-time  
- **Digital I/O:** Sends trigger signals during EEG experiments  

---

## 🚀 Setup & Installation

**Requirements:**
- Teensy 4.0  
- Arduino IDE or PlatformIO  
- Teensyduino add-on (if using Arduino IDE)  

**Flashing the Firmware:**
1. Open the project in Arduino IDE or PlatformIO  
2. Select board: **Teensy 4.0**  
3. Compile and upload to the device  
4. Connect via USB to the host computer  

---

## 🔌 Serial Communication

The firmware listens for commands from the GUI to:
- Select experiment mode  
- Set stimulation parameters  
- Start/stop experiments  

> ⚠️ Ensure that the **serial baud rate matches the GUI configuration**.

---

## ⚠️ Timing Considerations

- Hardware timers ensure:
  - Stable stimulation frequencies  
  - Low jitter for EEG triggers  
- **Avoid adding blocking code** in the main loop or interrupts  

---

## 🧪 EEG Synchronization

- Digital output pins send trigger signals for each stimulus event  
- Triggers are recorded by EEG amplifiers as digital inputs  
- Guarantees precise alignment between **stimulus presentation** and **neural recordings**
