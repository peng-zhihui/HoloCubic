#include "display.h"
#include "imu.h"
#include "rgb_led.h"
#include "ambient.h"
#include <lv_examples/lv_examples.h>

Display screen;
IMU mpu;
Pixel rgb;
Ambient ambLight;




void setup()
{
	Serial.begin(115200);

	screen.init();
	screen.setBackLight(0.2);

	mpu.init();

	rgb.init();
	rgb.setBrightness(0.3).setRGB(0, 122, 204);

	ambLight.init(ONE_TIME_L_RESOLUTION_MODE);

	lv_demo_benchmark();
}

int i = 0;
void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	mpu.update();

	rgb.setBrightness(ambLight.getLux() / 500.0);
	Serial.println(ambLight.getLux());

	delay(10);
}
