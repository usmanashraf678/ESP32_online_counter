/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

//This example shows how to set and delete data with checking the matching between node path ETag (unique identifier string)
//and provided Etag

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

/* 2. Define the API Key */
#define API_KEY "API_KEY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

void setup()
{

    Serial.begin(115200);
    Serial.println();
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    //Or use legacy authenticate method
    //Firebase.begin(DATABASE_URL, DATABASE_SECRET);

    Firebase.reconnectWiFi(true);

#if defined(ESP8266)
    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    fbdo.setBSSLBufferSize(1024, 1024);
#endif

    //Set the size of HTTP response buffers in the case where we want to work with large data.
    fbdo.setResponseSize(1024);
}

void loop()
{
    if (Firebase.ready() && !taskCompleted)
    {
        taskCompleted = true;

        String path = "/Test";

        String ETag = "";
        String wrong_ETag = "ANY_WRONG_ETag";

        Serial.println("------------------------------------");
        Serial.println("Set integer without ETag test...");
        String Path = path + "/Int/Data";

        if (Firebase.setInt(fbdo, Path.c_str(), 100))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.println("CURRENT ETag: " + fbdo.ETag());
            ETag = fbdo.ETag();
            Serial.print("VALUE: ");
            printResult(fbdo); //see addons/RTDBHelper.h
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }

        Serial.println("------------------------------------");
        Serial.println("Set integer with valid ETag test...");

        if (Firebase.setInt(fbdo, Path.c_str(), 200))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.println("CURRENT ETag: " + fbdo.ETag());
            ETag = fbdo.ETag();
            Serial.print("VALUE: ");
            printResult(fbdo); //see addons/RTDBHelper.h
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }

        Serial.println("------------------------------------");
        Serial.println("Set integer with wrong ETag test...");

        if (Firebase.setInt(fbdo, Path.c_str(), 300, wrong_ETag.c_str()))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.println("ETag: " + fbdo.ETag());
            ETag = fbdo.ETag();
            Serial.print("VALUE: ");
            printResult(fbdo); //see addons/RTDBHelper.h
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());

            //If provided ETag is not match to current ETag (httpCode 412)
            if (fbdo.httpCode() == 412)
            {
                Serial.println("PATH: " + fbdo.dataPath());
                Serial.println("TYPE: " + fbdo.dataType());
                Serial.println("PROVIDED ETag: " + wrong_ETag);
                Serial.println("CURRENT ETag: " + fbdo.ETag());
                ETag = fbdo.ETag();
                Serial.print("CURRENT VALUE: ");
                printResult(fbdo); //see addons/RTDBHelper.h
            }

            Serial.println("------------------------------------");
            Serial.println();
        }

        Serial.println("------------------------------------");
        Serial.println("Delete node with wrong ETag test...");
        if (Firebase.deleteNode(fbdo, Path.c_str(), wrong_ETag.c_str()))
        {

            Serial.println("PASSED");
            Serial.println("CURRENT ETag: " + fbdo.ETag());
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {

            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }
    }
}
