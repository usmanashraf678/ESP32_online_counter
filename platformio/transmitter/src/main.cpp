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

  // ----- counter setup code ----//
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(selectWifiPin, INPUT_PULLUP);

  figure_out_wifi(); // setup wifi, OTA, and check for wifi change
  esp_now_setup();

  // testing the digits
  // for (int i = 0; i<10; i++){
  //   shiftOutBuffer[0] = i;
  //   shiftOutBuffer[1] = i;
  //   shiftOutBuffer[2] = i;
  //   update_display();
  //   delay(500);
  //   }

  // reset_digits();
  // updated_on_cloud = false;

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
      write_to_firebase(counter);
      publish_to_esp();
    }
    else if (key_pressed == 17)
    { // A
      counter++;
      write_to_firebase(counter);
      publish_to_esp();
    }
    else if (key_pressed == 18)
    { // B
      counter = max(counter - 1, 0);
      write_to_firebase(counter);
      publish_to_esp();
    }
    else if (key_pressed == 20 && counter == 353) // D
    {
      if (WiFi.status() != WL_CONNECTED)
        server.end(); // turns off elegant OTA if available
      WiFi.begin();
      open_wifi_settings();
    }
    else if (key_pressed == -6)
    { // asterisk key
      counter = 0;
      write_to_firebase(counter);
      publish_to_esp();
    }
    else if (key_pressed == -13)
    {
      write_to_firebase(counter);
      publish_to_esp();
    }
    if (WiFi.status() != WL_CONNECTED) //updated_locally == true &&
    {                                  // value changed locally but wifi is not connected
      reattempt_wifi_connect();
      // updated_locally = false;
    }
  }
}
  // using internet to read data
  // read_from_firebase();
  // if (counter_needs_push)
  // {
  //   update_shiftOutBuffer();
  //   update_display();
  // }
