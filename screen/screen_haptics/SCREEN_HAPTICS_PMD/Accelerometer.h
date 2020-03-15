/*
* Accelerometer class Arduino library for Precision Motordrives
* Tristan Cool - November, 2019
*/
#ifndef Accelerometer_h
#define Accelerometer_h

#include <inttypes.h>

class Accelerometer{
		
	public:
	//Constructors:
		Accelerometer(); //default
		Accelerometer(int x_threshold,int y_threshold, int z_threshold, float X_gain,float Y_gain,float Z_gain);

	
	//Variables:
	bool run;
	//X-axis
		uint16_t X_read;      //10 bit accelerometer signal (raw)
		float X_G;            //Accelerometer axis in G (gain)
		float X_max;          //Max.G value
	//Y-axis
		uint16_t Y_read;      //10 bit accelerometer signal (raw)
		float Y_G;            //Accelerometer axis in G (gain)
		float Y_max;          //Max.G value
	//Z-axis
		uint16_t Z_read;      //10 bit accelerometer signal (raw)
		float Z_G;            //Accelerometer axis in G (gain)
		float Z_max;          //Max.G value

  //Data
    float vector_sum;     //Vector sum of x,y,z values
    float average;        //Avergae axis G value
    float std_dev;        //Standard Deviation

				
	//Functions:
	void init(uint8_t X, uint8_t Y, uint8_t Z);
	void read_data();
	void auto_calibrate();
	void manual_calibrate(int x_bias, int y_bias, int z_bias);
	void print_raw();
	void print_accel();
	void print_max();
  void analyse();
  void start(bool run);
	
		
	private:
	//Pins:
		uint8_t pinX;
		uint8_t pinY;
		uint8_t pinZ;
		
	//Variables:
	//X-axis
		int X_th = 183;         	//Accelerometer threshold (noise)
		float X_gain = 0.0303;      //Accelerometer calibration factor (~80mV/G)
		int X_bias = 6;           	//Accelerometer calibrate (bias)
	//Y-axis
		int Y_th = 182;         	//Accelerometer threshold (noise)
		float Y_gain = 0.0303;      //Accelerometer calibration factor (~80mV/G)
		int Y_bias = 6;           	//Accelerometer calibrate (bias)
	//Z-axis
		int Z_th = 198;         	//Accelerometer threshold (noise)
		float Z_gain = 0.0303;      //Accelerometer calibration factor (~80mV/G)
		int Z_bias = 6;           	//Accelerometer calibrate (bias)
		
};

#endif
