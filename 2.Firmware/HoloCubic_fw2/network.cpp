#include "network.h"


void Network::init(String ssid, String password)
{
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
}

unsigned int Network::getBilibiliFans(String url)
{
	String fansCount = "";
	HTTPClient http;
	http.begin(url);

	// start connection and send HTTP headerFFF
	int httpCode = http.GET();

	// httpCode will be negative on error
	if (httpCode > 0)
	{
		// file found at server
		if (httpCode == HTTP_CODE_OK)
		{
			String payload = http.getString();
			int pos = (payload.indexOf("follower"));
			fansCount = payload.substring(pos + 10, payload.length() - 2);
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
	}
	http.end();

	return atol(fansCount.c_str());
}

unsigned int Network::getTianqi(String url)
{
	String tanqiCount = "";
	HTTPClient https;
	https.begin(url);

	// start connection and send HTTP headerFFF
	int httpCode = https.GET();

	// httpCode will be negative on error
	if (httpCode > 0)
	{
		// file found at server
		if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
		{
			String payload = https.getString();
			//Serial.println(httpCode);
			//Serial.println(payload);
			int pos = (payload.indexOf("tem"));
			//Serial.println(payload.substring(pos, payload.length() - 419));
			tanqiCount = payload.substring(pos + 6, payload.length() - 432);
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
	}
	https.end();

	return atol(tanqiCount.c_str());
}
