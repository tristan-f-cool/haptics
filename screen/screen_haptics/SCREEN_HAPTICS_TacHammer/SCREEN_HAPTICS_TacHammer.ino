/***************************************************************************
 * Precision Microdrives
 * SCREEN_HAPTICS for Haptic Feedback Evaluation Kit 2 (HFEK2) and demos
 * by: Tristan Cool
 * August 2019 
 * Arduino MICRO
***************************************************************************/
//Source code: NANOPORT TACHAMMER DEVELOPER KIT FIRMWARE (TacHammerDevKitAutorun)

//*************************** INCLUDE *************************************//
#include <Wire.h>
#define PWM6        OCR4D
#define PWM13       OCR4A

//*************************** WIRING  *************************************//
/* DRV2605 VIN --> PSU 5V
 * DRV2605 GND --> ARD/PSU GND
 * DRV2605 SCL --> ARD D3
 * DRV2605 SDA --> ARD D2
 * DRV2605 IN  --> ARD D13
 * 
 * Button1 --> ARD D5
 * Button2 --> ARD D8
 * Button3 --> ARD D10
 */

//*************************** VARIABLES ***********************************//
//  SETUP I2C
byte DRV = 0x5A;    //DRV2605 slave address
byte ModeReg = 0x01;

// COUNTERS
int cycle = 0;

// BUTTONS
const int pinButton1 = 5;   //D5
int Button1 = 0;
int prevButton1 = 0;
const int pinButton2 = 8;   //D8
int Button2 = 0;
int prevButton2 = 0;
const int pinButton3 = 10;  //D10
int Button3 = 0;
int prevButton3 = 0;

//*************************** SETUP **************************************//

void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("PRECISION MICRODRIVES - SCREEN_HAPTICS_TacHammer DEMO");

  //init Buttons
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  pinMode(pinButton3, INPUT);

  pwm613configure();    //sets up PWM signal
  delay(2);
  initializeDRV2605();  //initializes DRV2605
  delay(10);
  //activate 
  pulse(0.1, 10);

} //end:SETUP

//*************************** LOOP ***************************************//
void loop() 
{
  //================= ORIGINAL CODE ========================
//  // true IF USING DIGITAL PIN 5 AS AN INPUT BUTTON
//  // false IF CYCLING THROUGH EFFECTS AUTOMATICALLY
//  if (false){     
//    if (!digitalRead(buttonPin)){
//      effectCycle(cycle);
//      cycle++;
//      pause(50);
//    }
//  }
//  else{
//    for (int i=0;i<12;i++){
//      effectCycle(i);
//    }
//  }
//}

  //=================== DEMO CODE =========================
  //Demo effects
  //demo();

  //============= APPLICATION CODE ================
  //read buttons
  Button1 = digitalRead(pinButton1);
  Button2 = digitalRead(pinButton2);
  Button3 = digitalRead(pinButton3);

  //Button1 is used to play effect
  if(Button1)
  {
    effectCycle(cycle);
  }
  
  //Button2 is used to increment effect
  if(Button2 != prevButton2){
    if(Button2)
    {
     cycle++;
     if(cycle>4)
      cycle=4; 
     Serial.println(cycle);
    }
    delay(50);
  }
  prevButton2 = Button2;

  //Button3 is used to decrement effect
  if(Button3 != prevButton3){
    if(Button3)
    {
      cycle--;
      if(cycle<0)
        cycle=0;
      Serial.println(cycle);
    }
    delay(50);
  }
  prevButton3 = Button3;

} //end:LOOP


//*************************** FUNCTIONS **********************************//

// ================================================================================
// =================== TACHAMMER BASIC FUNCTION LIBRARY ===================[TACLIB]
// ================================================================================
// the following are the basic function calls that are used to create more complex
// haptic experiences

// ================================================================================
// pulse drives the hammer towards the closed end of the TacHammer
// intensity defines the strength of the pulse. It ranges from [0-1] with 1 being strongest
// When the hammer rebounds off of the repelling magnetic array, the inaudible pulse haptic sensation is created
// milliseconds is the length of time the coil is activated in ms
// pulse is intended to be sequenced with subsequent pulse and hit commands and if called
// on its own, the hammer may travel after the rebound and strike the open end

void pulse(double intensity, double milliseconds) {
  int minimumint = 140;
  int maximumint = 255;
  int pwmintensity = (intensity * (maximumint - minimumint)) + minimumint;
  standbyOffB();
  PWM13 = pwmintensity;
  usdelay(milliseconds);
  standbyOnB();
}

// ================================================================================
// singlePulse includes a command to pulse the hammer followed by a command to brake the hammer.
// singlePulse is intended to be called on its own and should be followed by a pause command of
// at least 50ms before the next command is called
void singlePulse(double intensity, double milliseconds) {
  pulse(intensity, milliseconds);
  pause(3);
  pulse(intensity * 3 / 100, milliseconds * 2);
}

// ================================================================================
// hit drives the hammer towards the open end of the TacHammer
// intensity defines the strength of the hit. It ranges from [0-1] with 1 being strongest
// milliseconds is the length of time the coil is activated in ms
// When the hammer strikes the device housing, the audible click haptic sensation is created
void hit(double intensity, double milliseconds) {
  int minimumint = 0;
  int maximumint = 110;
  int pwmintensity = maximumint - (intensity * (maximumint - minimumint));
  standbyOffB();
  PWM13 = pwmintensity;
  usdelay(milliseconds);
  standbyOnB();
}

// ================================================================================
// waits for a duration of time defined in ms
void pause(double milliseconds) {
  double us = milliseconds - ((int)milliseconds);
  standbyOnB();
  for (int i = 0; i <= milliseconds; i++) {
    delay(1);
  }
  delayMicroseconds(us * 1000);
}

void usdelay(double time) {
  double us = time - ((int)time);
  for (int i = 0; i <= time; i++) {
    delay(1);
  }
  delayMicroseconds(us * 1000);
}

// ================================================================================
// Vibrate repeatedly calls the pulse command to drive the hammer into the closed end of the TacHammer
// intensity defines the strength of the vibrate. It ranges from [0-1] with 1 being strongest
// dutycycle defines what percent of the period the TacHammer is active. It ranges from 0-100
// frequency defines the frequency of the vibration
// duration defines the length of vibration in s
//
// suggested duty cycles for vibrate
// FREQ| 10| 30| 50| 70| 90|110|130|150|170|190|210|230|250|270|290
// DUTY| 32| 28| 33| 36| 38| 37| 41| 47| 45| 47| 39| 40| 44| 43| 40
void vibrate(double frequency, double intensity, double duration, int dutycycle) {
  int max_hit = 12;
  int min_hit = 1;
  int crossover = 60;
  int hitduration = 10 * dutycycle / frequency;

  boolean hold = false;
  double delayy;
  delayy = (1 / frequency * 1000) - hitduration;
  double timedown;
  timedown = duration * 1000;

  if (duration == 0) {
    hold =  true;
  }

  while (hold) {
    pulse(intensity, hitduration);
    pause(delayy);
  }

  while (timedown >= 0 && frequency < crossover) {
    pulse(intensity, hitduration);
    pause(3);
    pulse(0.002, delayy-3);

    timedown -= (delayy + hitduration);
  }

  while (timedown >= 0 && frequency >= crossover) {
    pulse(intensity, hitduration);
    pause(delayy);
    timedown -= (delayy + hitduration);
  }
}

// ================================================================================
// =============================== HAPTIC LIBRARY =========================[HAPLIB]
// ================================================================================
// the following are haptic effects created with the basic haptic function calls

// ================================================================================

// ================================================================================
// click from impact haptic
void impactClick() {
    Serial.println("impact click");
    pulse(1, 6);
    hit(1, 21);
  }
  
// ================================================================================
// simulates an ERM rumble
void ermRumble() {
  Serial.println("ERM Rumble");
  vibrate(30,0.7,0.33,30);
}

// ================================================================================
// simulates an LRA Click
void lraClick() {
    Serial.println("LRA Click");
    vibrate(250,1,0.01,70);
}

// ================================================================================
// plays a series of vibrates at varying frequencies and duty cycles
void vibrateRamp() {
  double i = 0;
  for (int j=0;j<20;j++){
    Serial.print("Vibrate Frequency: ");
    Serial.println(250-i);

    vibrate(200 - i, 0.7, 0.2, 70);

    if (i < 210) {
      i += 10;
    }
  }  
}

// ================================================================================
// sweeps the Pulse command from 0-1 intensity
void pulseRamp() {
  double i = 0;
    for (int j=0;j<20;j++){
    Serial.print("Pulse Intensity: ");
    Serial.println(i);

    singlePulse(i,8);
    pause(84);
    if (i < 0.99) {
      i += 0.05;
    }
  }
}

// ================================================================================
// cycles through various haptic effects 
void effectCycle(int count){
  Serial.println(count);
  if (count == 0){
    for (int j=0;j<4;j++){
      lraClick();
      pause(500);
    }
    pause(1000);
  }

  else if (count == 1){
    for (int j=0;j<4;j++){
      ermRumble();
    }
    pause(1000);
  }

  else if (count == 2){
    vibrateRamp();
    pause(1000);
  }

  else if (count == 3){
    for (int j=0;j<4;j++){
      impactClick();
      pause(500);
    }
    pause(1000);
  }

  else if (count == 4){
    pulseRamp();
    pause(1000);
  } 
}

// ********************************************************************************
void demo()
{
  impactClick();
  delay(1000);
  ermRumble();
  delay(1000);
  lraClick();
  delay(1000);
  vibrateRamp();
  delay(1000);
  pulseRamp();
  delay(1000);
}

// ================================================================================
// =============================== DRV SETUP ============================= [DRVSET]
// ================================================================================
// sets up the DRV2605 to drive the TacHammer components. It is strongly suggested
// to not edit this section
void standbyOnB() {
  Wire.beginTransmission(DRV);
  Wire.write(ModeReg);               // sets register pointer to the mode register (0x01)
  Wire.write(0x43);               // Puts the device pwm mode
  Wire.endTransmission();
}

void standbyOffB() {
  Wire.beginTransmission(DRV);
  Wire.write(ModeReg);               // sets register pointer to the mode register (0x01)
  Wire.write(0x03);               // Sets Waveform Mode to pwm
  Wire.endTransmission();
}

void initializeDRV2605() {
  Wire.beginTransmission(DRV);
  Wire.write(ModeReg);               // sets register pointer to the mode register (0x01)
  Wire.write(0x00);               // clear standby
  Wire.endTransmission();

  Wire.beginTransmission(DRV);
  Wire.write(0x1D);              // sets register pointer to the Libarary Selection register (0x1D)
  Wire.write(0xA8);              // set RTP unsigned
  Wire.endTransmission();

  Wire.beginTransmission(DRV);
  Wire.write(0x03);
  Wire.write(0x02);              // set to Library B, most aggresive
  Wire.endTransmission();

  Wire.beginTransmission(DRV);
  Wire.write(0x17);               // sets full scale reference
  Wire.write(0xff);               //
  Wire.endTransmission();

  Wire.beginTransmission(DRV);
  Wire.write(ModeReg);               // sets register pointer to the mode register (0x01)
  Wire.write(0x03);               // Sets Mode to pwm
  Wire.endTransmission();

  delay(100);

}

// Configure the PWM clock
#define PWM12k  5   //  11719 Hz

void pwm613configure() {
  // TCCR4A configuration
  TCCR4A = 0;

  // TCCR4B configuration
  TCCR4B = PWM12k;

  // TCCR4C configuration
  TCCR4C = 0;

  // TCCR4D configuration
  TCCR4D = 0;

  // TCCR4D configuration
  TCCR4D = 0;

  // PLL Configuration
  // Use 96MHz / 2 = 48MHz
  PLLFRQ = (PLLFRQ & 0xCF) | 0x30;
  // PLLFRQ=(PLLFRQ&0xCF)|0x10; // Will double all frequencies

  // Terminal count for Timer 4 PWM
  OCR4C = 255;

  //pwmSet6();
  pwmSet13();
}

// Set PWM to D6 (Timer4 D)
void pwmSet6() {
  OCR4D = 0;   // Set PWM value
  DDRD |= _BV(7);    // Set Output Mode D7
  TCCR4C |= 0x09;  // Activate channel D
}

// Set PWM to D13 (Timer4 A)
void pwmSet13() {
  OCR4A = 0;   // Set PWM value
  DDRC |= _BV(7);    // Set Output Mode C7
  TCCR4A = 0x82;  // Activate channel A
}
