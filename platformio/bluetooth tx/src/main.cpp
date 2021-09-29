/*
  This is the code for the project called

  WiFi & Bluetooth controlled Home Automation using ESP32

  This code is written by Sachin Soni on 14.08.2020

  The tutorial Video for the project is uploaded on 
  our YouTube channel called "techiesms"

  Channel Link - https://www.youtube.com/techiesms


          techiesms
  explore | learn | share 
*/

#include "BluetoothSerial.h" // https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial
#include "WiFi.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run
`make menuconfig` to and enable it
#endif

    BluetoothSerial SerialBT;

int bluedata; // variable for storing bluetooth data
void Bluetooth_handle();
void setup()
{
  Serial.begin(115200);

  btStart();
  Serial.println("Bluetooth On");

  SerialBT.begin("ESP32_Bluetooth"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(10000);

  Serial.println("Connecting to Internet");
  delay(2000);

  WiFi.begin("HUAWEI-8bz8", "qpwjUsMH");
  Serial.println("WiFi On");

  delay(2000);
}
void loop()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Not Connected");
  }
  else
  {
    Serial.println(" Connected");
  }

  if (SerialBT.available())
  {

    Bluetooth_handle();
  }
}

void Bluetooth_handle()
{
  //char bluedata;
  bluedata = SerialBT.parseInt();
  //Serial.println(bluedata);
  delay(20);
}