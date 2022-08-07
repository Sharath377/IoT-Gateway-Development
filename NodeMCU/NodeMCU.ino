#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "DHT.h"
#define DHTTYPE DHT11  
DynamicJsonDocument doc(1024);
//In this case SSID and password will be the Raspberry Pi Access Point
const char* ssid = "YOUR SSID NAME"; 
const char* password = "YOUR SSID PASSWORD";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "RPI IP ADDRESS";
const int mqttPort = 1883;
// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor
const int DHTPin = 14;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    //For multiple clients use different names
    if (client.connect("ESP8266Client3")) {
      Serial.println("connected");  
      client.subscribe("esp8266/4");
      client.subscribe("esp8266/5");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(15000);
    }
  }
}
char* toCharArrays(String str){
  return &str[0];
}


void setup() {
  dht.begin();
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())

    client.connect("ESP8266Client3");
    
  now = millis();
  // Publishes new temperature and humidity every 10 seconds
  if (now - lastMeasure > 10000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    //Store NodeID,temperature and humidity in Json format 
 doc["ID"]=WiFi.macAddress();
   doc["temp"]=t;
  doc["humidity"]=h;
  char buffer[256];
   serializeJson(doc,buffer);
    //Publish the data
    client.publish("/esp8266/update",buffer);
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
     Serial.println("");
  }
}
