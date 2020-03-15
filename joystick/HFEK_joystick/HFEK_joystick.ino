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

// Global variables for reading joystick and positioning
int initX, initY, Xread, Yread, moveX, moveY, Xco, Yco = 0;

// An array to set the moving speed of the joystick
// i.e. at extremities move fast (10 spaces)
int adj [] = {-10, -5, -1, 0, 1, 5, 10};

// An array to set the joystick speed scale
int limits [] = {-475, -250, -50, 50, 250, 475, 1000};

void setup()
{
	// Start Serial
	Serial.begin( 9600 );
	Serial.print( F("FreeMem=") );
	Serial.println( freeRAM() );
        Serial.println();
	
        // Set pins and initialise I2C bus
	setupPins();
	i2cInit( 200 );
	
	// Set up the motor (LRA)
	motor.selectMotor( 3 );
	motor.autoCalibrate();

	// Ensure any time for calibration is ignored.
	timer.reset();
        
        // Initialise the joystick
        Serial.println("INITIALISING JOYSTICK");
        Serial.println();
        initialisejoystick();
                
        printheader();
        
}

void loop()
{
  measure();
  adjustcoordinates();
  playeffects();
  printmovement();
  delay (100);
  
}



void setupPins()
{
	pinMode( SW_LRA_M,		OUTPUT );	// Output for switching LRA+/- v.s. M+/-
	pinMode( SW_MOS_DRV,	        OUTPUT );	// Output for switching +3.3V/MOS- v.s. DRV+/-
	pinMode( DRV_2605_EN,	        OUTPUT );	// Output for ERV2605 ENable
	pinMode( PWM_OUT,		OUTPUT );	// Output for PWM

        pinMode( XPIN,                  INPUT);         // Input for joystick X axis
        pinMode( YPIN,                  INPUT);         // Input for joystick Y axis

	digitalWrite( SW_LRA_M,		LOW );	// Select M+ and M- 
	digitalWrite( SW_MOS_DRV,	HIGH );	// Select +3.3V and MOS- 
	digitalWrite( DRV_2605_EN,	LOW );	// Disable the DRV2605 (low power mode)
	digitalWrite( PWM_OUT,		LOW );	// PWM output low for now 

	pinMode( MOTOR_PIN_0,	OUTPUT );	// Motor select 1
	pinMode( MOTOR_PIN_1,	OUTPUT );	// Motor select 2
}

void initialisejoystick()
{
  initX = analogRead(XPIN);
  initY = analogRead(YPIN);
  Serial.print("X adjustment ratio = ");
  Serial.println(initX);
  Serial.print("Y adjustment ratio = ");
  Serial.println(initY);
}

void printheader()
{
  Serial.println("----------------------------------------------");
  Serial.println(" ");
  Serial.print("\t");
  Serial.print("X");
  Serial.print("\t");
  Serial.println("Y");
  Serial.println("----------------------------------------------");
}

void measure()
{
  Xread = analogRead(XPIN);
  Xread = Xread - initX;
  Yread = analogRead(YPIN);
  Yread = Yread - initY;
  Yread = Yread * (-1);
}

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
}

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
}

void printmovement()
{
  Serial.print("\t");
  Serial.print(Xco);
  Serial.print("\t");
  Serial.println(Yco);
}

	
