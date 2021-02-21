#include <WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "test";
const char* password = "12345678";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
    //pinMode(0, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    //pinMode(16, OUTPUT);
    //pinMode(5, OUTPUT);
    //pinMode(4, OUTPUT);
    //digitalWrite(0, LOW);
    //digitalWrite(16, LOW);
    //digitalWrite(5, LOW);
    //digitalWrite(4, LOW);
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void setup_wifi() {

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

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

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') 
    {
      
    }
    else {

    }


}

void reconnect() 
{
    // Loop until we're reconnected
    while (!client.connected()) 
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        String clientId = "ESP32Client";
        if (client.connect("ESP8266Client")) 
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
        }
        else 
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000) 
    {
        lastMsg = now;
        ++value;
        snprintf(msg, 75, "hello world #%ld", value);
        Serial.print("Publish message: ");
        Serial.println(msg);
        //client.publish("outTopic", msg);
    }
}