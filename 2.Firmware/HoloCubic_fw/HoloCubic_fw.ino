#include "display.h"
#include "imu.h"
#include "rgb_led.h"
#include "ambient.h"
#include "lv_port_indev.h"
#include "lv_demo_encoder.h"

Display screen;
IMU mpu;
Pixel rgb;
Ambient ambLight;

void setup()
{
	Serial.begin(115200);

	screen.init();
	screen.setBackLight(0.2);
	lv_port_indev_init();


	mpu.init();

	rgb.init();
	rgb.setBrightness(0.1).setRGB(0, 122, 204);

	ambLight.init(ONE_TIME_L_RESOLUTION_MODE);

	//lv_demo_benchmark();
	lv_demo_encoder();
}

int i = 0;
void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	mpu.update(200);

	rgb.setBrightness(ambLight.getLux() / 500.0);
	//Serial.println(ambLight.getLux());

	delay(10);
}
