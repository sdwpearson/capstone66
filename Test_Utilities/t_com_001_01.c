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
#define speed3gshield 115200 // This is the speed that worked with my ESP8266
 
#define DEBUG true
 
// software serial #1: RX = digital pin 2, TX = digital pin 3
SoftwareSerial 3gshield(2, 3);

void setup()
{
  3gshield.begin (speed3gshield); 
  Serial.begin(9600);
}
 
void loop()
{  
  sendData("AT+NETOPEN=”TCP”,80",1000,1);  
  sendData("AT+TCPCONNECT=”192.168.0.1”,80",1000,1);  
  sendData("AT+TCPWRITE=8",1000,1);  
  sendData("AT+NETCLOSE ",1000,1);  
}

/*************************************************/ 
 // Send AT commands to module and print the response
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  3gshield.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (3gshield.available())
    {
      // The esp has data so display its output to the serial window
      char c = 3gshield.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
