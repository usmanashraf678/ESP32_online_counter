#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "global_variables.h"

String get_time();
void post_wifi_setup();
void publish_to_firebase(int val);

// Declare the Data object in the global scope
FirebaseData fbdo;


String get_time() // returns a string of current time e.g. Sat 20-Apr-19 12:31:45
{
    time_t now;
    time(&now);
    char time_output[30];
    // See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions
    strftime(time_output, 30, "%r %a %d-%m-%y", localtime(&now));
    return String(time_output);
}

void publish_to_firebase()
{
    // Firebase Error Handling And Writing Data At Specifed Path************************************************
    if (WiFi.status() == WL_CONNECTED) // updated_on_cloud == false &&
    {
        String global_timestamp = get_time();
        if (Firebase.setIntAsync(fbdo, "/dr_irfan_counter", counter) && Firebase.setStringAsync(fbdo, "/dr_irfan_time", global_timestamp))
        {
            Serial.println("\n");
            Serial.println("Value Uploaded Successfully");
            Serial.print("Val = ");
            Serial.println(counter);
            // updated_on_cloud = true;
            Serial.println(global_timestamp);
        }
        else
        {
            Serial.println("Connected but no internet");
            Serial.println(fbdo.errorReason());
        }
    }

    else
        Serial.println("no wifi for firebase upload");
}