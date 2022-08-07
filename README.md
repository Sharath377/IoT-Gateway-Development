# IoT-Gateway-Development
In this project, we use Raspberry Pi 4 as our Access Point which receives data from many NodeMCU's connected to DHT11 and sends the data to ThingSpeak Cloud

In order to replicate the project follow the given steps:
1) Convert your Raspberry Pi into an Access Point.For this project, we have made use of Raspberry Pi 4.
2) Connect your DHT11 sensor(or any sensor) to your NodeMCU(esp8266).To upload the code to NodeMCU we havev used Arduino IDE.
3) We have used MQTT protocol to communicate between Pi and the NodeMCU's. Pi acts as the MQTT broker.
4) Write a Python code to upload the recieved sensor readings to ThingSpeak.
