/*************************************************************************
* Web Server with ESP8266
* Based on: FILIPEFLOP http://blog.filipeflop.com/wireless/esp8266-arduino-tutorial.html
* Adapted by Marcelo Jose Rovai
**************************************************************************/

/* IN ORDER TO COMMUNICATE PROPERLY WITH SOFTWARE SERIAL:
 *   You MUST change the baud rate lower. I used 9600 and 
 *   it worked well enough. The ESP8266 comes at 115200 as 
 *   standard which will only work on a hardware serial. Instead
 *   use the command AT+CIOBAUD=9600 inside ExampleSketchInst1 to lower
 *   the baud rate.
 *   Source: https://arduino.stackexchange.com/questions/24156/how-to-change-baudrate-of-esp8266-12e-permanently
 */
#include <SoftwareSerial.h>

//#define esp8266 Serial2
#define CH_PD 4 
#define speed8266 9600//115200 // This is the speed that worked with my ESP8266
 
#define DEBUG true
 
// software serial #1: RX = digital pin 2, TX = digital pin 3
SoftwareSerial esp8266(2, 3);

void setup()
{
  esp8266.begin (speed8266); 
  Serial.begin(9600);
  reset8266(); // Pin CH_PD needs a reset before start communication
  InitWifiModule(); // Initiate module as WebServer 
}
 
void loop()
{
  
}
 
/*************************************************/
void InitWifiModule()
{
  sendData("AT+RST\r\n", 2000, DEBUG); // reset
  // REPLACE Network WITH YOUR SSID AND Password WITH YOUR PASSWORD
  sendData("AT+CWJAP=\"Network\",\"Password\"\r\n", 2000, DEBUG); //Connect network
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // Show IP Adress
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // Multiple conexions
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // start comm port 80
}

/*************************************************/ 
 // Send AT commands to module and print the response
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

/*************************************************/
// Reset funtion to accept communication
void reset8266 ()
{
  pinMode(CH_PD, OUTPUT);
  digitalWrite(CH_PD, LOW);
  delay(300);
  digitalWrite(CH_PD, HIGH);
}
