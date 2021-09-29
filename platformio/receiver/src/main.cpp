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

#define WIFI_SCAN_GAP 10000
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
  if (millis() - esp_now_ended > WIFI_SCAN_GAP && try_wifi == true)
  {
    Serial.println("scan going to start for wifi");
    if (WiFi.status() != WL_CONNECTED) //updated_locally == true &&
    {
      if (search_wifi(e_ssid.c_str()))
      {
        Serial.println("manually restart the module to check");
        ESP.restart();
        // time to restart
        // needs more work!
        // reconnect_routine();
      }
      else
      {
        Serial.println("wifi of interest not present");
      }
    }
    else
    {
      Serial.println("wifi already connected");
    }
    try_wifi = false;
  }
}
// client_loop();

// if (counter != incoming_counter){
//   counter = incoming_counter;
//   update_shiftOutBuffer();
//   update_display();
//   Serial.println("new counter value set!!");
// }

// using internet to read data
// read_from_firebase();
// if (counter_needs_push)
// {
//   update_shiftOutBuffer();
//   update_display();
// }

//   if (updated_on_cloud == false && WiFi.status() == WL_CONNECTED){
//     // if not updated on cloud and you have a wifi connection, then write to firebase
//     global_timestamp = get_time();
//     write_to_firebase(counter, global_timestamp);
//     }
//   if(updated_locally == true && WiFi.status() != WL_CONNECTED){// value changed locally but wifi is not connected
//       reattempt_wifi_connect();
//       updated_locally = false;
//     }
