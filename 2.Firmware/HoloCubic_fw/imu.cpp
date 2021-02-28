#include "imu.h"

void IMU::init()
{
	Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);
	Wire.setClock(400000);
	while (!imu.testConnection());
	imu.initialize();
}

void IMU::update(int interval)
{
	imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	//Serial.print(gx);
	//Serial.print(" ");
	//Serial.print(gy);
	//Serial.print(" ");
	//Serial.print(gz);
	//Serial.println(" ");

	if (millis() - last_update_time > interval)
	{
		if (ay > 3000 && flag)
		{
			encoder_diff--;
			flag = 0;
		}
		else if (ay < -3000 && flag)
		{
			encoder_diff++;
			flag = 0;
		}
		else
		{
			flag = 1;
		}

		if (ax > 10000)
		{
			encoder_state = LV_INDEV_STATE_PR;
		}
		else
		{
			encoder_state = LV_INDEV_STATE_REL;
		}

		last_update_time = millis();
	}
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
