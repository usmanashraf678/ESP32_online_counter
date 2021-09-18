#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <wifi_helpers.h>

WebServer server_for_AP(80);
int statusCode;
String st;
String content;
/********** Wifi Credentials Chnage *********************/
bool testWifi(void);
void launchWeb();
void setupAP(void);
void createWebServer();

/********************* NO NEED TO CHANGE THESE FUNCTIONS *************************/
bool testWifi(void)
{
    int c = 0;
    Serial.println("Waiting for Wifi to connect");
    while (c < 20)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return true;
        }
        delay(500);
        Serial.print("*");
        c++;
    }
    Serial.println("");
    Serial.println("Connect timed out, opening AP");
    return false;
}

void launchWeb()
{
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
        Serial.println("WiFi connected");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
    createWebServer();
    // Start the server
    Serial.println("created server sucessfully");
    server_for_AP.begin();
    Serial.println("Server started");
}

void setupAP(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
        Serial.println("no networks found");
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            //      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    st = "<ol>";
    for (int i = 0; i < n; ++i)
    {
        // Print SSID and RSSI for each network found
        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);

        st += ")";
        //    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
    }
    st += "</ol>";
    delay(100);
    WiFi.softAP("keep my seat", "");
    Serial.println("softap");
    launchWeb();
    Serial.println("over");
}

void createWebServer()
{
    {
        server_for_AP.on("/", []()
                         {
                             IPAddress ip = WiFi.softAPIP();
                             String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
                             content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
                             content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
                             content += ipStr;
                             content += "<p>";
                             content += st;
                             content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
                             content += "</html>";
                             server_for_AP.send(200, "text/html", content);
                         });
        server_for_AP.on("/scan", []()
                         {
                             //setupAP();
                             IPAddress ip = WiFi.softAPIP();
                             String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

                             content = "<!DOCTYPE HTML>\r\n<html>go back";
                             server_for_AP.send(200, "text/html", content);
                         });

        server_for_AP.on("/setting", []()
                         {
                             String qsid = server_for_AP.arg("ssid");
                             String qpass = server_for_AP.arg("pass");
                             if (qsid.length() > 0 && qpass.length() > 0)
                             {
                                 Serial.println("clearing eeprom");
                                 for (int i = 0; i < 96; ++i)
                                 {
                                     EEPROM.write(i, 0);
                                 }
                                 Serial.println(qsid);
                                 Serial.println("");
                                 Serial.println(qpass);
                                 Serial.println("");

                                 Serial.println("writing eeprom ssid:");
                                 for (int i = 0; i < qsid.length(); ++i)
                                 {
                                     EEPROM.write(i, qsid[i]);
                                     Serial.print("Wrote: ");
                                     Serial.println(qsid[i]);
                                 }
                                 Serial.println("writing eeprom pass:");
                                 for (int i = 0; i < qpass.length(); ++i)
                                 {
                                     EEPROM.write(32 + i, qpass[i]);
                                     Serial.print("Wrote: ");
                                     Serial.println(qpass[i]);
                                 }
                                 EEPROM.commit();

                                 content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
                                 statusCode = 200;
                                 ESP.restart();
                             }
                             else
                             {
                                 content = "{\"Error\":\"404 not found\"}";
                                 statusCode = 404;
                                 Serial.println("Sending 404");
                             }
                             server_for_AP.sendHeader("Access-Control-Allow-Origin", "*");
                             server_for_AP.send(statusCode, "application/json", content);
                         });
    }
}

/********************* NO NEED TO CHANGE THESE FUNCTIONS *************************/
