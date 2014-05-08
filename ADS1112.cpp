#include "ADS1112.h"
#include <Wire.h>

/*
 * ADS1112 Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 5/2012
 */

void ADS1112::init(byte A0, byte A1)
{
	if (A0 == L && A1 == L) I2C_ADDRESS = B1001000;
	else if (A0 == L && A1 == F) I2C_ADDRESS = B1001001;
	else if (A0 == L && A1 == H) I2C_ADDRESS = B1001010;
	else if (A0 == H && A1 == L) I2C_ADDRESS = B1001100;
	else if (A0 == H && A1 == F) I2C_ADDRESS = B1001101;
	else if (A0 == H && A1 == H) I2C_ADDRESS = B1001110;
	else if (A0 == F && A1 == L) I2C_ADDRESS = B1001011;
	else if (A0 == F && A1 == H) I2C_ADDRESS = B1001111;
}

void ADS1112::selectChannel(byte channel, byte gain, byte mode)
{
    byte INP1 = 0, INP0 = 0;

    if (mode == MODE_UNIPOLAR) {
		if (channel == CHANNEL_0) {
			INP1 = 1;
			INP0 = 0;
		} else if (channel == CHANNEL_1) {
			INP1 = 1;
			INP0 = 1;
		} else if (channel == CHANNEL_2) {
			INP1 = 0;
			INP0 = 1;
		}				
    } else { //bipolar
		if (channel == CHANNEL_0) {
			INP1 = 0;
			INP0 = 0;
		} else if (channel == CHANNEL_1) {
			INP1 = 0;
			INP0 = 1;
		}		
	}

    //configuration register, assuming 16 bit resolution
	byte reg = 1 << BIT_ST_DRDY |
			INP1 << BIT_INP1 |
			INP0 << BIT_INP0 |
			1 << BIT_DR1 |
			1 << BIT_DR0 |
			gain;

	Wire.beginTransmission(I2C_ADDRESS);
	Wire.write(reg);
	Wire.endTransmission();			   
}

double ADS1112::readADC()
{
  	Wire.requestFrom(I2C_ADDRESS, (byte) 3);
  	
	int h = Wire.read();
  	int l = Wire.read();
  	int r = Wire.read();
  
  	long t = h << 8 |  l;
  	
	if (t >= 32768) t -= 65536l;
  	
	double v = (double) t * 2.048/32768.0;

	return v;
}
