#include <Bounce2.h>
#include <TeensyThreads.h>
#include <IntervalTimer.h>

IntervalTimer timerL1;
IntervalTimer timerL2;
IntervalTimer timerL3;
IntervalTimer timerL4;

// LED and pin definitions
#define BUTTON_PIN  12 //DI1

#define AMBER       0  //DR1. + LED6, - LED9. Change for Cyan
#define RED         1  //DR2. + LED1, -LED14. Change for Blue
#define CYAN        4  //DR6. + LED3, -LED12.
#define BLUE        2  //DR5. + LED3, -LED12.
#define GREEN       3  //DR4. + LED3, -LED12.
#define trigger     6  //DO1

#define AIred       20 // knob 1.
#define AIgreen     21 // knob 2.

// configuration definitions for frequency
#define SQUARE_FREQ 10
#define SERIAL_FREQ 50

#define NUM_STEPS 10
#define NUM_STIMS (NUM_STEPS * NUM_STEPS)

const int greenMin = 0, greenMax = 2000;
const int redMin = 0, redMax = 3200;
const int blueMin = 0, blueMax = 3000;

float greenArray[NUM_STEPS];
float redArray[NUM_STEPS];
float blueArray[NUM_STEPS];

int coordinates[NUM_STIMS][2];
int newcoordinates[NUM_STIMS][2];
int expSequence[NUM_STIMS][2];

// bounce object. From library. This controls the interruption for the push button.
Bounce pushbutton = Bounce();

// flashing information for frequency
int timeFlash10 = 50; // 50 = 10HZ, 
int flash10T = 30;
int D = 12; // This selects the resolution 

unsigned int numBaselineTr = 1;
volatile int order = 0; //sizeof(redVals); size of doesn't return the number of elements, does number of bytes...

volatile int numValsLW = 20; //sizeof(redVals); size of doesn't return the number of elements, does number of bytes...

// So, depending on the command, redVals and greenVals are set. Start with default.
unsigned long halfPeriod = (1000 / (2 * SQUARE_FREQ));  // Calculate half period (50ms for 10 Hz) for blinking
// ****** Serial variables
// declare variables to send via serial. These might need to be volatile variables
unsigned int pressFlag        = 0;
unsigned int currentGreenTri  = 0;
unsigned int currentRedTri    = 0;
unsigned int currentBlueTri   = 0;
unsigned int currentAmberTri  = 0; 
unsigned int trCnt            = 0;
unsigned int trigFlag         = 0;

volatile bool started         = false;  // To track if the Teensy execution should start
unsigned long timeBounc       = 0.0;
unsigned int trialLength      = 3000; // lenght of the trial (ms)
unsigned int interTrialWait   = 750; // wait between trials (ms)

volatile int outputValueRed   = 0;       // Adjusted value for RED
volatile int outputValueBlue  = 0;       // Adjusted value for RED
volatile int outputValueGreen = 0;     // Adjusted value for GREEN

volatile unsigned int mode    = 0;

int initialOffsetRed    = 0;           // Initial random offset
int initialOffsetGreen  = 0;           // Initial random offset

volatile int currRED    = 0;
volatile int currGREEN  = 0;
volatile int currBLUE   = 0;
volatile int currAMBER  = 0;
unsigned int amberVal   =  2400;
unsigned int cyanVal    =  2400;

String endOfFrame = "~~~";
// this is a test for the timer controlling the amber thing

volatile bool AMBERState  = false;
volatile bool CYANState   = false;
volatile bool REDState    = true;
volatile bool GREENState  = true;
volatile bool BLUEState   = true;

uint8_t L1Pin;
uint8_t L2Pin;
uint8_t L3Pin;

uint8_t L1;
uint8_t L2;
uint8_t L3;

int state = 0;

unsigned int maxRed         = 3000;
unsigned int minRed         = 300;
unsigned int maxGreen       = 2400;
unsigned int minGreen       = 300;

const int numSamples = 10;  // Number of samples for averaging
const int sampleInterval = 5;  // Time between samples in milliseconds

// // Smoothing factor (0 < alpha < 1), closer to 1 updates faster
const float alpha = 0.5;

// Function to toggle the AMBER or CYAN LED
void toggleL1() {
    AMBERState = !AMBERState;
    analogWrite(L1Pin, AMBERState ?  currAMBER: 0);
}

// Function to start or stop the timer
void controlTimerL1(bool enable, uint8_t pin) {
  L1Pin = pin;
    if (enable) {
        timerL1.begin(toggleL1, halfPeriod*1000); // Start timer with 50 ms interval (10 Hz)
    } else {
        timerL1.end(); // Stop the timer
    }
}

// Function to toggle the RED lED
void toggleL2() {
    REDState = !REDState;
    analogWrite(L2Pin, REDState ?  currRED: 0);
}

// Function to start or stop the timer
void controlTimerL2(bool enable, uint8_t pin) {
    L2Pin = pin;
    if (enable) {
        timerL2.begin(toggleL2, halfPeriod*1000); // Start timer with 50 ms interval (10 Hz)
    } else {
        timerL2.end(); // Stop the timer
    }
}

// Function to toggle the RED lED
void toggleL3() {
    GREENState = !GREENState;
    analogWrite(L3Pin, GREENState ?  currGREEN: 0);
}

// Function to start or stop the timer
void controlTimerL3(bool enable, uint8_t pin) {
    L3Pin = pin;
    if (enable) {
        timerL3.begin(toggleL3, halfPeriod*1000); // Start timer with 50 ms interval (10 Hz)
    } else {
        timerL3.end(); // Stop the timer
    }
}

// ********* Definition of functions ********* //
// interruption handler. does this have to be managed with an interrupt, or can it be its own thread? 
void handleButtonPress() 
{
  unsigned long currentMillis = millis();
  if (currentMillis - timeBounc > 250)
  {
    switch (mode){
      case 0:
        currRED   = 0;
        currGREEN = 0;
        currAMBER = 0;
      break;

      case 1: //random walk mode
        Serial.println("boing");
      break;

      case 2: //variable resistorMode
        currRED   = 0;
        currGREEN = 0;
        currAMBER = 0;
        
        print_varResmode();
      break;

      default:
        Serial.println("invalid");
      break;
    }
    timeBounc = millis();
  }
}

void print_varResmode(){
  // Once the participant presses the button, we stop excecution and record the current values
  String dataFrame = String(0) + "@" + 
                        String(0) + "@" + 
                        String(amberVal) + "@" + 
                        String(outputValueRed) + "@" + 
                        String(outputValueGreen) + "@" + 
                        String(0);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
  Serial.println(dataFrame);
                      // Send the data frame via Serial
  Serial.println(endOfFrame);
  started = false; 
  mode    = 0;
}
// ********* operational functions   ********* // 
void getLinSpacedArrays() {
      for (int i = 0; i < NUM_STEPS; i++) {
        greenArray[i] = greenMin + (greenMax - greenMin) * i / (NUM_STEPS - 1);
        redArray[i] = redMin + (redMax - redMin) * i / (NUM_STEPS - 1);
    }
}

void produceSequence() {
    // Generate diagonal coordinates
    int count = 0;
    int d = 2; // Diagonal index

    while (count < NUM_STIMS) {
        if (d % 2 == 1) {
            // Odd diagonal: move downward (increasing x, decreasing y)
            for (int x = 1; x <= min(d - 1, NUM_STEPS); x++) {
                int y = d - x;
                if (y > 0 && y <= NUM_STEPS) {
                    coordinates[count][0] = x;
                    coordinates[count][1] = y;
                    count++;
                }
                if (count >= NUM_STIMS) break;
            }
        } else {
            // Even diagonal: move upward (increasing y, decreasing x)
            for (int y = 1; y <= min(d - 1, NUM_STEPS); y++) {
                int x = d - y;
                if (x > 0 && x <= NUM_STEPS) {
                    coordinates[count][0] = x;
                    coordinates[count][1] = y;
                    count++;
                }
                if (count >= NUM_STIMS) break;
            }
        }
        d++; // Move to the next diagonal
    }
}

void modSeqOrder(int order) {

    switch (order){
    
    // order 1 starts from 1,1
    case 1:
      for (int i = 0; i < NUM_STIMS; i++) {
        newcoordinates[i][0] = coordinates[i][0]; // Keep X
        newcoordinates[i][1] = coordinates[i][1]; // Keep Y
      }
      
    break;

    // order 2 starts from 1, Maxy
    case 2:
      for (int i = 0; i < NUM_STIMS; i++) {
        newcoordinates[i][0] = coordinates[i][0]; // Keep X
        newcoordinates[i][1] = NUM_STEPS + 1 - coordinates[i][1]; // Flip Y
      }
    break;
    // order 3 starts from MaxX, 1
    case 3:
      for (int i = 0; i < NUM_STIMS; i++) {
        newcoordinates[i][0] = NUM_STEPS + 1 - coordinates[i][0]; // Flip X
        newcoordinates[i][1] = coordinates[i][1]; // Keep Y
      }
      
    break;
    // order 4 starts from MaxX, MaxY
    case 4: 
      for (int i = 0; i < NUM_STIMS; i++) {
        newcoordinates[i][0] = NUM_STEPS + 1 - coordinates[i][0]; // Flip X
        newcoordinates[i][1] = NUM_STEPS + 1 - coordinates[i][1]; // Flip Y
      }
    default:

    break;
    }

    // Serial.print(" Coordinates Order "); Serial.println(order);
    // for (int i = 0; i <= NUM_STIMS - 1; i++) {
    //   Serial.print(newcoordinates[i][0]);
    //   Serial.print(", ");
    //   Serial.println(newcoordinates[i][1]);
  //}
}

void getExpSequence(int order) {
    modSeqOrder(order);
    for (int i = 0; i < NUM_STIMS; i++) {
      expSequence[i][0] = redArray[newcoordinates[i][0] -1]; 
      expSequence[i][1] = greenArray[newcoordinates[i][1] -1]; 
    }

  //   Serial.print(" Exp Sequence Order "); Serial.println(order);
  //   for (int i = 0; i < NUM_STIMS; i++) {
  //     Serial.print(expSequence[i][0]);
  //     Serial.print(", ");
  //     Serial.println(expSequence[i][1]);
  // }
}

// ********* Definition of functions ********* //
// interruption handler. does this have to be managed with an interrupt, or can it be its own thread? 

// ********* Functions for printing and stoping excecution ********* //

// Function to control LED brightness with square wave. This function presents the experiment 
void controlLEDs() 
{
  while (true) 
  {
    if (started == true)
    {
      switch (mode){
        case 0:
          currRED   = 0;
          currGREEN = 0;
          currAMBER = 0;
          controlTimerL1(false,L1); // Start the LED blinking
          controlTimerL2(false,L2);
          controlTimerL3(false,L3);
        break;
        // grid experiment 
        case 1:
          REDState = true;
          GREENState = true;
          AMBERState = false;

          blink_LEDs_Grid(); // red to green EEG
        break;
        // fixed experiment 
        case 2:
          REDState    = true;
          GREENState  = true;
          AMBERState  = false;
          currAMBER   = amberVal;
          var_nob_exp();
        break;

        default:
          Serial.println("paila, bebe");
        break;
      }
    started = false;
    }
  }     
}

void var_nob_exp () {
// Read the analog inputs
  controlTimerL1(true,L1);
  controlTimerL2(true,L2);
  controlTimerL3(true,L3);

  while(true)
  {
    if (started == false) 
    {
    mode      = 0;
    currAMBER = 0;
    currRED   = 0;
    currGREEN = 0;
    break;}

  unsigned long startTime = millis();
  unsigned long lastSampleTime = startTime;
  int sampleCount = 0;
  int sumRed = 0, sumGreen = 0;

  int sensorValueRed   = 0;
  int sensorValueGreen = 0;

  while (millis() - startTime < 50) {
      if (millis() - lastSampleTime >= sampleInterval) {
          lastSampleTime = millis();  // Update the last sample time

          sensorValueRed   = analogRead(AIred);
          sensorValueGreen = analogRead(AIgreen);

          sumRed   += sensorValueRed;
          sumGreen += sensorValueGreen;
          sampleCount++;

          if (sampleCount >= numSamples) {
              break;  // Stop sampling once we've collected numSamples
          }
      }
  }

  // Compute the average values if at least one sample was taken
  if (sampleCount > 0) {
      int avgRed   = sumRed / sampleCount;
      int avgGreen = sumGreen / sampleCount;

      // Adjust red sensor value
      int adjustedValueRed = avgRed + initialOffsetRed;
      //int adjustedValueRed = avgRed + 0;

      if (adjustedValueRed > 4095) {
          adjustedValueRed -= 4095;
      }

      // Adjust green sensor value
      int adjustedValueGreen = avgGreen + initialOffsetGreen;
      //int adjustedValueGreen = avgGreen + 0;

      if (adjustedValueGreen > 4095) {
          adjustedValueGreen -= 4095;
      }

      // Map the averaged and adjusted values
      outputValueRed   = map(adjustedValueRed, 0, 4095, 0, maxRed);
      outputValueGreen = map(adjustedValueGreen, 0, 4095, 0, maxGreen);

      // Update current values
      currRED   = outputValueRed;
      currGREEN = outputValueGreen;

      }
      }
  }

void blink_LEDs_Grid() 
{
  if (started == true)
  {
    // Present baseline trials
    baseline_trial(numBaselineTr);
    
    // Keep the LED pulsing at 10 Hz for x seconds
    for (int i = 0; i < NUM_STIMS; i++) 
    {
      if (started==false) {break;
      currAMBER = 0;
      currRED   = 0;
      currGREEN = 0;
      mode      = 0;}

      trCnt           = i+2;
      Serial.print("Stim No: "); Serial.print(i+1); Serial.print(" L2 (R or B): "); Serial.print(expSequence[i][0]);  Serial.print(" L3 (green): "); Serial.println(expSequence[i][1]); 

      currentRedTri   = expSequence[i][0];
      currentGreenTri = expSequence[i][1];
      currentAmberTri =0;
      // Keep the LED pulsing at 10 Hz for 3 seconds
      controlTimerL1(true,L1);
      controlTimerL2(true,L2);
      controlTimerL3(true,L3);
      digitalWrite(trigger, HIGH);

      unsigned long startTime = millis();
      while (millis() - startTime < trialLength) 
      {  // Loop for 3 seconds
        currentAmberTri = amberVal;
        currAMBER       = amberVal;
        currRED         = currentRedTri;
        currGREEN       = currentGreenTri;
      }
      digitalWrite(trigger, LOW);
      currRED         = 0;
      currGREEN       = 0;
      currAMBER       = 0;
      currentRedTri   = 0;
      currentGreenTri = 0;
      //set_trigVal(0);
      // now it is intertrial wait time
      threads.delay(interTrialWait);  // Wait 400 ms before moving to the next brightness level
    }
    baseline_trial(numBaselineTr);
    reset_board();
  started = false;
  }     
}


// Function for selecting the number of baseline trials of interest
void baseline_trial(unsigned int reps) 
{
  if (started==true){

    controlTimerL2(false, L2);
    controlTimerL3(false, L3);
    controlTimerL1(false, L1);
    for (unsigned int ri = 0; ri <= reps; ri ++) 
    {
      analogWrite(L1, amberVal);
      digitalWrite(trigger, HIGH);
      // Consider getting these in a function
      currentAmberTri   = amberVal;
      trCnt             = ri + 1;
      trigFlag          = 1;
      threads.delay(trialLength);
      digitalWrite(trigger, LOW);
      analogWrite(L1, 0);
      currentAmberTri   = 0;
      trigFlag          = 0;
      trCnt             = ri + 1;
      threads.delay(interTrialWait);
    }
  }
}

// *********** operative functions ************//
void reset_board() 
{
    // zero the hardware
    currRED   = 0;
    currGREEN = 0; 
    currAMBER = 0;
    // zero the variables 
    currentGreenTri   = 0;
    currentRedTri     = 0;
    pressFlag         = 0;
    currentAmberTri   = 0; 
    trCnt             = 0;
    trigFlag          = 0;
    delay(50); // pause for a brief moment
    //Serial.println("Board was reset");
}
// Function to generate and scramble an array with values up to maxValue

//////// **************************** config chunk

void setup() {
  Serial.begin(38400);
  // Here we set up the pins as outputs. This is what we will be controlling.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // This is the configuration required to get the button ready
  pushbutton.attach(BUTTON_PIN); 
  pushbutton.interval(200);

  // attach an interrupt to the button pin, triggered by a falling edge, since we are using a pull up resistor
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),handleButtonPress, FALLING);

  // init pins for other LEDs
  pinMode(AMBER, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(CYAN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(trigger, OUTPUT); // Not sure what this does

  // set the PWM resolution to 4096 
  analogWriteResolution(12);
  analogReadResolution(12); // Values range from 0 to 2047

  // // thread to handle button press
  timeBounc = millis();
  while (!Serial);  // Wait for serial connection
  //Serial.println("Provide the experiment type: grid OR 1789");
  // Create a thread to control LED 
  threads.addThread(controlLEDs,1);
  //threads.addThread(adc_reads,2);
}
void loop() 
{
  pushbutton.update();
  if (started == false) 
  {
    if (Serial.available() > 0) 
    {    
      String command = Serial.readStringUntil('\n');  // Read command until newline
      //Serial.print("Received command: ");  // Print received command for debugging
      //Serial.println(command);

      if (command == "1789") // variable resistor experiment. Dials 
      {  
        L1 = CYAN;
        L2 = BLUE;
        L3 = GREEN;
        reset_board();
        threads.delay(10);
        initialOffsetRed    = random(300, 1500);
        initialOffsetGreen  = random(300, 1500);

        mode = 2; // Var random expt. stops at button press.
        state   = 0;
        started   = true;
        //Serial.println("Starting execution of variable resistor experiment");
      }

      if (command == "1777") // variable resistor experiment. Dials 
      {  
        L1 = AMBER;
        L2 = RED;
        L3 = GREEN;
        reset_board();
        threads.delay(10);
        initialOffsetRed    = random(300, 1500);
        initialOffsetGreen  = random(300, 1500);

        mode = 2; // Var random expt. stops at button press.
        state   = 0;
        started   = true;
        //Serial.println("Starting execution of variable resistor experiment");
      }

      else if (command == "RGgrid") {
        // Define the LED pins to use for this L1 = amber, L2 = Red, L3 = Green
        L1 = AMBER;
        L2 = RED;
        L3 = GREEN;
        state = 2;
        //Serial.println("Provide the experiment setting (order) with the following format @order!");
      }
      else if (command == "BGgrid") {
        // Define the LED pins to use for this L1 = Cyan, L2 = Blue, L3 = Green
        L1 = CYAN;
        L2 = BLUE;
        L3 = GREEN;
        state = 2;
        //Serial.println("Provide the experiment setting (order) with the following format @order!");
      }
      switch (state){
        case 0:
          
        break;
        // state 1 checks experiment type = fixed
        case 1:{
          
          mode    = 2;
          state   = 0;
          started = true;
          break;  
        }
        // state = 2 checks for experiment type = grid
        case 2:{
          int atIndex = command.indexOf('@');
          int virgIdx = command.indexOf('!');

          if (atIndex == -1) {break;}
          if (virgIdx == -1) {break;}

          //Serial.println(atIndex);
          //Serial.println(virgIdx);

          String orderString    = command.substring(atIndex + 1, virgIdx);         // Extract the part after '@', before '!'

          //Serial.println(orderString);

          order     = orderString.toInt();

          // produce the sequences to test
          getLinSpacedArrays();
          produceSequence();
          getExpSequence(order);

          delay(1000);

          mode    = 1;
          state   = 0;
          started = true;
          break;  
        }
      }
    }
  }
  else{
    if (Serial.available() > 0) 
    {    
      String command = Serial.readStringUntil('\n');  // Read command until newline
      //Serial.print("Received command: ");  // Print received command for debugging
      //Serial.println(command);

      if (command == "stop") {
        state   = 0;
        reset_board();
        started = false;
        //Serial.println("stopping experiment");
      }
    }
  }
}

    
 