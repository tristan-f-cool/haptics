#include "DRV2605.h"
#include "i2c.h"
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "defs.h"
#include "DRV2605_defs.h"

DRV2605::DRV2605()
{
	TWCR |= (1<< TWEN );	// Enable TWI
}

void DRV2605::init()
{
	return;
	// First time init
	I2C_Write( ADDR_MODE,			MODE_ACTIVE );	// Exit standby
	I2C_Write( ADDR_RTP_INPUT,		0x00 );
	I2C_Write( ADDR_WAV_SEQ1,		0x01 );
	I2C_Write( ADDR_WAV_SEQ2,		0x00 );
	I2C_Write( ADDR_ODT,			0x00 );
	I2C_Write( ADDR_SPT,			0x00 );
	I2C_Write( ADDR_SNT,			0x00 );
	I2C_Write( ADDR_BRT,			0x00 );
	I2C_Write( ADDR_A2H_MAX_IN,		0x64 );
}

bool DRV2605::autoCal( uint8_t ratedVoltage, uint8_t overdriveClamp, boolean LRA, uint8_t* compensation, uint8_t* backEMF, uint8_t* feedback )
{
	// Set defaults
	setDefaults();

	// Work out control registers
	uint8_t fb = FB_BRAKE_4x | FB_LOOP_FAST;
	uint8_t control1 = DEFAULT_CTRL1;

	if( LRA )
	{
		fb |= FB_MODE_LRA;
	}
	else
	{
		fb |= FB_MODE_ERM;
	}

	// Write required registers
	I2C_Write( ADDR_RATED_VOLT,		ratedVoltage );
	I2C_Write( ADDR_OD_CLAMP,		overdriveClamp );
	I2C_Write( ADDR_FEEDBACK,		fb );
	I2C_Write( ADDR_CTRL1,			control1 );
	I2C_Write( ADDR_MODE,			MODE_AUTOCAL );

	fb = GO;
	control1 = 0;
	I2C_Write( ADDR_GO,			GO );

	do
	{
		I2C_Read( ADDR_GO, &fb );
		delay( 100 ); 
		++control1;
	} while( fb & GO && control1 < 100 );

	// Read status bit
	I2C_Read( ADDR_STATUS, &fb );

//	if( fb & STAT_DIAG_BAD )
//	{
//		//Results did not converge
//		Serial.print(F( "Fail: " ));
//		Serial.print( control1 );
//		Serial.print(F( " tries. Status " ));
//		Serial.println( fb, BIN );
//		return false;
//	}

	Serial.print(F( "Status " ));
	Serial.println( fb, BIN );

	I2C_Read( ADDR_AC_COMP,			compensation );
	I2C_Read( ADDR_AC_BACK_EMF,		backEMF );
	I2C_Read( ADDR_FEEDBACK,		feedback );
	return true;
}

void DRV2605::playFullHaptic( uint8_t library, uint8_t effect, uint8_t ratedVoltage, uint8_t overdriveClamp, uint8_t compensation, uint8_t backEMF, uint8_t feedback )
{
	// Set defaults
	setDefaults();

	// Work out control registers
	uint8_t fb = FB_BRAKE_4x | FB_LOOP_FAST | (6 == library ? FB_MODE_LRA : FB_MODE_ERM);	// Library 6 means LRA
	uint8_t control1 = DEFAULT_CTRL1;
	uint8_t control2 = DEFAULT_CTRL2;
	uint8_t control3 = DEFAULT_CTRL3;

	// Set saved BEMF Gain
	fb |= ( feedback & FB_BEMF_BITMASK );

	// Set open or closed loop based on library
	if( 1 == library )
		control3 |= CTRL3_ERM_OPEN;	// Set bit 5 for open loop operation
	else
		control3 &= ~CTRL3_ERM_OPEN;	// Clear bit 5 for closed loop operation

	// Protect I2C transaction from MPR121 interrupt.
	noInterrupts();
	i2cSendStart();
	i2cSendByte( DRV2605_ADDR_WR );	        // write 0xB4
	i2cSendByte( 0x16 );			// write first register address
	i2cSendByte( ratedVoltage );
	i2cSendByte( overdriveClamp );
	i2cSendByte( compensation );
	i2cSendByte( backEMF );
	i2cSendByte( fb );
	i2cSendByte( control1 );
	i2cSendByte( control2 );
	i2cSendByte( control3 );
	i2cSendStop();
	i2cSendStart();
	i2cSendByte( DRV2605_ADDR_WR );	// write 0xB4
	i2cSendByte( 0x03 );			// write first register address
	i2cSendByte( library );
	i2cSendByte( effect );
	i2cSendByte( MODE_ACTIVE );
	i2cSendStop();
	interrupts();

	I2C_Write( ADDR_MODE, MODE_ACTIVE );     //0x01

	fb = GO;
	control1 = 0;
	I2C_Write( ADDR_GO, GO );
	do
	{
		// For effect 118, we have to stop by clearing the GO bit.
		if ( 118 == effect ) 
		{
			// Let it run for 1 seconds.
			if ( 10 == control1 ) 
			{
				I2C_Write( ADDR_GO, STOP );     // Clear GO bit to stop effect.
			}

			delay( 100 );            
			++control1;
		}

		// Read GO bit to see if effect is still playing.
		I2C_Read( 0x0C, &fb );   
	} while ( fb & GO );
}



void DRV2605::Audio( uint8_t LRA_AUDIO, uint8_t effect, uint8_t ratedVoltage, uint8_t overdriveClamp, uint8_t compensation, uint8_t backEMF, uint8_t feedback )
{

	noInterrupts();                   
 
  i2cSendStart();
	i2cSendByte( DRV2605_ADDR_WR);	        // write 0xB4
	i2cSendByte( GO);                       //out of standby write 0x01 to 0x00 reg	              
  i2cSendByte( AUDIO_MODE );              //audio mode value 0x04
  i2cSendByte( MODE_INT_TRIG);            //reg adress 0x02 No Real time playback out of standby write 0x01 to 0x00 reg               
  i2cSendStop();

  i2cSendStart();
  i2cSendByte( DRV2605_ADDR_WR);          // write 0xB4
  i2cSendByte( 0x12 );                    // write first register address
  i2cSendByte( DEFAULT_A2H_MIN_IN );      // addr 0x12 AUDIO MIN INPUT LEVEL 1.8V * 32 / 255 ~225mV
  i2cSendByte( DEFAULT_A2H_MAX_IN);                    //addr 0x13 AUDIO MAX INPUT LEVEL 1.8V * 128 / 255 ~900mV 
  i2cSendByte( DEFAULT_A2H_MIN_OUT );                    //addr 0x14 AUDIO MIN OUTPUT DRIVE  reg /255 * 100%
  i2cSendByte( DEFAULT_A2H_MAX_OUT );                   //addr 0x15 AUDIO MAX OUTPUT DRIVE reg /255 * 100%
	i2cSendByte( ratedVoltage );            //addr 0x16
	i2cSendByte( overdriveClamp );          //addr 0x17
	i2cSendByte( compensation );            //addr 0x18
	i2cSendByte( backEMF );                 //addr 0x19
 	
 	if(LRA_AUDIO) 
 	  i2cSendByte( FB_MODE_LRA );           //addr 0x1A
 	else   
    i2cSendByte( FB_MODE_ERM );             //addr 0x1A

	i2cSendByte( AUDIO_CTRL1);                //addr 0x1B
	i2cSendByte( AUDIO_CTRL2 );                //addr 0x1C
	i2cSendByte( AUDIO_CTRL3 );                //addr 0x1D
	i2cSendStop();                          

	interrupts();
}


void DRV2605::setDefaults()
{ 
	// Protect I2C transaction from MPR121 interrupt.
	noInterrupts();                   
	i2cSendStart();
		i2cSendByte( DRV2605_ADDR_WR);	// write 0xB4
		i2cSendByte( 0x01);			                 // stnadby off
		i2cSendByte( DEFAULT_MODE );             //addr  0x01
		i2cSendByte( DEFAULT_RTP_INPUT );        //addr 0x02
		i2cSendByte( DEFAULT_LIBRARY );          //addr 0x03
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x04
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x05
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x06
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x07
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x08
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x09
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x0A
		i2cSendByte( DEFAULT_WAV_SEQ );          //addr 0x0B
		i2cSendByte( DEFAULT_GO );               //addr 0x0C
		i2cSendByte( DEFAULT_OD_OFFSET );        //addr 0x0D
		i2cSendByte( DEFAULT_ST_OFFSETP );       //addr 0x0E
		i2cSendByte( DEFAULT_ST_OFFSETN );       //addr 0x0F
		i2cSendByte( DEFAULT_BT_OFFSET );        //addr 0x10
		i2cSendByte( DEFAULT_A2H_CTRL );         //addr 0x11
		i2cSendByte( DEFAULT_A2H_MIN_IN );       //addr 0x12
		i2cSendByte( DEFAULT_A2H_MAX_IN );       //addr 0x13
		i2cSendByte( DEFAULT_A2H_MIN_OUT );      //addr 0x14
		i2cSendByte( DEFAULT_A2H_MAX_OUT );      //addr 0x15
		i2cSendByte( DEFAULT_RATED_VOLT );       //addr 0x16
		i2cSendByte( DEFAULT_OD_CLAMP );         //addr 0x17
		i2cSendByte( DEFAULT_AC_COMP );          //addr 0x18
		i2cSendByte( DEFAULT_AC_BACK_EMF );      //addr 0x19
		i2cSendByte( DEFAULT_FEEDBACK );         //addr 0x1A
		i2cSendByte( DEFAULT_CTRL1 );            //addr 0x1B
		i2cSendByte( DEFAULT_CTRL2 );            //addr 0x1C
		i2cSendByte( DEFAULT_CTRL3 );            //addr 0x1D
		i2cSendByte( DEFAULT_AC_MEM );           //addr 0x1E
		i2cSendByte( DEFAULT_VBAT_VOLT );        //addr 0x1F
		i2cSendByte( DEFAULT_LRA_RES );          //addr 0x20
	i2cSendStop();
	interrupts();
}

void DRV2605::I2C_Read( uint8_t address, uint8_t *data )
{
	noInterrupts(); // Protect I2C transaction from MPR121 interrupt, timer0, and usart

	i2cSendStart();                  // start
	i2cSendByte(DRV2605_ADDR_WR);	 // write 0xB4
	i2cSendByte(address);	         // write register address
	i2cSendStart();                  // repeated start
	i2cSendByte(DRV2605_ADDR_RD);	 // write 0xB5

    data[0] = i2cReceiveByte(0);    // NACK	
	i2cSendStop();
	TWCR &=~(1<<TWEN);	// Disable TWI
	TWCR |= (1<<TWEN);	// Enable TWI

	interrupts();
}

void DRV2605::I2C_Write( uint8_t address, uint8_t data )
{
	noInterrupts(); // Protect I2C transaction from MPR121 interrupt.
 	i2cSendStart();
	i2cSendByte( DRV2605_ADDR_WR );    // write 0xB4
	i2cSendByte( address );	         // write register address
	i2cSendByte( data );
	i2cSendStop();
	interrupts();
}
