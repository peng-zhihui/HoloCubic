#include "display.h"
#include "imu.h"
#include "rgb_led.h"
#include <lv_examples/lv_examples.h>

Display screen;
IMU mpu;
Pixel rgb;

void setup()
{
	Serial.begin(115200);

	screen.init();
	screen.setBackLight(0.2);

	mpu.init();

	rgb.init();
	rgb.setBrightness(0.3).setRGB(0, 122, 204);

	lv_demo_benchmark();
}

int i = 0;
void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	mpu.update();

	delay(10);
}
