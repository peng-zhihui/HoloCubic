#include "display.h"
#include "imu.h"
#include <lv_examples/lv_examples.h>

Display screen;

// MPU6050 accelerator and gyroscope
IMU mpu;

void setup()
{
	Serial.begin(115200);

	screen.init();
	screen.setBackLight(0.2);

	mpu.init();


	lv_demo_benchmark();
}


void loop()
{
	// run this as often as possible ¡ý
	screen.runtime();

	mpu.update();


	delay(5);
}
