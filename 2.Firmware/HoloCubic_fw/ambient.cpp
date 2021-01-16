#include "ambient.h"


void Ambient::init(int mode)
{
	mMode = mode;
	Wire.begin(AMB_I2C_SDA, AMB_I2C_SCL);
}

unsigned int Ambient::getLux()
{
	Wire.beginTransmission(ADDRESS_BH1750FVI); //"notify" the matching device
	Wire.write(ONE_TIME_L_RESOLUTION_MODE);     //set operation mode
	Wire.endTransmission();

	switch (mMode)
	{
	case ONE_TIME_H_RESOLUTION_MODE:
		delay(125);
		break;
	case ONE_TIME_H_RESOLUTION_MODE2:
		delay(125);
		break;
	case ONE_TIME_L_RESOLUTION_MODE:
		delay(20);
		break;
	}

	Wire.requestFrom(ADDRESS_BH1750FVI, 2); //ask Arduino to read back 2 bytes from the sensor
	highByte = Wire.read();  // get the high byte
	lowByte = Wire.read(); // get the low byte

	sensorOut = (highByte << 8) | lowByte;
	illuminance = sensorOut / 1.2;

	return illuminance;
}

