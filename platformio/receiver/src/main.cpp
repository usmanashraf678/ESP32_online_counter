// This is the Receiver code!!!!

// imported libraries
#include <Arduino.h>
#include <EEPROM.h>
// #include <WiFi.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager.git
#include <EEPROM.h>
#include "wifi_helpers.h"
#include <FirebaseESP32.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer.git
#include <AsyncElegantOTA.h>
#include "receiver_esp.h"
// user defined header files
#include "large_ssd.h"
#include "wifi_events.h"

int digits[10][8]{
    {0, 1, 1, 1, 1, 1, 1, 0}, // digit 0
    {0, 0, 1, 1, 0, 0, 0, 0}, // digit 1
    {0, 1, 1, 0, 1, 1, 0, 1}, // digit 2
    {0, 1, 1, 1, 1, 0, 0, 1}, // digit 3
    {0, 0, 1, 1, 0, 0, 1, 1}, // digit 4
    {0, 1, 0, 1, 1, 0, 1, 1}, // digit 5
    {0, 1, 0, 1, 1, 1, 1, 1}, // digit 6
    {0, 1, 1, 1, 0, 0, 0, 0}, // digit 7
    {0, 1, 1, 1, 1, 1, 1, 1}, // digit 8
    {0, 1, 1, 1, 1, 0, 1, 1}  // digit 9
};

void DisplayDigit(int Digit)
{
  digitalWrite(strobePin, LOW);
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(clockPin, LOW);
    if (digits[Digit][i] == 1)
      digitalWrite(dataPin, HIGH);
    if (digits[Digit][i] == 0)
      digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, HIGH);
  }
  digitalWrite(strobePin, HIGH);
}

void setup()
{
  // Select the same baud rate if you want to see the datas on Serial Monitor
  Serial.begin(115200);
  Serial.println("Serial communication started\n\n");
  EEPROM.begin(512);

  // ----- sending data out setup ----//
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(selectWifiPin, INPUT_PULLUP);
  
  counter = 123;
  update_shiftOutBuffer();
  update_display();

  WiFiSetup();
  // client_setup();
  // setup_BT_rx();
  figure_out_wifi();
  esp_now_rx_setup();
  start_elegant_OTA();
  // WiFi.reconnect();


  // testing the digits
  // for (int i = 0; i<10; i++){
  //   shiftOutBuffer[0] = i;
  //   shiftOutBuffer[1] = i;
  //   shiftOutBuffer[2] = i;
  //   update_display();
  //   delay(500);
  //   }

} // end setup()

void loop()
{
  // DisplayDigit(4);

  
  // for (int i = 0; i < 10; i++)
  // {
  //   DisplayDigit(i);
  //   delay(1000);
  // }
  // for (int i = 0; i < 10; i++)
  // {
  //   digitalWrite(strobePin, LOW);
  //   shiftOut(dataPin, clockPin, MSBFIRST, dec_digits[i]);
  //   digitalWrite(strobePin, HIGH);
  //   delay(2000);
  // }

  restart_if_WiFi_fluctuates();
  // for (int i = 0; i <= 9; i++)
  // {
  //   set_all_shiftOutBuffer(i);
  //   update_display();
  //   delay(1000);
  // }
}

// }

// client_loop();

// if (counter != incoming_counter){
//   counter = incoming_counter;
//   update_shiftOutBuffer();
//   update_display();
//   Serial.println("new counter value set!!");
// }

// using internet to read data
// read_from_firebase();

//   if (updated_on_cloud == false && WiFi.status() == WL_CONNECTED){
//     // if not updated on cloud and you have a wifi connection, then write to firebase
//     global_timestamp = get_time();
//     write_to_firebase(counter, global_timestamp);
//     }
//   if(updated_locally == true && WiFi.status() != WL_CONNECTED){// value changed locally but wifi is not connected
//       reattempt_wifi_connect();
//       updated_locally = false;
//     }
