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

// This example shows how error retry and queues work.

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

/* 2. Define the API Key */
#define API_KEY "API_KEY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in
 * your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

// Define FirebaseESP8266 data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

String path = "/Test";

std::vector<uint8_t> myblob;
double mydouble = 0;

uint32_t queueID[20];
uint8_t qIdx = 0;

void callback(QueueInfo queueinfo)
{

  if (queueinfo.isQueueFull())
  {
    Serial.println("Queue is full");
  }

  Serial.print("Remaining queues: ");
  Serial.println(queueinfo.totalQueues());

  Serial.print("Being processed queue ID: ");
  Serial.println(queueinfo.currentQueueID());

  Serial.print("Data type:");
  Serial.println(queueinfo.dataType());

  Serial.print("Method: ");
  Serial.println(queueinfo.firebaseMethod());

  Serial.print("Path: ");
  Serial.println(queueinfo.dataPath());

  Serial.println();
}

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
  config.token_status_callback = tokenStatusCallback; // see
                                                      // addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Or use legacy authenticate method
  // Firebase.begin(DATABASE_URL, DATABASE_SECRET);

  Firebase.reconnectWiFi(true);

#if defined(ESP8266)
  // Set the size of WiFi rx/tx buffers in the case where we want to work with
  // large data.
  fbdo.setBSSLBufferSize(1024, 1024);
#endif

  // Set the size of HTTP response buffers in the case where we want to work
  // with large data.
  fbdo.setResponseSize(1024);

  // Open and retore Firebase Error Queues from file.
  // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
  if (Firebase.errorQueueCount(fbdo, "/test.txt", StorageType::FLASH) > 0)
  {
    Firebase.restoreErrorQueue(fbdo, "/test.txt", StorageType::FLASH);
    Firebase.deleteStorageFile("/test.txt", StorageType::FLASH);
  }

  // Set maximum Firebase read/store retry operation (0 - 255) in case of
  // network problems and buffer overflow
  Firebase.setMaxRetry(fbdo, 3);

  // Set the maximum Firebase Error Queues in collection (0 - 255).
  // Firebase read/store operation causes by network problems and buffer
  // overflow will be added to Firebase Error Queues collection.
  Firebase.setMaxErrorQueue(fbdo, 10);

  Firebase.beginAutoRunErrorQueue(fbdo, callback);

  // Firebase.beginAutoRunErrorQueue(fbdo);
}

void loop()
{
  if (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;

    Serial.println("------------------------------------");
    Serial.println("Set double test...");

    String node = path + "/Double/Data";

    if (Firebase.setDouble(fbdo, node.c_str(), 340.123456789))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      Serial.print("VALUE: ");
      printResult(fbdo); // see addons/RTDBHelper.h
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      if (Firebase.getErrorQueueID(fbdo) > 0)
      {
        Serial.println("Error Queue ID: " +
                       String(Firebase.getErrorQueueID(fbdo)));
        queueID[qIdx] = Firebase.getErrorQueueID(fbdo);
        qIdx++;
      }
      Serial.println("------------------------------------");
      Serial.println();
    }

    Serial.println("------------------------------------");
    Serial.println("Set BLOB data test...");

    // Create demo data
    uint8_t data[256];
    for (int i = 0; i < 256; i++)
      data[i] = i;

    node = path + "/Binary/Blob/data";

    // Set binary data to database
    if (Firebase.setBlob(fbdo, node.c_str(), data, sizeof(data)))
    {
      Serial.println("PASSED");
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      if (Firebase.getErrorQueueID(fbdo) > 0)
      {
        Serial.println("Error Queue ID: " +
                       String(Firebase.getErrorQueueID(fbdo)));
        queueID[qIdx] = Firebase.getErrorQueueID(fbdo);
        qIdx++;
      }
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("---------------------------------------------------------"
                     "-----------------");
      Serial.println("To test error queue, turn off WiFi AP to make error in "
                     "the next operation");
      Serial.println("---------------------------------------------------------"
                     "-----------------");
      Serial.println();

      delay(10000);
    }

    Serial.println("------------------------------------");
    Serial.println("Get BLOB data test...");

    node = path + "/Binary/Blob/data";

    // Get binary data from database
    // Assign myblob as the target variable
    if (Firebase.getBlob(fbdo, node.c_str(), myblob))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      Serial.print("VALUE: ");
      if (fbdo.dataType() == "blob")
      {

        Serial.println();

        for (size_t i = 0; i < myblob.size(); i++)
        {
          if (i > 0 && i % 16 == 0)
            Serial.println();

          if (i < 16)
            Serial.print("0");

          Serial.print(myblob[i], HEX);
          Serial.print(" ");
        }
        myblob.clear();
        Serial.println();
      }
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      if (Firebase.getErrorQueueID(fbdo) > 0)
      {
        Serial.println("Error Queue ID: " +
                       String(Firebase.getErrorQueueID(fbdo)));
        queueID[qIdx] = Firebase.getErrorQueueID(fbdo);
        qIdx++;
      }
      Serial.println("------------------------------------");
      Serial.println();
    }

    Serial.println("------------------------------------");
    Serial.println("Get double test...");

    node = path + "/Double/Data";

    if (Firebase.getDouble(fbdo, node.c_str(), mydouble))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
      Serial.print("VALUE: ");
      printResult(fbdo); // see addons/RTDBHelper.h
      Serial.println("------------------------------------");
      Serial.println();
      mydouble = 0;
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      if (Firebase.getErrorQueueID(fbdo) > 0)
      {
        Serial.println("Error Queue ID: " +
                       String(Firebase.getErrorQueueID(fbdo)));
        queueID[qIdx] = Firebase.getErrorQueueID(fbdo);
        qIdx++;
      }
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.errorQueueCount(fbdo) > 0)
    {
      Serial.println("---------------------------------------------------------"
                     "--------------------");
      Serial.println(
          "Now turn on WiFi hotspot or router to process these queues");
      Serial.println("---------------------------------------------------------"
                     "--------------------");
      Serial.println();

      // Save Error Queues to file
      // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
      Firebase.saveErrorQueue(fbdo, "/test.txt", StorageType::FLASH);
    }

    // Stop error queue auto run process
    // Firebase.endAutoRunErrorQueue(fbdo);
  }

  if (!Firebase.ready())
    return;

  if (mydouble > 0)
  {
    Serial.println("------------------------------------");
    Serial.println("Double Data gets from Queue");
    printf("%.9lf\n", mydouble);
    Serial.println();
    mydouble = 0;
  }

  if (Firebase.errorQueueCount(fbdo) > 0)
  {

    /*

    if Firebase.beginAutoRunErrorQueue was not call,
    to manaul run the Firebase Error Queues, just call
    Firebase.processErrorQueue in loop


    Firebase.processErrorQueue(fbdo);

    delay(1000);

    if (Firebase.isErrorQueueFull(fbdo))
    {
      Serial.println("Queue is full");
    }

    Serial.print("Remaining queues: ");
    Serial.println(Firebase.errorQueueCount(fbdo));

    for (uint8_t i = 0; i < qIdx; i++)
    {
      Serial.print("Error Queue ");
      Serial.print(queueID[i]);
      if (Firebase.isErrorQueueExisted(fbdo, queueID[i]))
        Serial.println(" is queuing");
      else
        Serial.println(" is done");
    }
    Serial.println();

    */

    if (mydouble > 0)
    {
      Serial.println("------------------------------------");
      Serial.println("Double Data gets from Queue");
      Serial.println(mydouble, 9);
      Serial.println();
      mydouble = 0;
    }

    if (myblob.size() > 0)
    {
      Serial.println("------------------------------------");
      Serial.println("Blob Data gets from Queue");
      Serial.println();
      for (size_t i = 0; i < myblob.size(); i++)
      {
        if (i > 0 && i % 16 == 0)
          Serial.println();
        if (myblob[i] < 16)
          Serial.print("0");
        Serial.print(myblob[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      Serial.println();
      myblob.clear();
    }
  }
}