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

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void esp_now_rx_setup();

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
    else{
        Serial.println("Initialized ESP-NOW");
    }
    esp_wifi_set_ps(WIFI_PS_NONE);

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(OnDataRecv);
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Int: ");
    Serial.println(myData.c);

    Serial.println();
}