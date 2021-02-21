#include "display.h"
#include "imu.h"
#include "ambient.h"
#include "network.h"
#include "sd_card.h"
#include "rgb_led.h"
#include "lv_port_indev.h"
#include "lv_cubic_gui.h"
//#include "lv_bilifens.h"
#include "lv_demo_encoder.h"


/*** Component objects ***/
Display screen;
IMU mpu;
Pixel rgb;
Ambient ambLight;
SdCard tf;
Network wifi;

int fans;
int tian;
long time1;

String  appid = "55551439";		     //天气API的APPID	  官网地址：https://www.tianqiapi.com
String  appsecret = "NEk0V71b";		 //天气API的APPSecret
String  cityid = "101280501";		 //天气API的城市ID  
String  biliid = "20259914";		 //Bilibili的ID
String  cityname = "Shantou";		 //城市名

void setup()
{
	Serial.begin(9600);

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
	//String cityname = tf.readFileLine("/wifi.txt", 3);	// line-3 for Cityname

	/*** Inflate GUI objects ***/
	//lv_demo_benchmark();
	//lv_demo_encoder();
	//tf.readBinFromSd("/ali.bin", (uint8_t*)screen_buffer.data);
	//tf.writeBinToSd("/ali.bin", (uint8_t*)screen_buffer.data + 64);
	

	/*** Read WiFi info in SD-Card, then scan & connect WiFi ***/
#if 1
	wifi.init(ssid, password);
	fans = wifi.getBilibiliFans("http://api.bilibili.com/x/relation/stat?vmid="+biliid);
	Serial.println(fans);
	tian = wifi.getTianqi("https://www.tianqiapi.com/api?version=v6&appid="+appid+"&appsecret="+appsecret+"&cityid="+cityid);
	Serial.println(tian);
#endif

	char fengs[8];
	char tiangqi[8];
	char cityname1[10];
	itoa(fans, fengs, 10);
	itoa(tian, tiangqi, 10);
	const char* fens = fengs;
	const char* tianq = tiangqi;
	const char* cityname2 = strcpy(cityname1,cityname.c_str());

	lv_holo_cubic_gui(tianq,fens,cityname2);
	//lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 3000, false);

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

int page = 0;

long previousMillis = 0;

long interval = 1800000;

void loop()
{
	// run this as often as possible ↓
	screen.routine();

	mpu.update(200);

	rgb.setBrightness(ambLight.getLux() / 500.0);
	//Serial.println(ambLight.getLux());

	delay(10);

	//if (Serial.available() > 0)
	//{
	//	char c = Serial.read();
	//	//lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_FADE_ON, 300, 500, false);
	//}

	if (tmp == 1 & page == 0)
	{
		lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 500, false);
		page = 1;
		tmp = 0;
	}
	if(tmp == 1 & page == 1)
	{
		lv_scr_load_anim(scr3, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 500, false);
		page = 2;
		tmp = 0;
	}
	if(tmp ==1 & page == 2)
	{
		lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 500, false);
		page = 0;
		tmp = 0;
	}
	if (tmp == 2 & page == 0)
	{
		lv_scr_load_anim(scr3, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 500, false);
		page = 2;
		tmp = 0;
	}
	if (tmp == 2 & page == 1)
	{
		lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 500, false);
		page = 0;
		tmp = 0;
	}
	if(tmp == 2 & page == 2)
	{
		lv_scr_load_anim(scr2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 500, false);
		page = 1;
		tmp = 0;
	}


	if (millis() - time1 > 5000 && !flag)
	{
		flag = 1;
		lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_NONE, 10, 0, false);
	}

	//if (millis() - previousMillis >= interval) 
	//{
	//	previousMillis = millis();
	//}


	delay(500);
}
