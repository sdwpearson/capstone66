



volatile double waterFlow;
volatile double waterFlow_new;
volatile double waterFlow_old;
volatile int pulse_count;
volatile int pulse_count_old;
volatile double flow_rate;
volatile double time_old;
volatile double time_old1;
volatile double period;
volatile double freq;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  waterFlow = 0;
  waterFlow_new = 0;
  pulse_count=0;
  time_old = 0;

  attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  flow_rate = (waterFlow-waterFlow_old)/(double(millis())-time_old)*60;
  //flow_rate = freq/3.75;
  if(pulse_count_old != pulse_count)
    waterFlow_new += flow_rate/60.0*1000.0/freq;
  waterFlow_old=waterFlow;
  time_old = millis();
  
  /*Serial.print("System Time: ");
  Serial.print(millis());*/
  Serial.print(" WaterFlow: ");
  Serial.print(waterFlow);
  Serial.print(" ml ");
  Serial.print("Flow rate: ");
  Serial.print(flow_rate);
  Serial.print(" L/min: ");
  Serial.print("Pulse count: ");
  Serial.print(pulse_count);
  Serial.print(" Pulse Frequency: ");
  Serial.print(freq);
  Serial.print(" Hz: ");
  Serial.print("\n");
  
  delay(500);
}

void pulse() //measure the quantity of square wave 30 
{
  //waterFlow += 1.0 / 3561.0 * 1000;
  waterFlow = 0.2415*pulse_count+27.563;
  pulse_count++;
  period = (millis()-time_old1)/1000.0;
  freq = 1/period;
  time_old1 = millis();
  pulse_count_old = pulse_count;
}







