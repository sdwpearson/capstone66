/* Arduino+esp8266 thingSpeak example  
 * Example name = "Write temperature and humidity to Thingspeak channel"
 * Created by Ilias Lamprou
 * Updated Oct 30 2016
 * 
 * Download latest Virtuino android app from the link:https://play.google.com/store/apps/details?id=com.virtuino_automations.virtuino&hl
 * Video tutorial link: https://youtu.be/4XEe0HY0j6k
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

// Code to use SoftwareSerial
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin



#include <DHT.h>        //Attention: For new  DHT11 version  library you will need the Adafruit_Sensor library
                        //Download from here: https://github.com/adafruit/Adafruit_Sensor



//Attention: For new  DHT11 version  libraries you will need the Adafruit_Sensor library
//Download from here:https://github.com/adafruit/Adafruit_Sensor
//and install to Arduino software

#define DHTPIN 7                // Connect the signal pin of DHT11 sensor to digital pin 5
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

String apiKey = "HJAUCNEYALJY0VAK";     // replace with your channel's thingspeak WRITE API key

boolean DEBUG=true;

//======================================================================== showResponce
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

//========================================================================
boolean thingSpeakWrite(float value1, float value2){
  String cmd = "AT+NETOPEN=”TCP”,80";                  // TCP connection
  cmd += "AT+TCPCONNECT=”192.168.0.1”,80";                               // api.thingspeak.com
  3gshield.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(3gshield.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+TCPWRITE=";
  cmd += String(getStr.length());
  3gshield.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(3gshield.find(">")){
    3gshield.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    3gshield.println("AT+NETCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+NETCLOSE");
    return false;
  }
  return true;
}
//================================================================================ setup
void setup() {                
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
  
  dht.begin();          // Start DHT sensor
  
  3gshield.begin(9600);  

   if (DEBUG)  Serial.println("Setup completed");
}
// ====================================================================== loop
void loop() {

  // Read sensor values
   float t = dht.readTemperature();
   float h = dht.readHumidity();
        if (isnan(t) || isnan(h)) {
        if (DEBUG) Serial.println("Failed to read from DHT");
      }
      else {
          if (DEBUG)  Serial.println("Temp="+String(t)+" *C");
          if (DEBUG) Serial.println("Humidity="+String(h)+" %");
           thingSpeakWrite(t,h);                                      // Write values to thingspeak
      }
  
    
  // thingspeak needs 15 sec delay between updates,     
  delay(20000);  
}
