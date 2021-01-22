#include <WiFi.h>
#include <HTTPClient.h>
#include "display.h"
#include "imu.h"
#include "rgb_led.h"
#include "ambient.h"
#include "sd_card.h"
#include "lv_port_indev.h"
#include "lv_cubic_gui.h"

#include "lv_demo_encoder.h"


/*** Component objects ***/
Display screen;
IMU mpu;
Pixel rgb;
Ambient ambLight;
SdCard tf;

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
	ambLight.init(ONE_TIME_L_RESOLUTION_MODE);


	/*** Init micro SD-Card ***/
	tf.init();


	/*** Read WiFi info in SD-Card, then scan & connect WiFi ***/
	String ssid = tf.readFileLine("/wifi.txt", 1);
	String password = tf.readFileLine("/wifi.txt", 2);

	Serial.println("scan start");

	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
	{
		Serial.println("no networks found");
	}
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i)
		{
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
			delay(10);
		}
	}
	Serial.println("");
	Serial.print("Connecting: ");
	Serial.print(ssid.c_str());
	Serial.print(" @");
	Serial.println(password.c_str());

	WiFi.begin(ssid.c_str(), password.c_str());
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());


	/*** Inflate GUI objects ***/
	//lv_demo_benchmark();
	lv_demo_encoder();



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
