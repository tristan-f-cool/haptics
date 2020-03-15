// Precision Microdrives Haptic Feedback Evaluation Kit
// Haptic Touchscreen Example Application (Arduino Mega)
// REFERENCEE: https://www.precisionmicrodrives.com/haptic-feedback/haptic-feedback-example-applications-haptic-touchscreen/

//*************************** WIRING  **************************************//
/* PMD Haptic Feedback Evaluation Kit (HFEK) in Developer Mode
 * UTFT screen pins 6, 5, 4, 3, 2
 * UTFT touch pins  9, 11, 12, 13
 */
 
#include <tinyFAT.h>
#include <UTFT.h>
#include <UTFT_tinyFAT.h>
#include <UTouch.h>

extern uint8_t SmallFont[];

UTFT myGLCD(SSD1289, 38, 39, 40, 41);
UTFT_tinyFAT myFiles(&myGLCD);
UTouch myTouch(6, 5, 4, 3, 2);

boolean display_rendertime = false;
boolean display_filename = false;

word res;
long sm, em;

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.fillScr(255,255,255);
  
  file.initFAT();
  
  myGLCD.setBackColor(255,255,255);
  myGLCD.setFont(SmallFont);
  
  myTouch.InitTouch(PORTRAIT);
  myTouch.setPrecision(PREC_MEDIUM);  
  
  myFiles.loadBitmap(12,10,220,293,"haptics.raw");
  delay(3000);
  myGLCD.fillScr(255,255,255);
  
  myFiles.loadBitmap(0,6,240,47,"PMD.raw");
  delay(1000);

  myFiles.loadBitmap(25,62,60,60,"power.raw");
  myFiles.loadBitmap(145,62,60,60,"reset.raw");
  myFiles.loadBitmap(25,127,60,60,"yes.raw");
  myFiles.loadBitmap(145,127,60,60,"no.raw");
  myFiles.loadBitmap(85,188,60,60,"up.raw");
  myFiles.loadBitmap(5,227,60,60,"left.raw");
  myFiles.loadBitmap(165,227,60,60,"right.raw");
  myFiles.loadBitmap(85,260,60,60,"down.raw");
  
  myGLCD.setColor(VGA_RED);
  
  Serial.begin(9600);
}

void loop()
{
  long x, y, z;
  
  while (myTouch.dataAvailable() == true)
  {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if ((x!=-1) and (y!=-1))
    {     
                                                               // 1st Row
      if (62 < y && y < 122)
      {
        if (25 < x && x < 85)                                  // Power
        {
          digitalWrite(13, HIGH);
          myGLCD.fillCircle(55,92,30);
          myFiles.loadBitmap(25,62,60,60,"power.raw");
          delay(50);
          digitalWrite(13, LOW);
          Serial.println("Digital Pin 13");  
        }
        
        else if (145 < x && x < 205)                           // Reset
        {
          digitalWrite(12, HIGH);
          myGLCD.fillCircle(175,92,30);    
          myFiles.loadBitmap(145,62,60,60,"reset.raw");
          delay(50);
          digitalWrite(12, LOW);
          Serial.println("Digital Pin 12");  
        }
        
        else
        {
          z++;
        }
      }
      
                                                              // 2nd Row
      if (127 < y && y < 187)
      {
        if (25 < x && x < 85)                                 // Yes
        {
          digitalWrite(11, HIGH);
          myGLCD.fillCircle(55,157,30);    
          myFiles.loadBitmap(25,127,60,60,"yes.raw");
          delay(50);
          digitalWrite(11, LOW);
          Serial.println("Digital Pin 11");  
        }
        
        else if (145 < x && x < 205)                           // No
        {
          digitalWrite(11, HIGH);
          myGLCD.fillCircle(175,157,30);     
          myFiles.loadBitmap(145,127,60,60,"no.raw");
          delay(50);
          digitalWrite(11, LOW);
          Serial.println("Digital Pin 11");  
        }
        
        else
        {
          z++;
        }
      }
      
                                                              // 3rd Row
      if (188 < y && y < 248)
      {
        if (85 < x && x < 145)                                // Up
        {
          digitalWrite(9, HIGH);
          myGLCD.fillCircle(115,218,30);
          myFiles.loadBitmap(85,188,60,60,"up.raw");
          delay(50);
          digitalWrite(9, LOW);
          Serial.println("Digital Pin 9");  
        }
        
        else
        {
          z++;
        }
      }
      
                                                              // 4th Row
      if (227 < y && y < 287)
      {
        if (5 < x && x < 65)                                  // Left
        {
          digitalWrite(9, HIGH);
          myGLCD.fillCircle(35,257,30);       
          myFiles.loadBitmap(5,227,60,60,"left.raw");
          delay(50);
          digitalWrite(9, LOW);
          Serial.println("Digital Pin 9");  
        }
        
        else if (165 < x && x < 225)                          // Right
        {
          digitalWrite(9, HIGH);
          myGLCD.fillCircle(195,257,30);
          myFiles.loadBitmap(165,227,60,60,"right.raw");
          delay(50);
          digitalWrite(9, LOW);
          Serial.println("Digital Pin 9");  
        }
        
        else
        {
          z++;
        }
      }
      
                                                              // 5th Row
      if (260 < y && y < 320)
      {
        if (85 < x && x < 145)                                // Down
        {
          digitalWrite(9, HIGH);
          myGLCD.fillCircle(115,290,30);
          myFiles.loadBitmap(85,260,60,60,"down.raw");
          delay(50);
          digitalWrite(9, LOW);
          Serial.println("Digital Pin 9");  
        }
               
        else
        {
          z++;
        }
      }
      
      else
      {
        z++;
      }
      
      delay(200);
      digitalWrite(13, LOW);

    }
  }
}
