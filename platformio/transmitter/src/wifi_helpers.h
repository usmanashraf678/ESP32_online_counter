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
#define FIREBASE_HOST "queue-mgmt-947d1-default-rtdb.asia-southeast1.firebasedatabase.app"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "RMaoeR1ArcF2fo0BUaVa5eKI1KAutrmhcgMNoIh9" //Your Firebase Database Secret goes here

/*************** Async web server for Elegant OTA *************************************/
AsyncWebServer server(80);

void figure_out_wifi();
void connect_to_wifi(const char *ssid, const char *pass);
void open_wifi_settings();
void start_elegant_OTA();
void post_wifi_setup();

void wifi_from_EEPROM();

void start_elegant_OTA(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! I am ESP32: ready for OTA."); });

    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("Elegant OTA started");

}

void open_wifi_settings(){
    // switching to a different network
    Serial.println("Turning the HotSpot On for password change");
    launchWeb();
    setupAP();// Setup HotSpot

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
    if (WiFi.status() != WL_CONNECTED){ // not connected
        Serial.print("failed connecting to wifi: ");
        Serial.println(ssid);
        WiFi.disconnect();
        if(retry_count < 2){ // 2 retries allowed
            retry_count++;
            goto REDO;
        }
    }
    else{ // connected to wifi.. yay!!
        Serial.println();
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP Address is : ");
        Serial.println(WiFi.localIP()); //print local IP address
    }

}

void figure_out_wifi() // attempt to connect to wifi (1: hard code, 2: EEPROM )
{
    // using the hard code ssid and password
    
    Serial.println("Trying with EEPROM now..");

    String e_ssid= read_ee_ssid();
    String e_pass = read_ee_pass();
    connect_to_wifi(e_ssid.c_str(), e_pass.c_str());

    if (WiFi.status() != WL_CONNECTED){ // not connected
        // connect using credentials from hard code
        connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);           
    }
    
    // looking for need to reset wifi network
    Serial.println("Connect D4 to ground in 1 sec to switch wifi");
    bool wifi_reset = digitalRead(selectWifiPin);
    delay(1000);
    if (wifi_reset == LOW)
        open_wifi_settings();

    if (WiFi.status() != WL_CONNECTED){
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

    // setup elegant OTA
    start_elegant_OTA();
    Serial.println(WiFi.macAddress());

}

/********* everything above this line has been refactored ****************/
void reattempt_wifi_connect() // re-attempt connecting to wifi on every even token number
{
    Serial.println("Reattempt connecting to ");
    if (counter % 2 == 0)
    { // when the token number served is even
        String esid = read_ee_ssid();
        String epass = read_ee_pass();
        WiFi.begin(esid.c_str(), epass.c_str());
        Serial.println(esid);
        delay(500);
    }
    else
    {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.println(WIFI_SSID);
        delay(500);
    }
    if (WiFi.status() != WL_CONNECTED)
    { // not connected
        Serial.println("failed connecting to");
        Serial.println(WIFI_SSID);
        //      WiFi.disconnect();
    }
    else
    { // connected to the wifi
        Serial.println();
        Serial.print("Connected to ");
        Serial.println(WIFI_SSID);
        Serial.print("IP Address is : ");
        Serial.println(WiFi.localIP()); //print local IP address
        post_wifi_setup();
    }
}



