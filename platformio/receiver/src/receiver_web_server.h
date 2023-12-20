#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "global_variables.h"

const char *ssid_ap = "dr_irfan_counter";
const char *pass_ap = "usman@kms.com";

//Your IP address or domain name with URL path
const char *serverNameTemp = "http://192.168.55.5/counter";
unsigned long attempt_start_time = 0;

void client_setup();
void client_loop();
String httpGETRequest(const char *serverName);

unsigned long previous_client_time = 0;
const long interval = 3000;

void client_setup()
{
  // uint32_t retry_count = 0;
// REDO: // jump back to the top of the function
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_ap, pass_ap);
  Serial.println("Connecting to .." + String(ssid_ap));

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(5000);
    WiFi.reconnect();
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.print("Client connected with IP Address: ");
    Serial.println(WiFi.localIP());
  }
}
void client_loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previous_client_time >= interval)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      counter_str = httpGETRequest(serverNameTemp); //.toInt();
      Serial.println(", Currently Serving: " + counter_str);
      incoming_counter  = counter_str.toInt();
      // Serial.println(counter);
      // save the last HTTP GET Request
      previous_client_time = currentMillis;
    }
    else{
      Serial.println("not connected with WiFi");
      previous_client_time = currentMillis;
      WiFi.reconnect();
    }
  }
}

// returns the data read from the server
String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.print(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}