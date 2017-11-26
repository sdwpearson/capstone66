/**
 * @file
 *
 * This is the main process for UBC Tree canopy shleter data logger system that read air and soil temperature, waterflow rate and windspeed.
 * It also push data to a public website for study and education purpose
 *
 */

/*====libraries included====*/
// air temperature sensor DHT22, may have to delete DHT_U.h
#include <DHT.h>
// soil temperature sensor DS18B20
#include <DallasTemperature.h>
#include <OneWire.h>
// WIFI ESP8266 
#include <SoftwareSerial.h> 

/*====Pin configuration library====*/
#include "configuration.h"

/*====Obejct creation====*/
//enum FSM workflow;
FSM current_state = INITIAL_STATE;
DHT dht(DHTPIN, DHTTYPE);  // create an instance for DHT sensor 
OneWire oneWire(DS18B20PIN);  // Setup a oneWire instance to communicate with any OneWire devices  
DallasTemperature DS18B20(&oneWire); // Pass our oneWire reference to Dallas Temperature.
SoftwareSerial espSerial(WIFI_RX,WIFI_TX); // create an instance for wifi chip esp8266

/*====Function definitions====*/
/**
 * @ display connection response of esp8266 during waiting time
 * @ param waitTime int the time period to wait 
 */
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
          c=espSerial.read();
          if (DEBUG) Serial.print(c);
      }
    }                   
}

/**
 * @ Interrupt function for reading the water flow sensor
 */
void pulse()   //measure the quantity of square wave
{
  pulse_count++;
  // Function that was derived to relate the pulse count to the volume of water
  waterFlow = 0.219*pulse_count + 16.518;
}

/**
 * @ Begin the esp8266 chip and connect it to WIFI 
 * @ return true if it connect to WIFI access point 
 */
bool esp8266_begin(){
   
//   espSerial.println("AT+RST");         // Enable this line to reset the module;
//   showResponse(1000);
   espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
   showResponse(1000);
   espSerial.println("AT+CWMODE=1");   // set esp8266 as client
   showResponse(1000);
   espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
   showResponse(5000);

   if (espSerial.find("OK")){
    if(DEBUG) Serial.println("Connected to Access Point");  
    return true;
   }
   else{
    if(DEBUG) Serial.println("Could not connect to Access Point"); 
    return false;
  }
}

/**
 * @ Establish a TCP connection to a web server and port and push data to ThingSpeak
 * @ param value1 float - air humidity (# sigificant digits) in %
 * @ param value2 float - air temperature (# sigificant digits) in degree celsius 
 * @ param value3 float - soil temperature (# sigificant digits) in degree celsius 
 * @ param value4 double - water volume in mL
 * @ param value5 double - flow rate in L/min
 * @ return true if it is successful sending the actual data, else return false
 */
bool thingspeak_write (float value1, float value2, float value3, double value4, double value5){
  // TCP connection 
  String cmd = "AT+CIPSTART=\"TCP\",\""; // TCP connection            
         cmd += host_address;  // "184.106.153.149" or api.thingspeak.com       
         cmd += "\",80";   // port 80
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error : TCP conenction fails!!");
    return false;
  }

  // Write data to ThingSpeak
  String getstring = "GET /update?api_key=";   // prepare GET string
         getstring += apiKey;
         // get parameters in string
         getstring +="&field1=";
         getstring += String(value1); // air temperature 
         getstring +="&field2=";
         getstring += String(value2); // air humidity
         getstring +="&field3=";
         getstring += String(value3); // soil temperature 
         getstring +="&field4=";
         getstring += String(value4); // water volume (mL)
         getstring +="&field5=";
         getstring += String(value5); // flow_rate (L/min) 
         // ...
         getstring += "\r\n\r\n"; // end string
   
  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getstring.length()); // count string length
  // Serial.println(getstring.length()); 
  espSerial.println(cmd); // send command 
  
  //After the execution of the above command, ESP will enter “unvarnished mode – returning
  // of  ‘>’ character”  – that we search for in the return of the command 
  if (DEBUG)  Serial.println(cmd);
  delay(100);
  if(espSerial.find(">")){
    //if successful, send the actual data
    espSerial.print(getstring);
    if (DEBUG)  Serial.print(getstring);
  }
  else{
    // If not, we close the connection
    espSerial.println("AT+CIPCLOSE");
    if (DEBUG)   Serial.println("AT+CIPCLOSE"); // alert user
    return false;
  }
  return true; 
}

/*====Initialization====*/
void setup() {
 // start serial port 
 Serial.begin(9600); 
 // start air temperature sensor
 dht.begin(); 
 // start soil temperatue sensor 
 DS18B20.begin(); 
 // start water flow sensor 
 waterFlow = 0;
 attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
 // enable software serial
 espSerial.begin(9600);
 //esp8266_begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(current_state){
    case INITIAL_STATE: // wake up the arduino and starts all sensors 
      air_humidity = 0.0f;
      air_temperature = 0.0f;
      soil_temperature = 0.0f; 
      current_state = AIR_TEMPERATURE;
      break;
    
    
    case AIR_TEMPERATURE: // air temperature reading
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      air_humidity = dht.readHumidity();
      // Read temperature as Celsius (the default)
      air_temperature = dht.readTemperature();
      
      // Check if any reads failed and exit early (to try again) else read soil temperature 
      if (isnan(air_humidity) || isnan(air_temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        current_state = AIR_TEMPERATURE;
        delay(2000); // wait for two seconds 
      }
      else{
        if (DEBUG) Serial.println("AIR_Temp="+String(air_temperature)+" *C");
        if (DEBUG) Serial.println("AIR_Humidity="+String(air_humidity)+" %");
        current_state = SOIL_TEMPERATURE;
      }
      break;
    
    
    case SOIL_TEMPERATURE:
      // call sensors.requestTemperatures() to issue a global temperature 
      // request to all devices on the bus if needed 
      DS18B20.requestTemperatures();
      delay(100);
      //more than one DS18B20 on the same bus is allowed. 
      soil_temperature = DS18B20.getTempCByIndex(0); //  0 refers to the first IC on the wire. 

      // Check if any reads failed and exit early (to try again) else read waterflow rate
      if(soil_temperature < (-55)){
         current_state = SOIL_TEMPERATURE;
      }
      else{
        if (DEBUG) Serial.println("SOIL_Temp="+String(soil_temperature)+" *C");
        current_state = WATER_FLOW;
      }
      break;

   
    case WATER_FLOW:
      // NOTE: Might have some issues integrating with sleep because of it is idle.
      //       If so, there is an EEPROM on board we can write to:
      //       https://www.arduino.cc/en/Reference/EEPROM
      //       Also need to watch out for the interrupts on pin2 waking it from sleep.
      
      // Get the current time in s
      systemTime = millis()/1000;
      
      // Calculate the Flow rate
      // L/min = (delta volume (mL) / delta time (s)) * 60 secs/min * 1 L/1000 mL
      // Serial.println("WaterFlow "+String(waterFlow)+" waterFlow_old "+String(waterFlow_old)+" systemTime "+String(systemTime)+" systemTime_old "+String(systemTime_old)); // For Debug
      Flow_rate = ((waterFlow-waterFlow_old)/(systemTime - systemTime_old))*60/1000;
      
      // Check if any reads failed and exit early (to try again) else transmit data
      if(waterFlow < 0){
         current_state = WATER_FLOW;
      }
      else{
        if (DEBUG) {
          Serial.println("FLOW_RATE="+String(Flow_rate)+" L/min");
          Serial.println("WATER_VOLUME="+String(waterFlow)+" mL");
        }
        current_state = DATA_TRANSMISSION;
      }
      // Keep track of old times and water volumes to compute the flow rate
      // Also use these in the data transmission so that it is the same as what's printed in DEBUG
      systemTime_old = systemTime;
      waterFlow_old = waterFlow;
      break;

    case DATA_TRANSMISSION:
      // begin esp8266 and connect it to wifi
      //noInterrupts(); // disable interrput 
      if(esp8266_begin()){
        // data transimission 
        bool done = thingspeak_write(air_temperature, air_humidity, soil_temperature, waterFlow_old, Flow_rate);
        if(done){
          current_state = SLEEP_MODE;
          delay(2000);
        }
        else{
          current_state = DATA_TRANSMISSION;
          delay(2000);
        }
      }
      else{
         current_state = DATA_TRANSMISSION; // if it fails, then try to connect it again;
         delay(2000);
      }
      //interrupts(); // enable interrupt 
      break;

     case SLEEP_MODE:
      // fake sleep for 3s
      if (DEBUG) Serial.println("Arduino sleeps");
      delay(15000);
      current_state = INITIAL_STATE;
      break;
      
    default:
      current_state = INITIAL_STATE;
  }

}
