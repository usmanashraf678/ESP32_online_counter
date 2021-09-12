#pragma once
#include <Arduino.h>
#include "global_variables.h"
#include "eeprom_helpers.h"
#include <time.h>
#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager.git

#include <FirebaseESP32.h>
#include <AsyncElegantOTA.h>

String get_time() // returns a string of current time e.g. Sat 20-Apr-19 12:31:45
{
    time_t now;
    time(&now);
    char time_output[30];
    // See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions
    strftime(time_output, 30, "%r %a %d-%m-%y", localtime(&now));
    return String(time_output);
}

void wifi_from_hard_code() // try to connect with wifi present in hard code
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("first attempt from hard code");

    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    //try to connect with wifi - first attempt
    unsigned long attempt_start_time = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - attempt_start_time < WIFI_TIMEOUT_MS)
    {
        // wait for some time untill WiFi is connected
        Serial.print(".");
        delay(500);
    }
}

void wifi_from_EEPROM() // try to connect with wifi present in EEPROM
{
    Serial.println("trying wifi credentials from EEPROM");

    WiFi.mode(WIFI_STA);
    unsigned long attempt_start_time = millis();

    String esid = read_ee_ssid();
    String epass = read_ee_pass();

    WiFi.begin(esid.c_str(), epass.c_str());

    Serial.print("Connecting to ");
    Serial.println(esid);
    Serial.println("via EEPROM credentials ");

    while (WiFi.status() != WL_CONNECTED && millis() - attempt_start_time < WIFI_TIMEOUT_MS)
    {
        // wait for some time untill WiFi is connected
        Serial.print(".");
        delay(500);
    }
}

void post_wifi_setup() // configure time, firebase, elegant ota on wifi connection
{
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
    setenv("TZ", "PKT-5", 1);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to firebase
    Firebase.reconnectWiFi(true);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! I am ESP32: ready for OTA."); });

    AsyncElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");

    delay(500);
}

void attempt_wifi_connect() // attempt to connect to wifi (1: hard code, 2: EEPROM )
{
    wifi_from_hard_code();

    if (WiFi.status() != WL_CONNECTED)
    { // not connected
        Serial.println("failed connecting to wifi from hard code");
        WiFi.disconnect();
        wifi_from_EEPROM();
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println();
        Serial.print("Connected to ");
        Serial.println(WIFI_SSID);
        Serial.print("IP Address is : ");
        Serial.println(WiFi.localIP()); //print local IP address
        post_wifi_setup();
    }
    else
    {
        Serial.print("Still not connected after trying Hard code and EEPROM ");
    }
}

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
