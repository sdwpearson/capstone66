// inside sensor configuration 
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*====Pin defined====*/
#define DHTPIN 7 // DHT22 sensors reading pin
#define DS18B20PIN 8 // DS18B20 sensor reading pin 
#define WIFI_RX 4 // arduino RX pin 2 for wifi // MOVED SOFTWARE SERIAL PORTS TO MAKE ROOM FOR INTERRUPT INPUT
#define WIFI_TX 5 // arudino TX pin 3 for wifi
#define WATERFLOW 2 // Attached to interrupt 0

// air temperature sensors  
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

// WIFI constants  
const String apiKey = "HJAUCNEYALJY0VAK"; // thingspeak WRITE API key
const String ssid= "Albert's iPhone";//"Albert's iPhone";//"iPhone"; //"TELUS3205-2.4G";    // Wifi network SSID
const String password = "88888888"; //"88888888";//"stewsphone"; //"4gvheaypjg";  // Wifi network password
const String host_address = "184.106.153.149"; //  api.thingspeak.com

// global variables 
boolean DEBUG = true;
float air_humidity;
float air_temperature;
float soil_temperature;
double waterFlow=0;
double waterFlow_old=0;
double systemTime=0;
double systemTime_old=0;
double Flow_rate=0;
volatile int pulse_count=0;


// watchdog variables
volatile int f_wdt = 1;  // This variable is made volatile because it is changed inside an interrupt function
volatile int counter = 0;
const int sleep_duration = 2; // the maximum sleep time for wdt is 8s, 10mins/8sec = 75

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

