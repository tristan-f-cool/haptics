/***************************************************************************
 * Precision Microdrives
 * SCREEN_HAPTICS for Haptic Feedback Evaluation Kit 2 (HFEK2) and demos
 * by: Tristan Cool
 * August 2019 
 * Arduino UNO
***************************************************************************/

//*************************** INCLUDE **************************************//
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>

#include <SPI.h>
#include <mpr121.h>
#include <i2c.h>

// Debug
#include "debug.h"

// Include our headers
#include "Metro.h"
#include "motor.h"

#include "Accelerometer.h"

// Init timer for 2s, and auto-reset when we get a positive check
Metro timer( 2000, 1 );
Motor motor = Motor();

//*************************** WIRING  **************************************//
/* PMD Haptic Feedback Evaluation Kit (HFEK) in Developer Mode
 * 
 * DRV2605 VIN --> ARD 5V
 * DRV2605 GND --> ARD GND
 * DRV2605 SCL --> ARD A5 (Mega D21, Micro D3)
 * DRV2605 SDA --> ARD A4 (Mega D20, Micro D2)
 * 
 * Button1 --> ARD D5
 * Button2 --> ARD D8
 * Button3 --> ARD D10
 * 
 * Accel. X --> ARD A0
 * Accel. Y --> ARD A1
 * Accel. Z --> ARD A2
 * 
 */

//*************************** VARIABLES **************************************//
bool demo_mode = false; //default Application Mode

int motor_id = 3; //default LRA (C10-100)

const int pinButton1 = 5;   //D5
int Button1 = 0;
int prevButton1 = 0;
const int pinButton2 = 8;   //D8
int Button2 = 0;
int prevButton2 = 0;
const int pinButton3 = 10;  //D10
int Button3 = 0;
int prevButton3 = 0;

typedef struct{
  int value;
  String id;
} effect;

//add effects - (Clicks, Sliders)
effect EFFECT[10] = {{5,"single-click sharp"},{25,"single-tick sharp"},
                     {35,"double-tick sharp"},{36,"double-click sharp"},
                     {87,"ramp-up short"},{85,"ramp-up medium"},{83,"ramp-up long"},
                     {75,"ramp-down short"},{73,"ramp-down medium"},{71,"ramp-down long"}};

int index = 0;
int max_effects = sizeof(EFFECT)/sizeof(effect);

//Accelerometer
#define pinX A0
#define pinY A1
#define pinZ A2
Accelerometer accelerometer(183,182,198,0.0303,0.0303,0.0303); // xyz threshold, xyz gain

// ******** FUNCTION PROTOTYPES ***********
void PrintEffect(effect &EFFECT);

//*************************** SETUP **************************************//
void setup()
{
  //setup Serial
  Serial.begin( 9600 );
  Serial.println(F("PRECISION MICRODRIVES - SCREEN_HAPTICS_PMD DEMO"));
  //Serial.print( F("FreeMem=") );
  //Serial.println( freeRAM() );
  Serial.println(F("!=====*=======*=======*======*======*=======*======!"));
  Serial.println(F(":: Press and hold Button2 & Button3 to change screen/motor."));
  Serial.println(F(":: Press and hold Button1 & Button3 to change Demo/App mode."));
  Serial.println(F(":: Toggle switches between screen sizes and actuators."));

  //init Buttons
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  pinMode(pinButton3, INPUT);
 
  //init DRV
  setupPins();
  i2cInit( 200 );
  
  //Set up the motor
  motor.selectMotor(motor_id); 
  motor.autoCalibrate();

  //Set up the accelerometer
  accelerometer.init(pinX,pinY,pinZ);
  accelerometer.auto_calibrate();
  //accelerometer.manual_calibrate(5,5,5);

  //Ensure any time for calibration is ignored.
  timer.reset();

} //end:SETUP

//*************************** LOOP **************************************//
void loop()
{
  //============= ORIGINAL CODE ====================
//  // Example code - check for some condition
//  // N.B. We play an effect every 2 seconds
//  uint8_t condition = timer.check();
//  if( condition )
//  {
//    // Do an effect
//   condition = 0;
//   motor.playFullHaptic( 1, 53 );
//  }

    //read buttons
    Button1 = digitalRead(pinButton1);
    Button2 = digitalRead(pinButton2);
    Button3 = digitalRead(pinButton3);
    
  //============= DEMO CODE =======================
  //Demo effects (with both motors)
  if(demo_mode)
  {
    demo();
  } //DEMO CODE

  //============= APPLICATION CODE ================
  else
  { 
    //Button1 is used to play effect
    if(Button1)
    {
      //change mode by Button1 + Button3
      if(Button3 != prevButton3){
        if(Button3)
        {
          SetMode();
          Serial.println(F("$$$$$$$$$$$ DEMO mode $$$$$$$$$$$$$$"));
        }
        prevButton3 = Button3;
      }
      else
      {
        PrintIndex();
        PrintEffect(EFFECT[index]);
        PlayEffect(EFFECT[index].value);
      }
    }
  
    //Button2 is used to increment effect
    if(Button2 != prevButton2){
      if(Button2)
      {
        //change motor by Button2 + Button3
        if(Button3)
          SetMotor();
        
        else 
          index++;
          if(index>max_effects-1)
            index=max_effects-1; 
          PrintIndex();
      }
      delay(50);
    }
    prevButton2 = Button2;
  
    //Button3 is used to decrement effect
    if(Button3 != prevButton3){
      if(Button3)
      {
        //change motor by Button2 + Button3
        if(Button2)
          SetMotor();
  
        else
          index--;
          if(index<0)
            index=0;
          PrintIndex();
      }
      delay(50);
    }
    prevButton3 = Button3;
  } //APPLICATION CODE
  
} //end:LOOP


//*************************** FUNCTIONS **************************************//

void setupPins()
{
    pinMode( DRV_2605_EN,       OUTPUT );   // Output for DRV2605 Enable
    pinMode( PWM_OUT,           OUTPUT );   // Output for PWM
    pinMode( SRC_SEL,           OUTPUT );   // HIGH = DRV, LOW = MOS 
    pinMode( GRIP_SEL,          OUTPUT );   // LOW = Haptic Grip Connected
    pinMode( LRA_SEL,           OUTPUT );   // LOW = Haptic Shield LRA Connected
    pinMode( ERM_SEL,           OUTPUT );   // LOW = Haptic Shield ERM Connected

    pinMode( MOTOR_PIN_0,       OUTPUT );   // Motor select 1
    pinMode( MOTOR_PIN_1,       OUTPUT );   // Motor select 2
    
    digitalWrite( PWM_OUT,      LOW );      // PWM output low
    digitalWrite( SRC_SEL,      HIGH );     // Select DRV
    digitalWrite( GRIP_SEL,     HIGH  );    // Select none 
    digitalWrite( ERM_SEL,      HIGH  );    // Select none
    digitalWrite( LRA_SEL,      HIGH  );    // Select none 
} //end:setupPins

void SetMode()
{
  if(demo_mode)
    demo_mode = false;
  else
    demo_mode = true;
} //end:SetMode

void SetMotor()
{
  //3 = C10-100 for small touchscreen ; 6 = TAC-C27 for large touchscreen
  if(motor_id == 3)
    motor_id = 6;
  else
    motor_id = 3;

  Serial.println(F("££££££££ MOTOR £££££££££"));
  Serial.println(F("(Set switch to C10-100(3) for small screen ; TAC-C27(6) for large screen)"));
  //Experiment with running C27 as C10 (!!!)
  Serial.print(F("motor: "));
  Serial.println(motor_id);
  motor.selectMotor(motor_id); 
  motor.autoCalibrate(); 
} //end:SetMotor

void PrintIndex()
{
  if(index < 5) //"Click" effects
    Serial.print(F("Click"));
  else          //"Slider" effects
    Serial.print(F("Slider"));
    
  Serial.print(F(" - effect: "));
  Serial.print(index+1);
  Serial.print(F(" of "));
  Serial.println(max_effects);
} //end:PrintIndex

void PrintEffect(effect &EFFECT)
{
  Serial.print(EFFECT.value);
  Serial.print(F(": "));
  Serial.println(EFFECT.id);
} //end:PrintEffect

void PlayEffect(int effect)
{
    //set and play effect
    motor.playFullHaptic( 1, effect );
    //start accelerometer reading
    accelerometer.start(true);
    delay(1000);
} //end:PlayEffect

void demo()
{
  delay(1000);
  for(int i = 0; i < max_effects; i++)
  { 
    //read buttons
    Button1 = digitalRead(pinButton1);
    Button2 = digitalRead(pinButton2);
    Button3 = digitalRead(pinButton3);  
    if(Button1)
    {
      if(Button3)
      {
        SetMode();
        Serial.println(F("$$$$$$$$$ APP mode $$$$$$$$$$$$$"));
        break;
      }
    }
    else
    {
      PrintEffect(EFFECT[i]);
      PlayEffect(EFFECT[i].value);
      delay(1000);
    }
  }
  SetMotor(); //repeat with next motor
} //end:demo
