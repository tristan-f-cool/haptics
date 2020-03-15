/***************************************************************************
 * Precision Microdrives
 * JOYSTICK_HAPTICS for Haptic Feedback Evaluation Kit 2 (HFEK2) and demos
 * by: Tristan Cool
 * September 2019 
 * Arduino UNO
***************************************************************************/
// REFERENCE: https://www.precisionmicrodrives.com/haptic-feedback/haptic-feedback-example-applications-joystick-crane-interface/

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

// Init timer for 2s, and auto-reset when we get a positive check
Metro timer( 2000, 1 );
Motor motor = Motor();

//*************************** WIRING  **************************************//
/* PMD Haptic Feedback Evaluation Kit (HFEK) in Developer Mode
 * Joystick X - A2
 * Joystick Y - A3
 * Joystick SEL - TBD
 */

//*************************** VARIABLES **************************************//
// Joystick
//read and position
int initX, initY, Xread, Yread, moveX, moveY, Xco, Yco = 0;
//speed
int adj [] = {-10, -5, -1, 0, 1, 5, 10};
//scale 
int limits [] = {-475, -250, -50, 50, 250, 475, 1000};

// ******** FUNCTION PROTOTYPES ***********


//*************************** SETUP **************************************//
void setup()
{
  //setup Serial
  Serial.begin( 9600 );
  Serial.println(F("PRECISION MICRODRIVES - JOYSTICK_HAPTICS_PMD DEMO"));
  //Serial.print( F("FreeMem=") );
  //Serial.println( freeRAM() );

  //init DRV
  setupPins();
  i2cInit( 200 );
  
  //Set up the motor (LRA/Solenoid)
  motor.selectMotor(3); 
  motor.autoCalibrate();

  //Ensure any time for calibration is ignored.
  timer.reset();

  //joystick
  initialisejoystick();           
  printheader();

} //end:SETUP

//*************************** LOOP **************************************//
void loop()
{
  measure();
  adjustcoordinates();
  playeffects();
  printmovement();
  delay (100); 
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
    
    pinMode( XPIN,              INPUT);     // Input for joystick X axis
    pinMode( YPIN,              INPUT);     // Input for joystick Y axis
} //end:setupPins

void initialisejoystick()
{
  initX = analogRead(XPIN);
  initY = analogRead(YPIN);
  Serial.print("X adjustment ratio = ");
  Serial.println(initX);
  Serial.print("Y adjustment ratio = ");
  Serial.println(initY);
} //end:initialiseJoystick

void printheader()
{
  Serial.println("----------------------------------------------");
  Serial.println(" ");
  Serial.print("\t");
  Serial.print("X");
  Serial.print("\t");
  Serial.println("Y");
  Serial.println("----------------------------------------------");
} //end:printheader

void measure()
{
  Xread = analogRead(XPIN);
  Xread = Xread - initX;
  Yread = analogRead(YPIN);
  Yread = Yread - initY;
  Yread = Yread * (-1);
} //end:measure

void adjustcoordinates()
{
  for (int i=0; i < 7; i++)
  {
    if ( Xread < limits [i])
    {
      moveX = adj[i];
      break;
    }
  }

  for (int i=0; i < 7; i++)
  {
    if ( Yread < limits [i])
    {
      moveY = adj[i];
      break;
    }
  }
  
  Xco = Xco + moveX;
  Yco = Yco + moveY;
  
  
  /* The following if statements confine the Xco 
     and Yco to maximums of 100 and of -100 */
     
  if (Xco < -100)
  {
    Xco = -100;
  }
  
  else if (Xco > 100)
  {
    Xco = 100;
  }
  
  if (Yco < -100)
  {
    Yco = -100;
  }
  
  else if (Yco > 100)
  {
    Yco = 100;
  }
} //end:adjustcoordinates

void playeffects()
{
  /* Plays an effect depending on distance to boundary
     The closer to the boundary the more urgent the effect! */
  
  if ( Xco == 100 || Xco == -100 || Yco == 100 || Yco == -100)
  {
    motor.playFullHaptic ( 6, 92 );
    motor.playFullHaptic ( 6, 92 );
  }
  
  else if ( 84 < Xco || -84 > Xco || 84 < Yco || -84 > Yco )
  {
    motor.playFullHaptic ( 6 , 12 );
  }
  
  else if ( 59 < Xco || -59 > Xco || 59 < Yco || -59 > Yco )
  {
    motor.playFullHaptic ( 6 , 9 );
  }
} //end:playeffects

void printmovement()
{
  Serial.print("\t");
  Serial.print(Xco);
  Serial.print("\t");
  Serial.println(Yco);
} //end:printmovement
