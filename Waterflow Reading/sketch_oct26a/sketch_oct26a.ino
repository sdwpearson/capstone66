/**
 * @file
 *
 * This is the main process for waterflow reading.  
 *
 */
#include "waterflow_sensor.h"

#define WATERSENSOR_PIN 2

/*================Initialization sensors===============*/
Waterflowsensor watermeter(WATERSENSOR_PIN);

const unsigned long duration = 2000;

void setup() {
  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600);

  // Initialize waterflow sensor
  watermeter.init();

  // enable a call to the 'interrupt service handler' (ISR) on every rising edge at the interrupt pin
  // do this setup step for every ISR you have defined, depending on how many interrupts you use
  attachInterrupt(INT0, WatersensorISR, RISING);
}

void loop() {
  // wait until the result updated 
  delay(duration);

  // compute the results 
  watermeter.measure(duration);

  // output some measurement result
  Serial.println("Currently " + String(watermeter.getFlowrate()) + " l/min, " + String(watermeter.getTotalVolume())+ " L in total.");
}

void WatersensorISR() {
	watermeter.pulseCounter();
}
