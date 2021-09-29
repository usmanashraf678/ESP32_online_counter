#pragma once
#include <Arduino.h>
#include "global_variables.h"
#include "eeprom_helpers.h"
#include <time.h>
#include <WiFi.h>

#include <FirebaseESP32.h>
#include <AsyncElegantOTA.h>
#include "large_ssd.h"
#include "change_wifi.h"

void figure_out_wifi();
void connect_to_wifi(const char *ssid, const char *pass);
void open_wifi_settings();
void start_elegant_OTA();
void post_wifi_setup();
void streaming_setup();
void streamCallback(StreamData data);
void streamTimeoutCallback(bool timeout);
void streaming_loop();
void reattempt_wifi_connect();
void wifi_from_EEPROM();

unsigned long previousMillis = 0;
AsyncWebServer server(80);

String get_time() // returns a string of current time e.g. Sat 20-Apr-19 12:31:45
{
  time_t now;
  time(&now);
  char time_output[30];
  // See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions
  strftime(time_output, 30, "%r %a %d-%m-%y", localtime(&now));
  return String(time_output);
}

void start_elegant_OTA()
{
  WiFi.softAPdisconnect (true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! I am ESP32: ready for OTA."); });

  Serial.println(WiFi.localIP());

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
  WiFi.mode(WIFI_MODE_APSTA);
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
    if (retry_count < 2)
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
    Serial.println(WiFi.channel());
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
    Serial.println(wifi_reset);
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
  configTime(18000, 0, "pool.ntp.org", "time.nist.gov");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // connect to firebase
  Firebase.reconnectWiFi(true);

  // streaming_setup();

  // Serial.println(WiFi.macAddress());

  delay(500);
}

void streaming_setup()
{

  //In setup(), set the stream callback function to handle data
  //streamCallback is the function that called when database data changes or updates occurred
  //streamTimeoutCallback is the function that called when the connection between the server
  //and client was timeout during HTTP stream

  Firebase.setStreamCallback(fbdo, streamCallback, streamTimeoutCallback);

  //In setup(), set the streaming path to "/test/data" and begin stream connection

  if (!Firebase.beginStream(fbdo, "/dr_umair_counter"))
  {
    //Could not begin stream connection, then print out the error detail
    Serial.println(fbdo.errorReason());
  }
}

//Global function that handles stream data
void streamCallback(StreamData data)
{

  //Print out all information

  Serial.println("Stream Data...");
  Serial.println(data.streamPath());
  Serial.println(data.dataPath());
  Serial.println(data.dataType());

  //Print out the value
  //Stream data can be many types which can be determined from function dataType

  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
    Serial.println(data.to<int>());
  else if (data.dataTypeEnum() == fb_esp_rtdb_data_type_string)
    Serial.println(data.to<String>());
}

//Global function that notifies when stream connection lost
//The library will resume the stream connection automatically
void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    //Stream timeout occurred
    Serial.println("Stream timeout, resume streaming...");
  }
}

void streaming_loop()
{
  if (!Firebase.readStream(fbdo))
  {
    Serial.println(fbdo.errorReason());
  }

  if (fbdo.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (fbdo.streamAvailable())
  {

    if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
      Serial.println(fbdo.to<int>());
    else if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_string)
      Serial.println(fbdo.to<String>());
  }
}

/********* everything above this line has been refactored ****************/
void reattempt_wifi_connect() // re-attempt connecting to wifi on every even token number
{
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= 7000))
  {
    Serial.println(currentMillis - previousMillis);
    Serial.println("Reconnecting to WiFi..." + e_ssid);
    // WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }
}
