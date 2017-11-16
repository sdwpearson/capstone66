#include "waterflow_sensor.h"

// Compatibility with the Arduino 1.0 library standard
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

FlowSensors_pro default_flowsensor = {6.0f, 7.5f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// constructor
Waterflowsensor::Waterflowsensor(unsigned int pin, FlowSensors_pro pro)
{
  pin_ = pin;
  properties_ = pro;
  /*The Atmega microcontroller on the Arduino has internal pull-up resistors 
  (resistors that connect to power internally) that you can access.*/
  pinMode(pin_, INPUT_PULLUP);
}

/*
 * Return water flow rate
 */
double Waterflowsensor::getFlowrate()
{
	return flowrate;
}

/*
 * Return water volume 
 */
double Waterflowsensor::getTotalVolume()
{
	return totalLitres;
}

/*
 * Insterrupt Service Routine
 */
void Waterflowsensor::pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

/*
 * calculate and update all the parameters
 */
void Waterflowsensor::measure(unsigned long duration)
{
	double seconds = duration / 1000.0f;
	unsigned int interrupts = SREG; // save the interrupt status
	cli();	// disable the interrupt
	double frequency = pulseCount / seconds; 
	pulseCount = 0; // clean the pulse 
	SREG = interrupts; // load the interrupt status

 unsigned int decile = floor(10.0f * frequency / (properties_.capacity * properties_.k_factor));
 currentCorrection = properties_.k_factor / properties_.mFactor[min(decile, 9)];

	flowrate = frequency / currentCorrection;	// compute flowrate in L/min
	currentLitres = flowrate * seconds / 60.0f;		// compute current waterflow 
	totalLitres += currentLitres; 		// compute the total waterflow 
}

/*
 * Initailize or reset all the parameters
 */
void Waterflowsensor::init()
{
	unsigned int interrupts = SREG; // save the interrupt status
	cli();	// disable the interrupt
	pulseCount = 0; // clean the pulse 
	SREG = interrupts; // load the interrupt status
	

	flowrate = 0.0f;
	currentLitres = 0.0f;
	totalLitres = 0.0f;
	duration = 0; 
}


