#include "Accelerometer.h"

#include <stdio.h>
#include <string.h>
#include "Arduino.h"

Accelerometer::Accelerometer()
{
}

Accelerometer::Accelerometer(int x_threshold,int y_threshold, int z_threshold, float x_gain,float y_gain,float z_gain)
{
	X_th = x_threshold;
	Y_th = y_threshold;
	Z_th = z_threshold;
	X_gain = x_gain;
	Y_gain = y_gain;
	Z_gain = z_gain;
}

void Accelerometer::init(uint8_t X, uint8_t Y, uint8_t Z)
{
	pinX = X;
	pinY = Y;
	pinZ = Z;

  pinMode(pinX,INPUT);
  pinMode(pinY,INPUT);
  pinMode(pinZ,INPUT);

  Serial.println(F("Accelerometer.....initialised."));
}

void Accelerometer::read_data()
{
	X_read = analogRead(pinX);
	Y_read = analogRead(pinY);
	Z_read = analogRead(pinZ);

	X_G = abs((X_read*X_gain)-X_bias);
	Y_G = abs((Y_read*Y_gain)-Y_bias);
	Z_G = abs((Z_read*Z_gain)-Z_bias);

	if(X_G > X_max)
		X_max = X_G;
	if(Y_G > Y_max)
		Y_max = Y_G;
	if(Z_G > Z_max)
		Z_max = Z_G;
}

void Accelerometer::auto_calibrate()
{
	read_data();
  //set threshold to base values + small tolerance
  X_th = X_read;
  Y_th = Y_read;
  Z_th = Z_read;
  //(DC)bias is initial reading with gain (in G)
	X_bias = X_read*X_gain;
	Y_bias = Y_read*Y_gain;
	Z_bias = Z_read*Z_gain;
	Serial.println(F("*** AUTO-CALIBRATION X-Y-Z (Bias G) ***"));
	Serial.print(X_bias);
	Serial.print(F("G ; "));
	Serial.print(Y_bias);
	Serial.print(F("G ; "));
	Serial.print(Z_bias);
	Serial.println(F("G"));
	Serial.println(F("Accelerometer.....calibrated."));
}

void Accelerometer::manual_calibrate(int x_bias, int y_bias, int z_bias)
{
	X_bias = x_bias;
	Y_bias = y_bias;
	Z_bias = z_bias;
	Serial.println(F("*** CALIBRATION X-Y-Z (Bias G) ***"));
	Serial.print(X_bias);
	Serial.print(F("G ; "));
	Serial.print(Y_bias);
	Serial.print(F("G ; "));
	Serial.print(Z_bias);
	Serial.println(F("G"));
	Serial.println(F("Accelerometer.....calibrated."));
}

void Accelerometer::print_raw()
{
	//Serial.println(F("*** ACCEL. X-Y-Z (raw)   ***"));
	Serial.print(X_read);
	Serial.print(F("   ; "));
	Serial.print(Y_read);
	Serial.print(F("   ; "));
	Serial.println(Z_read);
}

void Accelerometer::print_accel()
{
	//Serial.println(F("***** ACCEL. X-Y-Z (G)     *****"));
	Serial.print(X_G);
	Serial.print(F("G ; "));
	Serial.print(Y_G);
	Serial.print(F("G ; "));
	Serial.print(Z_G);
	Serial.println(F("G"));
}

void Accelerometer::print_max()
{
	Serial.println(F("***** ACCEL. X-Y-Z (Max. G) *****"));
	Serial.print(X_max);
	Serial.print(F("G ; "));
	Serial.print(Y_max);
	Serial.print(F("G ; "));
	Serial.print(Z_max);
	Serial.println(F("G"));
}

void Accelerometer::analyse()
{
  vector_sum = X_max + Y_max + Z_max;
  average = vector_sum / 3;
  float variance = (X_max*X_max + Y_max*Y_max + Z_max*Z_max)/3;
  std_dev = sqrt(variance);
  
  Serial.println(F("***** ACCEL. Analyse       *****"));
  Serial.print(F("* Vector sum (G): "));
  Serial.println(vector_sum);
  Serial.print(F("* Axis average (G): "));
  Serial.println(average);
  Serial.print(F("* Standard Deviation: "));
  Serial.println(std_dev);

  bool one_std_dev;
  if((X_max*1.05 > average-std_dev) && (X_max*0.95 < average+std_dev))
  {
    if((Y_max*1.05 > average-std_dev) && (Y_max*0.95 < average+std_dev))
    {
      if((Z_max*1.05 > average-std_dev) && (Z_max*0.95 < average+std_dev))
      {
        one_std_dev = true;
      }
      else
        one_std_dev = false;
    }
    else
      one_std_dev = false;
  }
  else
    one_std_dev = false;
  
  if(one_std_dev)
    Serial.println(F("All axes are within 1 standard deviation (5% tolerance)."));
  else
    Serial.println(F("Not all axes are within 1 standard deviation."));

}

void Accelerometer::start(bool run)
{
	if(run)
	{
		read_data();
		if(X_read > X_th || Y_read > Y_th || Z_read > Z_th)
		{
			//Serial.println(F("***** ACCEL. X-Y-Z (raw)   *****"));
			Serial.println(F("***** ACCEL. X-Y-Z (G)     *****"));
			while(X_read > X_th || Y_read > Y_th || Z_read > Z_th)
			{
				//print_raw();
				print_accel();
				read_data();
			}
			print_max();
      analyse();
		}
		X_max = Y_max = Z_max = 0;
	}
}
