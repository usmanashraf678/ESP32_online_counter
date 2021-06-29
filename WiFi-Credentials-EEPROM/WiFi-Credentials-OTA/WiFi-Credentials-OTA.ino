/**

   Written and Verified by the owner of techiesms
   Mr. Sachin Soni

   This code is open for fair use.
   If you are using it for commercial purpose, then it's mandatory to give credits

   Tutorial Video Link :- 

*/
// directives for ESP8266
//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <ESP8266WebServer.h>

// directives for ESP 32 
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <EEPROM.h>

//Variables
int i = 0;
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;


//Function Decalration
bool testWifi(void);
void launchWeb();
void setupAP(void);
void createWebServer();

//Establishing Local server at port 80 whenever required
WebServer server(80);


void setup()
{
  

  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  delay(5000);

  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  //---------------------------------------- Read eeprom for ssid and pass

  
  String esid = read_ee_ssid();
  String epass = read_ee_pass();


  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }

}
void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {

    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }

  }
  else
  {
  }

}
