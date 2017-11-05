/*************************************************************************
* Web Server with ESP8266
* Based on: FILIPEFLOP http://blog.filipeflop.com/wireless/esp8266-arduino-tutorial.html
* Adapted by Marcelo Jose Rovai
**************************************************************************/
#include <SoftwareSerial.h>

//#define esp8266 Serial2
#define CH_PD 4 
#define speed8266 115200 // This is the speed that worked with my ESP8266
 
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
  if (esp8266.available()) // check if 8266 is sending data
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      
      webpage += "</head><h1>STEW WebServer ESP8266</h1><h2>Arduino Pin: ";
      webpage += "D8 status ==> ";
      int a = digitalRead(8);
      Serial.print("\n\rpin 8 ==> ");
      Serial.print(a);
      webpage += a;
      webpage += "<h2>Arduino Pin: D9 status ==> ";
      int b = digitalRead(9);
      Serial.print("\n\rpin 9 ==> ");
      Serial.print(b);
      webpage += b;
      //webpage += "<h2>Arduino Pin: A0 data ===> ";
      //int c = analogRead(0);
      //webpage += c;
      webpage += "</h2>";
      
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
    }
  }
}
 
/*************************************************/
void InitWifiModule()
{
  sendData("AT+RST\r\n", 2000, DEBUG); // reset
  //sendData("AT+CWJAP=\"Network\",\"Password\"\r\n", 2000, DEBUG); //Connect network
  sendData("AT+CWJAP=\"JB_Hillary\",\"nostrajibus\"\r\n", 2000, DEBUG); //Connect network
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
