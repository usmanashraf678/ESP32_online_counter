/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "wifi_helpers.h"

unsigned long esp_now_ended = 0;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    int c;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// REPLACE WITH YOUR RECEIVER MAC Address: 08:3A:F2:51:59:30
// uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x51, 0x59, 0x30};
#define WIFI_SCAN_GAP 10000

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void esp_now_rx_setup();
int32_t search_wifi(const char *ssid);
void restart_if_WiFi_fluctuates();

void esp_now_rx_setup()
{
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_MODE_APSTA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else
    {
        Serial.println("Initialized ESP-NOW");
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);
    WiFi.setSleep(WIFI_PS_NONE);
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    // delay(2000);
    memcpy(&myData, incomingData, sizeof(myData));
    // Serial.print("Bytes received: ");
    // Serial.println(len);
    Serial.print("Counter: ");
    Serial.println(myData.c);
    counter = myData.c;
    try_wifi = true;

    esp_now_ended = millis();

    update_shiftOutBuffer();
    update_display();
    beep();

    Serial.println("new counter value set!!");
}

int32_t search_wifi(const char *ssid)
{
    if (int32_t n = WiFi.scanNetworks())
    {
        for (uint8_t i = 0; i < n; i++)
        {
            Serial.println(WiFi.SSID(i).c_str());
            if (!strcmp(ssid, WiFi.SSID(i).c_str()))
            {
                Serial.println("Wifi found");
                return 1;
            }
        }
        Serial.println("Wifi not found");
        return 0;
    }

    return 0;
}


void restart_if_WiFi_fluctuates()
{
  if (millis() - esp_now_ended > WIFI_SCAN_GAP && try_wifi == true)
  {
    // Serial.println("scan going to start for wifi");
    if (WiFi.status() != WL_CONNECTED) //updated_locally == true &&
    {
      if (search_wifi(e_ssid.c_str()))
      {
        Serial.println("manually restart the module to check");
        ESP.restart();
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