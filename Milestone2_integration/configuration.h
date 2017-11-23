// inside sensor configuration 
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*====libraries included====*/
// air temperature sensor DHT22
#include "DHT.h"
// soil temperauter sensor DS18B20
#include <OneWire.h> 
#include <DallasTemperature.h>
// WIFI ESP8266 
#include <SoftwareSerial.h>

/*====Pin defined====*/
#define DHTPIN 7 // DHT22 sensors reading pin
#define DS18B20PIN 8 // DS18B20 sensor reading pin 
#define WIFI_RX 2 // arduino RX pin 2 for wifi
#define WIFI_TX 3 // arudino TX pin 3 for wifi

// air temperature sensors  
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

// WIFI constants  
const String apiKey = "HJAUCNEYALJY0VAK"; // thingspeak WRITE API key
const String ssid= "iPhone"; //"TELUS3205-2.4G";    // Wifi network SSID
const String password = "stewsphone"; //"4gvheaypjg";  // Wifi network password
const String host_address = "184.106.153.149"; //  api.thingspeak.com

// global variables 
boolean DEBUG = true;
float air_humidity;
float air_temperature;
float soil_temperature;

/*====Finite state machine====*/
typedef enum {
  INITIAL_STATE, 
  AIR_TEMPERATURE, 
  SOIL_TEMPERATURE, 
  WATER_FLOW, 
  DATA_TRANSMISSION,
  SLEEP_MODE
}FSM;

#endif /*SENSOR_CONFIGURATION_H*/

