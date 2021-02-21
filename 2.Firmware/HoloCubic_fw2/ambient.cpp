#include "ambient.h"


void Ambient::init(int mode)
{
	mMode = mode;
	switch (mode)
	{
	case ONE_TIME_H_RESOLUTION_MODE:
		sample_time = 125;
		break;
	case ONE_TIME_H_RESOLUTION_MODE2:
		sample_time = 125;
		break;
	case ONE_TIME_L_RESOLUTION_MODE:
		sample_time = 20;
		break;
	}

	Wire.begin(AMB_I2C_SDA, AMB_I2C_SCL);

	delay(50);

	Wire.beginTransmission(ADDRESS_BH1750FVI); //"notify" the matching device
	Wire.write(mMode);     //set operation mode
	Wire.endTransmission();
}

unsigned int Ambient::getLux()
{
	if (millis() - last_time > sample_time)
	{
		last_time = millis();
		Wire.requestFrom(ADDRESS_BH1750FVI, 2); //ask Arduino to read back 2 bytes from the sensor
		highByte = Wire.read();  // get the high byte
		lowByte = Wire.read(); // get the low byte

		sensorOut = (highByte << 8) | lowByte;
		illuminance = sensorOut / 1.2;

		for (int i = 4; i > 0; i--) lux[i] = lux[i - 1];
		lux[0] = illuminance;

		Wire.beginTransmission(ADDRESS_BH1750FVI); //"notify" the matching device
		Wire.write(mMode);     //set operation mode
		Wire.endTransmission();
	}

	unsigned int avg = 0;
	for (int i = 4; i >= 0; i--) avg += lux[i];
	avg /= 5;

	return avg;
}

