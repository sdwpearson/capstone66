/*
 * This library is created for waterflow reading, (calibration needed?)
 *
 */

// inside sensor configuration 
#ifndef WATERFLOW_SENSOR_H
#define WATERFLOW_SENSOR_H

// Compatibility with the Arduino 1.0 library standard
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

struct FlowSensors_pro
{
	double capacity; // max flow rate in L/min
	double k_factor; // k_factor in (Pulse frequency / (L/min) )
  double mFactor[10];   // < multiplicative correction factor near unity, "meter factor" (per decile of flow)
};

extern FlowSensors_pro default_flowsensor; //!< default sensor

class Waterflowsensor {
  protected:
    unsigned int pin_;
    FlowSensors_pro properties_; 

    volatile unsigned long pulseCount = 0; // pulse within sampling period 
    double flowrate = 0.0f;
    double currentLitres = 0.0f;
    double totalLitres = 0.0f;
    double currentCorrection;  
    unsigned long duration; 

  public:
    Waterflowsensor(unsigned int pin = 2, 
    				FlowSensors_pro pro = default_flowsensor); // overloading constructor 

    double getFlowrate(); // return the current flow rate  
    double getTotalVolume(); // return the total volume 
    void pulseCounter(); // increment service rountine
    void measure(unsigned long duration = 1000); // update all the parameters 1000ms = 1s
    void init(); // initialize or reset all the parameters 
  };

#endif /*WATERFLOW_SENSOR_H*/
