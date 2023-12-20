#pragma once
#include <Arduino.h>
#include "global_variables.h"
#include "eeprom_helpers.h"
#include <time.h>
#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager.git

#include <FirebaseESP32.h>
#include <AsyncElegantOTA.h>
#include "change_wifi.h"
#include "large_ssd.h"
#include "publisher_fb_esp.h"

/**************** Firebase Settings ****************************************/
#define FIREBASE_HOST "queue-mgmt-947d1-default-rtdb.asia-southeast1.firebasedatabase.app" //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "RMaoeR1ArcF2fo0BUaVa5eKI1KAutrmhcgMNoIh9"                           //Your Firebase Database Secret goes here

/*************** Async web server for Elegant OTA *************************************/
AsyncWebServer server(80);
unsigned long previousMillis = 0;

void figure_out_wifi();
void connect_to_wifi(const char *ssid, const char *pass);
void open_wifi_settings();
void start_elegant_OTA();
void post_wifi_setup();

void wifi_from_EEPROM();

void start_elegant_OTA()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! I am ESP32: ready for OTA."); });

    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("Elegant OTA started");
}

void open_wifi_settings()
{
    // switching to a different network
    Serial.println("Turning the HotSpot On for password change");
    launchWeb();
    setupAP(); // Setup HotSpot

    Serial.println();
    Serial.println("Waiting.");

    while ((WiFi.status() != WL_CONNECTED))
    {
        Serial.print(".");
        delay(1000);
        server_for_AP.handleClient();
    }
    Serial.println("new password saved to EEPROM, gonna restart in 10sec");

    delay(10000);
    ESP.restart();
}

void connect_to_wifi(const char *ssid, const char *pass) // try to connect with wifi given the credentials
{
    uint32_t retry_count = 0;
REDO: // jump back to the top of the function
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    Serial.print("Trying to connect to: ");
    Serial.println(ssid);

    //try to connect with wifi - first attempt
    unsigned long attempt_start_time = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - attempt_start_time < WIFI_TIMEOUT_MS)
    {
        // wait for some time untill WiFi is connected
        Serial.print(".");
        delay(500);
    }
    if (WiFi.status() != WL_CONNECTED)
    { // not connected
        Serial.print("failed connecting to wifi: ");
        Serial.println(ssid);
        WiFi.disconnect();
        if (retry_count < 1)
        { // 2 retries allowed
            retry_count++;
            goto REDO;
        }
    }
    else
    { // connected to wifi.. yay!!
        Serial.println();
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP Address is : ");
        Serial.println(WiFi.localIP()); //print local IP address
        Serial.print("DNS IP is : ");
        Serial.println(WiFi.dnsIP());
    }
}

void figure_out_wifi() // attempt to connect to wifi (1: hard code, 2: EEPROM )
{
    // using the hard code ssid and password

    Serial.println("Trying with EEPROM now..");

    e_ssid = read_ee_ssid();
    e_pass = read_ee_pass();
    connect_to_wifi(e_ssid.c_str(), e_pass.c_str());

    if (WiFi.status() != WL_CONNECTED)
    { // not connected
        // connect using credentials from hard code
        // connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
    }

    // looking for need to reset wifi network
    Serial.println("Connect D4 to ground in 1 sec to switch wifi");
    bool wifi_reset = digitalRead(selectWifiPin);
    delay(1000);
    if (wifi_reset == LOW)
        open_wifi_settings();

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Could not connect after utilizing both in-code and EEPROM creds..");
        Serial.println("let's continue without wifi");
    }
    else
        post_wifi_setup();
}

void post_wifi_setup() // configure time, firebase, elegant ota on wifi connection
{
    // fixate the time
    configTime(18000, 0, "pool.ntp.org", "time.nist.gov");

    // connect to firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    // Firebase.allowMultipleRequests(true);
    // Firebase.setReadTimeout(fbdo, 5000);

    // setup elegant OTA
    start_elegant_OTA();
    Serial.println(WiFi.macAddress());
    WiFi.reconnect();
}

/********* everything above this line has been refactored ****************/
void reattempt_wifi_connect() // re-attempt connecting to wifi on every even token number
{
    Serial.println("Reattempt connecting to ");
    Serial.println("Trying with EEPROM now..");
    unsigned long currentMillis = millis();
    // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= 7000))
    {
        Serial.print(currentMillis - previousMillis);
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.begin(e_ssid.c_str(), e_pass.c_str());

        previousMillis = currentMillis;

    }
    if(WiFi.status() == WL_CONNECTED){
        Serial.println("Wifi is connected");
        post_wifi_setup();

    }


}


//  e_ssid = read_ee_ssid();
    // e_pass = read_ee_pass();
    // connect_to_wifi(e_ssid.c_str(), e_pass.c_str());

    // if (counter % 2 == 0)
    // { // when the token number served is even
    //     e_ssid = read_ee_ssid();
    //     e_pass = read_ee_pass();
    //     WiFi.begin(e_ssid.c_str(), e_pass.c_str());
    //     // new_channel = getWiFiChannel(esid.c_str());
    //     Serial.println(e_ssid);
    //     delay(500);
    // }
    // else
    // {
    //     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    //     // new_channel = getWiFiChannel(WIFI_SSID);
    //     Serial.println(WIFI_SSID);
    //     delay(500);
    // }


    // if (WiFi.status() != WL_CONNECTED)
    // { // not connected
    //     Serial.println("failed connecting to the mentioned network");
    //     //      WiFi.disconnect();
    // }
    // else
    // { // connected to the wifi
    //     Serial.println();
    //     Serial.print("Connected to ");
    //     Serial.println(WIFI_SSID);
    //     Serial.print("IP Address is : ");
    //     Serial.println(WiFi.localIP()); //print local IP address
    //     post_wifi_setup();
    // }

    // Serial.printf("Wifi Channel from getWifiChannel is: %d ", new_channel);
    // Serial.println(new_channel);
    // esp_wifi_set_promiscuous(true);
    // esp_wifi_set_channel(new_channel, WIFI_SECOND_CHAN_ABOVE);
    // esp_wifi_set_promiscuous(false);