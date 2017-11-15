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
void setup() {
  Serial.begin(9600);  //baudrate
  waterFlow = 0;
  attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
}
void loop() {
  Serial.print("waterFlow:");
  Serial.print(waterFlow);
  Serial.println("   L");
  delay(500);
}

void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 5880.0;
}
