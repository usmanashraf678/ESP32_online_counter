//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Victor Tchistiak - 2019
//
//This example demostrates master mode bluetooth connection and pin
//it creates a bridge between Serial and Classical Bluetooth (SPP)
//this is an extention of the SerialToSerialBT example by Evandro Copercini - 2018
//

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String ServerMACadd = "08:3A:F2:52:EA:AE";
uint8_t ServerMAC[6] = {0x08, 0x3A, 0xF2, 0x52, 0xEA, 0xAE};
String ServerName = "ESP32_SPP";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

void setup()
{
  Serial.begin(115200);
  // SerialBT.setPin(pin);
  SerialBT.begin("ESP32_Client", true);
  //SerialBT.setPin(pin);
  Serial.println("Client is started in master mode, make sure remote BT device is on!");

  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  // connected = SerialBT.connect(name);
  connected = SerialBT.connect(ServerMAC);

  if (connected)
  {
    Serial.println("Connected Succesfully!");
  }
  else
  {
    while (!SerialBT.connected(10000))
    {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
      ESP.restart();
    }
  }
  // // disconnect() may take upto 10 secs max
  // if (SerialBT.disconnect())
  // {
  //   Serial.println("Disconnected Succesfully!");
  // }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
}

void loop()
{
  if (Serial.available())
  {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    Serial.write(SerialBT.read());
  }
  delay(20);
}
// // imported libraries
// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebServer.h>
// #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager.git
// #include <EEPROM.h>

// #include <Keypad.h> // to use keypad
// #include <FirebaseESP32.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer.git
// #include <AsyncElegantOTA.h>

// // user defined header files
// #include <small_ssd.h>
// #include <change_wifi.h>
// #include <eeprom_helpers.h>
// #include <wifi_helpers.h>
// #include <large_ssd.h>
// #include "wifi_events.h"

// void reconnect_routine();
// void once_esp_connected();
// void force_channel_one();

// void setup()
// {
//   // Select the same baud rate if you want to see the datas on Serial Monitor
//   Serial.begin(115200);
//   Serial.println("Serial communication started\n\n");
//   EEPROM.begin(512);

//   setup_pins_small_ssd(); // setup output pins including light_on()

//   // ----- sending data out setup ----//
//   pinMode(strobePin, OUTPUT);
//   pinMode(clockPin, OUTPUT);
//   pinMode(dataPin, OUTPUT);
//   pinMode(buzzerPin, OUTPUT);
//   pinMode(selectWifiPin, INPUT_PULLUP);

//   // setup elegant OTA

//   WiFiSetup();
//   figure_out_wifi(); // setup wifi, OTA, and check for wifi change
//   esp_now_setup();
//   start_elegant_OTA();
//   publish_to_fb_and_esp();

// } // end setup()

// void loop()
// {
//   // count2nine();

//   // Displays 3 numbers on small SSD
//   display_on_small_ssd();

//   int customKey = customKeypad.getKey();
//   if (customKey)
//   {
//     if (WiFi.status() != WL_CONNECTED) //updated_locally == true &&
//     {
//       // needs more work!
//       // reconnect_routine();
//     }
//     int key_pressed = customKey - 48;

//     if (key_pressed >= 0 && key_pressed <= 9)
//     { // 0-9
//       shift_left(key_pressed);
//       publish_to_fb_and_esp();
//     }
//     else if (key_pressed == 17)
//     { // A
//       counter++;
//       if (counter >= 1000)
//         counter = 0;
//       publish_to_fb_and_esp();
//     }
//     else if (key_pressed == 18)
//     { // B
//       counter = max(counter - 1, 0);
//       publish_to_fb_and_esp();
//     }
//     else if (key_pressed == 20 && counter == 353) // D
//     {
//       if (WiFi.status() == WL_CONNECTED)
//         server.end(); // turns off elegant OTA if available
//       WiFi.begin();
//       open_wifi_settings();
//     }
//     else if (key_pressed == -6)
//     { // asterisk key
//       counter = 0;
//       publish_to_fb_and_esp();
//     }
//     else if (key_pressed == -13)
//     {
//       publish_to_fb_and_esp();
//     }
//     // check elegant ota, firebase and time
//   }
// }

// void reconnect_routine()
// {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(e_ssid.c_str(), e_pass.c_str());
//   delay(7000);
//   // always using the ssid on the eeprom
//   // Serial.println(channel);
//   // if(WiFi.status() != WL_CONNECTED)
//   // channel = 1;
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     once_esp_connected(); // move to a new channel
//   }
//   else
//   {
//     WiFi.disconnect();
//     force_channel_one();
//   }
// }

// void once_esp_connected()
// {
//   esp_now_deinit();

//   channel = getWiFiChannel(e_ssid.c_str());
//   // WiFi.printDiag(Serial);
//   Serial.printf("Wifi Channel from getWifiChannel is: %d on re-try \n", channel);

//   esp_wifi_set_promiscuous(true);
//   esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
//   esp_wifi_set_promiscuous(false);
//   // WiFi.printDiag(Serial);

//   // Init ESP-NOW
//   if (esp_now_init() != ESP_OK)
//   {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Once ESPNow is successfully Init, we will register for Send CB to
//   // get the status of Trasnmitted packet
//   esp_now_register_send_cb(OnDataSent);
//   // WiFi.reconnect();
//   // search_wifi(e_ssid.c_str());

//   // Register peer
//   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   peerInfo.ifidx = ESP_IF_WIFI_STA;
//   // peerInfo.channel = 11;
//   peerInfo.encrypt = false;

//   // Add peer
//   if (esp_now_add_peer(&peerInfo) != ESP_OK)
//   {
//     Serial.println("Failed to add peer");
//     Serial.println(esp_err_to_name(esp_now_add_peer(&peerInfo)));

//     return;
//   }
// }

// void force_channel_one()
// {
//   // esp_now_deinit();

//   // channel = getWiFiChannel(e_ssid.c_str());
//   // // WiFi.printDiag(Serial);
//   // Serial.printf("Wifi Channel from getWifiChannel is: %d  on re-try \n", channel);

//   esp_wifi_set_promiscuous(true);
//   esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
//   esp_wifi_set_promiscuous(false);
//   // WiFi.printDiag(Serial);

//   // Init ESP-NOW
//   // if (esp_now_init() != ESP_OK)
//   // {
//   //   Serial.println("Error initializing ESP-NOW");
//   //   return;
//   // }

//   // Once ESPNow is successfully Init, we will register for Send CB to
//   // get the status of Trasnmitted packet
//   // esp_now_register_send_cb(OnDataSent);

//   // WiFi.reconnect();
//   // search_wifi(e_ssid.c_str());

//   // Register peer
//   // memcpy(peerInfo.peer_addr, broadcastAddress, 6);
//   // peerInfo.ifidx = ESP_IF_WIFI_STA;
//   // // peerInfo.channel = 11;
//   // peerInfo.encrypt = false;

//   // // Add peer
//   // if (esp_now_add_peer(&peerInfo) != ESP_OK)
//   // {
//   //   Serial.println("Failed to add peer");
//   //   Serial.println(esp_err_to_name(esp_now_add_peer(&peerInfo)));

//   //   return;
//   // }
// }

// // WiFi.mode(WIFI_STA);
// //     // always using the ssid on the eeprom
// //     channel = getWiFiChannel(e_ssid.c_str());
// //     // Serial.println(channel);
// //     // if(WiFi.status() != WL_CONNECTED)
// //     // channel = 1;

// //     // WiFi.printDiag(Serial);
// //     Serial.printf("Wifi Channel from getWifiChannel is: %d ", channel);

// //     esp_wifi_set_promiscuous(true);
// //     esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
// //     esp_wifi_set_promiscuous(false);
// //     // WiFi.printDiag(Serial);

// //     // Init ESP-NOW
// //     if (esp_now_init() != ESP_OK)
// //     {
// //         Serial.println("Error initializing ESP-NOW");
// //         return;
// //     }

// //     // Once ESPNow is successfully Init, we will register for Send CB to
// //     // get the status of Trasnmitted packet
// //     esp_now_register_send_cb(OnDataSent);

// //     // Register peer
// //     memcpy(peerInfo.peer_addr, broadcastAddress, 6);
// //     peerInfo.ifidx = ESP_IF_WIFI_STA;
// //     // peerInfo.channel = 11;
// //     peerInfo.encrypt = false;

// //     // Add peer
// //     if (esp_now_add_peer(&peerInfo) != ESP_OK)
// //     {
// //         Serial.println("Failed to add peer");
// //         Serial.println(esp_err_to_name(esp_now_add_peer(&peerInfo)));

// //         return;
// //     }