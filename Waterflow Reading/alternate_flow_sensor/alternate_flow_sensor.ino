/***************************************************
 2  This example reads Water flow sensor Sensor.
 3  
 4  Created 2016-3-13
 5  By berinie Chen <bernie.chen@dfrobot.com>
 6  
 7  GNU Lesser General Public License.
 8  See <http://www.gnu.org/licenses/> for details.
 9  All above must be included in any redistribution
10  ***************************************************
***********Notice and Trouble shooting***************
13  1.Connection and Diagram can be found here  http://www.dfrobot.com/wiki/index.php?title=Water_Flow_Sensor_-_1/8%22_SKU:_SEN0216
14  2.This code is tested on Arduino Uno.
15  ****************************************************/
volatile double waterFlow;
volatile double waterFlow_old;
volatile double Flow_rate;
int pulse_count=0;
volatile int pulse_count_flow = 0;
void setup() {
  Serial.begin(9600);  //baudrate
  waterFlow = 0;
  attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
}
void loop() {  
  Serial.print("water rate: ");
  //Flow_rate = pulse_count_flow*60/7.5;
  Flow_rate = (waterFlow-waterFlow_old)*60/1000;
  Serial.print(Flow_rate);
  Serial.print("   waterFlow: ");
  Serial.print(waterFlow, DEC);
  Serial.print("   mL pulses: ");
  Serial.print(pulse_count);
  Serial.println();
  pulse_count_flow = 0;
  waterFlow_old=waterFlow;
  
  delay(1000);
}

void pulse()   //measure the quantity of square wave
{
  //pulse_count_flow += 1.0 / 5880.0;
  pulse_count++;
  //pulse_count_flow++;
  waterFlow = 0.219*pulse_count + 16.518;
}
