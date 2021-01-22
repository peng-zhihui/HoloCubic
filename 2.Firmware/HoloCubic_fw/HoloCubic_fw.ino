#include "display.h"
#include "imu.h"
#include "rgb_led.h"
#include "ambient.h"
#include "sd_card.h"

#include "lv_port_indev.h"
#include "lv_demo_encoder.h"
#include "lv_cubic_gui.h"


Display screen;
IMU mpu;
Pixel rgb;
Ambient ambLight;
SdCard tf;

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

	tf.init();

	//lv_demo_benchmark();
	lv_demo_encoder();


	Serial.print(tf.readFileLine("/wifi.txt", 1) + "\n");

	Serial.print(tf.readFileLine("/wifi.txt", 2));

	/*tf.listDir("/", 0);
	tf.createDir("/mydir");
	tf.listDir("/", 0);
	tf.removeDir("/mydir");
	tf.listDir("/", 2);
	tf.writeFile("/hello.txt", "Hello ");
	tf.appendFile("/hello.txt", "World!\n");
	tf.readFile("/hello.txt");
	tf.deleteFile("/foo.txt");
	tf.renameFile("/hello.txt", "/foo.txt");
	tf.readFile("/foo.txt");
	tf.fileIO("/test.txt");
	Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
	Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));*/
}


void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	mpu.update(200);

	rgb.setBrightness(ambLight.getLux() / 500.0);
	//Serial.println(ambLight.getLux());

	delay(10);
}
