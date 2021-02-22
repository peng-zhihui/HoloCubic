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

long time1;

void setup()
{
	Serial.begin(115200);

	/*** Init screen ***/
	screen.init();
	screen.setBackLight(0.8);

	/*** Init IMU as input device ***/
	lv_port_indev_init();
	mpu.init();

	/*** Init on-board RGB ***/
	rgb.init();
	rgb.setBrightness(0.1).setRGB(0, 0, 0);// (0, 122, 204);

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
	//lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_FADE_ON, 300, 5000, false);


	/*** Read WiFi info in SD-Card, then scan & connect WiFi ***/
#if 0
	wifi.init("ssid", "password");

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

	time1 = millis();

}

int flag = 0;

float fade = 0;

extern int tmp;
void loop()
{
	// run this as often as possible ¡ý
	screen.routine();

	mpu.update(200);

	rgb.setBrightness(ambLight.getLux() / 500.0);
	Serial.println(ambLight.getLux());

	delay(10);

	//if (Serial.available() > 0)
	//{
	//	char c = Serial.read();
	//	lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_FADE_ON, 300, 500, false);
	//}

	//if (tmp == 1)
	//{
	//	lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 500, false);
	//	tmp = 0;
	//}
	//if (tmp == 2)
	//{
	//	lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 500, false);
	//	tmp = 0;
	//}


	//if (millis() - time1 > 5000 && !flag)
	//{
	//	flag = 1;
	//	//lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, false);
	//lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_NONE, 10, 0, false);
	//}


	//if (millis() - time1 > 2000)
	//{
	//	lv_label_set_text_fmt(label1, "CPU Temp: %d\%", (int)random(35, 50));
	//	lv_label_set_text_fmt(label2, "CPU Usage: %d\%", (int)random(20, 25));
	//	lv_label_set_text_fmt(label3, "Mem Usage: %dMB", 12538);
	//	lv_label_set_text_fmt(label4, "Net Upload: %dKB/s", (int)random(0, 100));
	//	lv_label_set_text_fmt(label5, "Net Download: %dKB/s", (int)random(35, 50));

	//	time1 = millis();
	//}


	//while (true)
	//{
	//	while (true)
	//	{
	//		if (fade < 1)
	//		{
	//			fade += 0.005;
	//			screen.setBackLight(fade);
	//		}
	//		else
	//			break;
	//		delay(10);
	//	}

	//	delay(5000);
	//	fade = 0;
	//	screen.setBackLight(fade);
	//	delay(5000);
	//}

}
