/**
 * @file
 *
 * This is the main process for temperature reading. install DHT library first and add zipped library "adafruit_Sensor.h" 
 *
 */

#include "sensor_configuration.h" // 

/*================Initialization sensors===============*/
DHT dht(DHTPIN, DHTTYPE);  // initialize DHT sensor 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();  // start the sensors 
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
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
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t\n");

  delay(2000); // wait for two seconds 
}
