Metamer Experiment GUI
A graphical user interface (GUI) for running behavioral and EEG-based metamer experiments with a Teensy 4.0 microcontroller.
Used by experimenter in Metamers experiments with SSVEP.

Overview:
This application provides experimenters with a streamlined interface to run metamer perception experiments involving controlled color mixing. It integrates hardware control, session management, and automated data logging into a single workflow.

The GUI enables:
- Serial communication with a Teensy 4.0 microcontroller (See repository TeensyMetamersFirmware)
- Setup of subject sessions (ID + data directory)
- Execution of behavioral and EEG experiment protocols
- Automatic organization and storage of experimental data

Features
🔌 Serial Communication
Connects to Teensy 4.0 via configurable serial port

🧑‍🔬 Session Management
Assign subject IDs
Select and manage data directories

🧪 Experiment Modes
2 Behavioral (manual adjustment via knobs)
2 EEG (trigger/control only)

🎨 Color Paradigms
Red–Green: Constant yellow reference; red and green vary
Blue–Green: Constant cyan reference; blue and green vary

💾 Automated Data Handling
Generates .txt files for behavioral experiments
Automatically increments run numbers
Organizes data per subject/session

🧪 Experiment Types
Behavioral Experiments
Participants adjust input knobs to achieve a perceptual metamer
Responses are recorded and saved as .txt files
Each run is automatically indexed and stored
EEG Experiments
GUI initializes and controls experiment timing via Teensy
No behavioral data is saved (EEG data acquisition handled externally)

🎨 Color Conditions
Condition	Constant Light	Adjustable Lights
Red–Green	Yellow	Red, Green
Blue–Green	Cyan	Blue, Green

Data Output
Behavioral experiments generate .txt files
Files are saved in the selected directory
Run numbers are automatically incremented
