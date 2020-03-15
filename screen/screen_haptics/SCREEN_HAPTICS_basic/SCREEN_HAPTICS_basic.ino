/***************************************************************************
 * Precision Microdrives
 * SCREEN_HAPTICS for Haptic Feedback Evaluation Kit 2 (HFEK2) and demos
 * by: Tristan Cool
 * August 2019 
 * Arduino UNO
***************************************************************************/

//*************************** INCLUDE **************************************//
#include <Wire.h>
#include "Adafruit_DRV2605.h"


//*************************** WIRING  **************************************//
/* DRV2605 VIN --> ARD 5V
 * DRV2605 GND --> ARD GND
 * DRV2605 SCL --> ARD A5 (Mega D21, Micro D3)
 * DRV2605 SDA --> ARD A4 (Mega D20, Micro D2)
 * 
 * Button1 --> ARD D5
 * Button2 --> ARD D8
 * Button3 --> ARD D10
 */
 
//*************************** VARIABLES **************************************//

Adafruit_DRV2605 drv;

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

//single-click(strong,medium), double-click(strong,medium), ramp-up(short,medium,long), ramp-down(short,medium,long)
effect EFFECT[10] = {{5,"single-click sharp"},{25,"single-tick sharp"},
                     {35,"double-tick sharp"},{36,"double-click sharp"},
                     {87,"ramp-up short"},{85,"ramp-up medium"},{83,"ramp-up long"},
                     {75,"ramp-down short"},{73,"ramp-down medium"},{71,"ramp-down long"}};

int index = 0;


//*************************** SETUP **************************************//
void setup() 
{
  Serial.begin(9600);
  Serial.println(F("PRECISION MICRODRIVES - SCREEN_HAPTICS_basic DEMO"));

  //init Buttons
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);
  pinMode(pinButton3, INPUT);
  
  //init DRV2605
  drv.begin();

} //end:SETUP

//*************************** LOOP **************************************//
void loop() 
{
  //============= DEMO CODE =======================
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
    Serial.println(index);
    PrintEffect(EFFECT[index]);
    PlayEffect(EFFECT[index].value);
  }

  //Button2 is used to increment effect
  if(Button2 != prevButton2){
    if(Button2)
    {
     index++;
     if(index>9)
      index=9; 
     Serial.println(index);
    }
    delay(50);
  }
  prevButton2 = Button2;

  //Button3 is used to decrement effect
  if(Button3 != prevButton3){
    if(Button3)
    {
      index--;
      if(index<0)
        index=0;
      Serial.println(index);
    }
    delay(50);
  }
  prevButton3 = Button3;

} //end:LOOP


//*************************** FUNCTIONS **************************************//

void PrintEffect(effect &EFFECT)
{
  Serial.print(EFFECT.value);
  Serial.print(F(": "));
  Serial.println(EFFECT.id);
}

void PlayEffect(int effect)
{
    //set the effect to play
    drv.setWaveform(0, effect);  // play effect 
    drv.setWaveform(1, 0);       // end waveform
    //play the effect!
    drv.go();
    delay(1000);
}

void demo()
{
  for(int i = 0; i < sizeof(EFFECT)/sizeof(effect); i++)
  {
    PrintEffect(EFFECT[i]);
    PlayEffect(EFFECT[i].value);
    delay(1000);
  }
}
