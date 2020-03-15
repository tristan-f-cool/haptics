// This Software demo for the Precision Microdrives Haptic Shield is provided
// under the MIT License

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// REFERENCE: https://www.precisionmicrodrives.com/haptic-feedback/haptic-feedback-example-applications-haptic-touchscreen/

//*************************** WIRING  **************************************//
/* PMD Haptic Feedback Evaluation Kit (HFEK) in Developer Mode
 * Arduino Uno (from Arduino Mega)
 * Navigation   A3
 * Confirmation A2
 * Reset        3
 * Power        12
 */
 
// Include system headers
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

// Variables for button identification

int navigation, confirmdecline, reset, power;
boolean switchon = false;

void setup()
{
	// Setup serial
	Serial.begin( 9600 );
	Serial.print( F("FreeMem=") );
	Serial.println( freeRAM() );
	
	setupPins();
	i2cInit( 200 );
	
	// Set up the motor
	motor.selectMotor( 3 );
	motor.autoCalibrate();

	// Ensure any time for calibration is ignored.
	timer.reset();
}

void loop()
{
  
  navigation = digitalRead(A3);
  confirmdecline = digitalRead(A2);
  reset = digitalRead(3);
  power = digitalRead(12);
  
  if (navigation == HIGH)
  {
    motor.playFullHaptic( 6 , 7 );
    navigation = LOW;
    delay(1000);
  }
  
  if (confirmdecline == HIGH)
  {
    motor.playFullHaptic( 6 , 27 );
    confirmdecline = LOW;
    delay(1000);
  }
  
  if (reset == HIGH)
  {
    motor.playFullHaptic( 6 , 56 );
    reset = LOW;
    delay(1000);
  }
  
  if (power == HIGH)
  {
    if (switchon == true)
    {
      motor.playFullHaptic( 6 , 82 );
    }
    
    else
    {
      motor.playFullHaptic( 6 , 71 );
    }
    power = LOW;
    switchon = !switchon;
    delay(1000);
  }
  
  Serial.println(navigation);  
  Serial.println(confirmdecline);  
  Serial.println(reset);  
  Serial.println(power);  
	
}

void setupPins()
{
	pinMode( SW_LRA_M,		OUTPUT );	// Output for switching LRA+/- v.s. M+/-
	pinMode( SW_MOS_DRV,	OUTPUT );	// Output for switching +3.3V/MOS- v.s. DRV+/-
	pinMode( DRV_2605_EN,	OUTPUT );	// Output for ERV2605 ENable
	pinMode( PWM_OUT,		OUTPUT );	// Output for PWM

	digitalWrite( SW_LRA_M,		LOW );	// Select M+ and M- 
	digitalWrite( SW_MOS_DRV,	HIGH );	// Select +3.3V and MOS- 
	digitalWrite( DRV_2605_EN,	LOW );	// Disable the DRV2605 (low power mode)
	digitalWrite( PWM_OUT,		LOW );	// PWM output low for now 

	pinMode( MOTOR_PIN_0,	OUTPUT );	// Motor select 1
	pinMode( MOTOR_PIN_1,	OUTPUT );	// Motor select 2
        pinMode( 12,	INPUT );
        pinMode( 3,	INPUT );
}	// setupPins
