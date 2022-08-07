# Set up a system where the Raspberry Pi acts as broker and Esp8266 sends sensor information in Json format to the Python Program and uploads it to ThingSpeak
import random
import urllib.request
import urllib.error
import requests
import json
import threading
import certifi

import paho.mqtt.client as mqtt
requests.packages.urllib3.disable_warnings()
import ssl
ssl._create_default_https_context =ssl._create_unverified_context
try:
    _create_unverified_https_context=ssl._create_unverified_context
except AttributeError:
    pass
else:
    ssl._create_unverified_https_context=ssl._create_unverified_context
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("/esp8266/update")   #Topic subscribed to
def on_message(client, userdata, message):
    temp=message.payload
    data=json.loads(temp) 
    print(data) #Print the received Json data in dictionary format
    x=data['temp']
    y=data['humidity']
    URL='https://api.thingspeak.com/update?api_key='
    KEY='WGN65Q9L3UYZ6EOR'  #Write API key
    HEADER='&field1={}&field2={}'.format(x,y)
    new_URL=URL+KEY+HEADER
    print(new_URL)
    data1 =urllib.request.urlopen(new_URL)
    print(data1)
   
def main():
    mqtt_client= mqtt.Client()
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
   
    mqtt_client.connect('localhost', 1883, 60)
    mqtt_client.loop_start()
   
if __name__=='__main__':
    print("MQTT to InfluxDB bridge")
    main()
