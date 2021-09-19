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

  figure_out_wifi(); // setup wifi, OTA, and check for wifi change
  esp_now_setup();
  publish_to_fb_and_esp();
} // end setup()

void loop()
{
  // count2nine();

  // Displays 3 numbers on small SSD
  display_on_small_ssd();

  int customKey = customKeypad.getKey();
  if (customKey)
  {
    int key_pressed = customKey - 48;

    if (key_pressed >= 0 && key_pressed <= 9)
    { // 0-9
      shift_left(key_pressed);
      publish_to_fb_and_esp();
    }
    else if (key_pressed == 17)
    { // A
      counter++;
      if(counter >= 1000)
        counter = 0;
      publish_to_fb_and_esp();
    }
    else if (key_pressed == 18)
    { // B
      counter = max(counter - 1, 0);
      publish_to_fb_and_esp();
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
      publish_to_fb_and_esp();
    }
    else if (key_pressed == -13)
    {
      publish_to_fb_and_esp();
    }
    if (WiFi.status() != WL_CONNECTED) //updated_locally == true &&
    {                                  // value changed locally but wifi is not connected
      reattempt_wifi_connect();
      // updated_locally = false;
    }
  }
}