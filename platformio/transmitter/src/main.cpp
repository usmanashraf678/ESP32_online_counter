// imported libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager.git
#include <EEPROM.h>

#include <Keypad.h> // to use keypad
#include <FirebaseESP32.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer.git
#include <AsyncElegantOTA.h>

// user defined header files
#include <small_ssd.h>
#include <change_wifi.h>
#include <eeprom_helpers.h>
#include <wifi_helpers.h>
#include <large_ssd.h>
#include "wifi_events.h"
#include "bluetooth_tx.h"
#include "publisher_fb.h"
#include "publisher_esp.h"

void reconnect_routine();
void once_esp_connected();
void force_channel_one();

bool previous_wifi_state = false;
bool current_wifi_state = false;


void setup()
{
  // Select the same baud rate if you want to see the datas on Serial Monitor
  Serial.begin(115200);
  Serial.println("Serial communication started\n\n");
  EEPROM.begin(512);

  setup_pins_small_ssd(); // setup output pins including light_on()

  // ----- sending data out setup ----//
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(selectWifiPin, INPUT_PULLUP);

  // setup elegant OTA

  // setup_BT_TX();
  WiFiSetup();
  figure_out_wifi(); // setup wifi, OTA, and check for wifi change
  esp_now_setup();
  start_elegant_OTA();
  publish_to_firebase();

} // end setup()

void loop()
{
  // count2nine();
  // BT_tx_loop();
  // Displays 3 numbers on small SSD
  display_on_small_ssd();

  int customKey = customKeypad.getKey();
  if (customKey)
  {
    if (WiFi.status() != WL_CONNECTED && search_wifi(e_ssid.c_str())) //updated_locally == true &&
    {
      Serial.println("manually restart the module to check");
      ESP.restart();
      // time to restart
      // needs more work!
      // reconnect_routine();
    }
    int key_pressed = customKey - 48;

    if (key_pressed >= 0 && key_pressed <= 9)
    { // 0-9
      shift_left(key_pressed);
      updated_locally = true;
      publish_to_esp();
      publish_to_firebase();
    }
    else if (key_pressed == 17)
    { // A
      counter++;
      if (counter >= 1000)
        counter = 0;
      updated_locally = true;
      publish_to_esp();
      publish_to_firebase();
    }
    else if (key_pressed == 18)
    { // B
      counter = max(counter - 1, 0);
      updated_locally = true;
      publish_to_esp();
      publish_to_firebase();
    }
    else if (key_pressed == 20 && counter == 353) // D
    {
      if (WiFi.status() == WL_CONNECTED)
        server.end(); // turns off elegant OTA if available
      WiFi.begin();
      open_wifi_settings();
    }
    else if (key_pressed == -6)
    { // asterisk key
      counter = 0;
      updated_locally = true;
      publish_to_esp();
      publish_to_firebase();
    }
    else if (key_pressed == -13)
    {
      updated_locally = true;
      publish_to_esp();
      publish_to_firebase();
      // SerialBT.parseInt();
    }
    // check elegant ota, firebase and time
  }
}

void reconnect_routine()
{
  WiFi.reconnect();
  // WiFi.disconnect();
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(e_ssid.c_str(), e_pass.c_str());
  // Serial.print("Trying to connect to: ");
  // Serial.println(e_ssid);
  // delay(7000);
  // always using the ssid on the eeprom
  // Serial.println(channel);
  // if(WiFi.status() != WL_CONNECTED)
  // channel = 1;
  if (WiFi.status() == WL_CONNECTED)
  {
        // once_esp_connected(); // move to a new channel
  }
  // else
  // {
  //   WiFi.disconnect();
  //   // force_channel_one();
  // }
}