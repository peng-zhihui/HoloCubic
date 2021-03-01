#include "display.h"
#include "imu.h"
#include "ambient.h"
#include "network.h"
#include "sd_card.h"
#include "rgb_led.h"
#include "lv_port_indev.h"
#include "lv_port_fatfs.h"
#include "lv_cubic_gui.h"

#include "lv_demo_encoder.h"


/*** Component objects ***/
Display screen;
IMU mpu;
Pixel rgb;
SdCard tf;
Network wifi;
//Ambient ambLight;


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
	rgb.setBrightness(0.1).setRGB(0, 0, 122, 204).setRGB(1, 0, 122, 204);

	/*** Init micro SD-Card ***/
	tf.init();
	lv_fs_if_init();

	String ssid = tf.readFileLine("/wifi.txt", 1);		// line-1 for WiFi ssid
	String password = tf.readFileLine("/wifi.txt", 2);	// line-2 for WiFi password

	/*** Init ambient-light sensor ***/
	//ambLight.init(ONE_TIME_H_RESOLUTION_MODE);

	/*** Inflate GUI objects ***/
	//lv_demo_benchmark();
	//lv_demo_encoder();
	//tf.readBinFromSd("/ali.bin", (uint8_t*)screen_buffer.data);
	//tf.writeBinToSd("/ali.bin", (uint8_t*)screen_buffer.data + 64);
	lv_holo_cubic_gui();
	//lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 3000, false);


	/*** Read WiFi info from SD-Card, then scan & connect WiFi ***/
#if 1
	wifi.init(ssid, password);

	// Change to your BiliBili UID ¡ý
	Serial.println(wifi.getBilibiliFans("20259914"));
#endif
}


void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	// 200 means update IMU data every 200ms
	mpu.update(200);

	delay(10);
}
