#include "display.h"
#include "imu.h"
#include "ambient.h"
#include "network.h"
#include "sd_card.h"
#include "rgb_led.h"
#include "lv_port_indev.h"
#include "lv_cubic_gui.h"

#include "lv_demo_encoder.h"


/*** Component objects ***/
Display screen;
IMU mpu;
Pixel rgb;
Ambient ambLight;
SdCard tf;
Network wifi;

void setup()
{
	Serial.begin(115200);

	/*** Init screen ***/
	screen.init();
	screen.setBackLight(0.2);

	/*** Init IMU as input device ***/
	lv_port_indev_init();
	mpu.init();

	/*** Init on-board RGB ***/
	rgb.init();
	rgb.setBrightness(0.1).setRGB(0, 122, 204);

	/*** Init ambient-light sensor ***/
	ambLight.init(ONE_TIME_H_RESOLUTION_MODE);

	/*** Init micro SD-Card ***/
	tf.init();
	String ssid = tf.readFileLine("/wifi.txt", 1);		// line-1 for WiFi ssid
	String password = tf.readFileLine("/wifi.txt", 2);	// line-2 for WiFi password

	/*** Inflate GUI objects ***/
	//lv_demo_benchmark();
	//lv_demo_encoder();
	//tf.readBinFromSd("/ali.bin", (uint8_t*)screen_buffer.data);
	//tf.writeBinToSd("/ali.bin", (uint8_t*)screen_buffer.data + 64);
	lv_holo_cubic_gui();


	/*** Read WiFi info in SD-Card, then scan & connect WiFi ***/
#if 1
	wifi.init(ssid, password);

	Serial.println(wifi.getBilibiliFans("http://api.bilibili.com/x/relation/stat?vmid=20259914"));
#endif



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


	//Serial.println(sizeof(screen_buffer));
}


void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	mpu.update(200);

	rgb.setBrightness(ambLight.getLux() / 500.0);
	Serial.println(ambLight.getLux());

	delay(10);
}
