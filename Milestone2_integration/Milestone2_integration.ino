/**
 * @file
 *
 * This is the main process for UBC Tree canopy shleter data logger system that read air and soil temperature, waterflow rate and windspeed.
 * It also push data to a public website for study and education purpose
 *
 */
 
/*====*====*/
/*====Pin configuration library====*/
#include "configuration.h" 

/*====Obejct creation====*/
//enum FSM workflow;
FSM current_state = INITIAL_STATE;
DHT dht(DHTPIN, DHTTYPE);  // create an instance for DHT sensor 
OneWire oneWire(DS18B20PIN);  // Setup a oneWire instance to communicate with any OneWire devices  
DallasTemperature DS18B20(&oneWire); // Pass our oneWire reference to Dallas Temperature.
SoftwareSerial espSerial(2,3); // create an instance for wifi chip esp8266

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
 * @ Begin the esp8266 chip and connect it to WIFI 
 * @ return true if it connect to WIFI access point 
 */
bool esp8266_begin(){
   
   espSerial.println("AT+RST"); // reset ESP8266
   showResponse(1000);
   espSerial.println("AT+UART_CUR=9600,8,1,0,0"); // Enable this line to set esp8266 serial speed to 9600 bps
   showResponse(1000);
   //conect to network
   espSerial.println("AT+CWMODE=1"); // set esp8266 as a client
   showResponse(1000);
   espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\""); // set your home router SSID and password
   showResponse(5000); //wait for 5 seconds
   
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
 * @ return true if it is successful sending the actual data, else return false
 */
bool thingspeak_write (float value1, float value2, float value3){
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
         getstring += String(value1); // air humidity
         getstring +="&field2=";
         getstring += String(value2); // air temperature 
         getstring +="&field3=";
         getstring += String(value3); // soil temperature 
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

 // enable software serial
 espSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(current_state){
    case INITIAL_STATE: // wake up the arduino and starts all sensors 
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
        current_state = DATA_TRANSMISSION;
      }
      break;

    case DATA_TRANSMISSION:
      // begin esp8266 and connect it to wifi
      if(esp8266_begin()){
        // data transimission 
        bool done = thingspeak_write(air_humidity, air_temperature, soil_temperature);
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
      break;
      
    default:
      current_state = INITIAL_STATE;
  }

}
