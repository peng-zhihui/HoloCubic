#ifndef AMBIENT_H
#define AMBIENT_H

#include <Wire.h>

#define AMB_I2C_SDA 32 
#define AMB_I2C_SCL 33

#define ADDRESS_BH1750FVI 0x23    //ADDR="L" for this module
#define ONE_TIME_H_RESOLUTION_MODE 0x20 // 1lux for 120ms
#define ONE_TIME_H_RESOLUTION_MODE2 0x21 // 0.5lux for 120ms
#define ONE_TIME_L_RESOLUTION_MODE 0x23 // 4lux for 16ms

class Ambient
{
private:
	int mMode;
	unsigned char highByte = 0;
	unsigned char lowByte = 0;
	unsigned int sensorOut = 0;
	unsigned int illuminance = 0;

	unsigned int lux[5];
	long sample_time = 125;
	long last_time;

public:
	void init(int mode);
	unsigned int getLux();
};

#endif