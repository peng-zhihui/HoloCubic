#include "imu.h"


void IMU::init()
{
	Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
	Wire.setClock(400000);
	while (!imu.testConnection());
	imu.initialize();
}

void IMU::update()
{
	imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}

int16_t IMU::getAccelX()
{
	return ax;
}

int16_t IMU::getAccelY()
{
	return ay;
}

int16_t IMU::getAccelZ()
{
	return az;
}

int16_t IMU::getGyroX()
{
	return gx;
}

int16_t IMU::getGyroY()
{
	return gy;
}

int16_t IMU::getGyroZ()
{
	return gz;
}
