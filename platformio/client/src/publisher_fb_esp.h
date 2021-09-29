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

// const char *remote_host = "www.google.com";
const IPAddress remote_ip(192, 168, 43, 1);

uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x51, 0x59, 0x30};
uint32_t esp_retry = 0;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    int c;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// REPLACE WITH YOUR RECEIVER MAC Address: 08:3A:F2:51:59:30
esp_now_peer_info_t peerInfo;

// Declare the Data object in the global scope
FirebaseData fbdo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void esp_now_setup();
int32_t getWiFiChannel(const char *ssid);
void post_wifi_setup();
void publish_to_firebase(int val);
void publish_to_esp();
void publish_to_fb_and_esp(); // reset the digits of the counter

int32_t getWiFiChannel(const char *ssid)
{

    if (int32_t n = WiFi.scanNetworks())
    {
        for (uint8_t i = 0; i < n; i++)
        {
            if (!strcmp(ssid, WiFi.SSID(i).c_str()))
            {
                return WiFi.channel(i);
            }
        }
    }

    return 0;
}

void esp_now_setup()
{
    WiFi.mode(WIFI_STA);
    // always using the ssid on the eeprom
    channel = getWiFiChannel(e_ssid.c_str());
    // Serial.println(channel);
    // if(WiFi.status() != WL_CONNECTED)
    // channel = 1;

    // WiFi.printDiag(Serial);
    Serial.printf("Wifi Channel from getWifiChannel is: %d ", channel);

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
    // WiFi.printDiag(Serial);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.ifidx = ESP_IF_WIFI_STA;
    // peerInfo.channel = 11;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Last Packet: ");
    // Serial.print(WiFi.channel());
    if (status == 0)
    {
        Serial.println("Delivery success");
        esp_retry = 0;
    }
    else
    {
        Serial.println("Delivery fail");
        esp_retry++;
        if (esp_retry <= 1)
        { // 2 retries allowed
            publish_to_esp();
        }
    }
}

String get_time() // returns a string of current time e.g. Sat 20-Apr-19 12:31:45
{
    time_t now;
    time(&now);
    char time_output[30];
    // See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions
    strftime(time_output, 30, "%r %a %d-%m-%y", localtime(&now));
    return String(time_output);
}

void publish_to_firebase(int val)
{
    // Firebase Error Handling And Writing Data At Specifed Path************************************************
    if (WiFi.status() == WL_CONNECTED) // updated_on_cloud == false &&
    {
        String global_timestamp = get_time();
        if (Firebase.setIntAsync(fbdo, "/dr_irfan_counter", val) && Firebase.setStringAsync(fbdo, "/dr_irfan_time", global_timestamp))
        {
            Serial.println("\n");
            Serial.println("Value Uploaded Successfully");
            Serial.print("Val = ");
            Serial.println(val);
            updated_on_cloud = true;
            Serial.println(global_timestamp);
        }
        else
        {
            Serial.println("Connected but no internet");
            Serial.println(fbdo.errorReason());
        }
    }

    else
        Serial.println("No conneciton to wifi");
}

// Serial.println(global_timestamp);
// if (Firebase.setInt(fbdo, "/dr_irfan_counter", val))

// if (Firebase.setInt(fbdo, "/dr_irfan_counter", val) && Firebase.setString(fbdo, "/dr_irfan_time", global_timestamp))
// { // On successful Write operation, function returns 1
//
// }
// else
// {
//     Serial.println(fbdo.errorReason());
//     post_wifi_setup();
// }

void publish_to_esp()
{
    myData.c = counter;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    Serial.printf("\n sent: %3u on channel: %u, ", myData.c, WiFi.channel());
}

void publish_to_fb_and_esp()
{
    esp_retry = 0;
    publish_to_esp();
    publish_to_firebase(counter);
}