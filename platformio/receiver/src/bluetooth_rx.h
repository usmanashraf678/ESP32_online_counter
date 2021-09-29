#pragma once
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String ServerMACadd = "08:3A:F2:52:EA:AE";
uint8_t ServerMAC[6] = {0x08, 0x3A, 0xF2, 0x52, 0xEA, 0xAE};
String ServerName = "ESP32_SPP";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;
bool isSppOpened = false;

void setup_BT_rx();
void BT_loop();
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event)
    {
    case ESP_SPP_INIT_EVT:
        Serial.println("ESP_SPP_INIT_EVT");
        break;

    case ESP_SPP_SRV_OPEN_EVT: //Server connection open
        Serial.println("ESP_SPP_SRV_OPEN_EVT");
        break;

    case ESP_SPP_CLOSE_EVT: //Client connection closed
        Serial.println("ESP_SPP_CLOSE_EVT");
        isSppOpened = false;
        break;

    case ESP_SPP_CONG_EVT: //connection congestion status changed
        Serial.println("ESP_SPP_CONG_EVT");
        break;

    case ESP_SPP_WRITE_EVT: //write operation completed
        // Serial.println("ESP_SPP_WRITE_EVT");
        break;

    case ESP_SPP_DATA_IND_EVT: //connection received data
        // Serial.println("ESP_SPP_DATA_IND_EVT");
        break;

    case ESP_SPP_DISCOVERY_COMP_EVT: //discovery complete
        Serial.println("ESP_SPP_DISCOVERY_COMP_EVT");
        break;

    case ESP_SPP_OPEN_EVT: //Client connection open
        Serial.println("ESP_SPP_OPEN_EVT");
        isSppOpened = true;
        break;

    case ESP_SPP_START_EVT: //server started
        Serial.println("ESP_SPP_START_EVT");
        break;

    case ESP_SPP_CL_INIT_EVT: //client initiated a connection
        Serial.println("ESP_SPP_CL_INIT_EVT");
        break;

    default:
        Serial.println("unknown event!");
        break;
    }
}

void setup_BT_rx()
{
    SerialBT.begin("ESP32_Client", true);
    Serial.println("BT Client started in master mode, make sure remote BT device is on!");
    SerialBT.register_callback(btCallback);
    //connected = SerialBT.connect(ServerName);
    connected = SerialBT.connect(ServerMAC);

    /*
     * In my trial, 
     * SerialBT.connect() always return  true, even no server exist.
     * To solve it, I implemented bluetooth event callback function,
     * double varify if ESP_SPP_OPEN_EVT raised.
     */

    if (connected)
    {
        Serial.println("SerialBT.connect() == true");
    }
    else
    {
        Serial.println("Failed to connect BT from setup..");
        Serial.println("SerialBT.connect() == false");
        connected = SerialBT.connect(ServerMAC);
    }

    //may be there are some delay to call callback function,
    //delay before check
    delay(500);
    if (isSppOpened == false)
    {
        Serial.println("isSppOpened == false");
        Serial.println("Reset to re-try");
        connected = false;
        SerialBT.connect(ServerMAC);
    }
    else
    {
        Serial.println("isSppOpened == true");
        Serial.println("CONNECTED to Transmitter from setup");
    }
}

void BT_loop()
{
    while (!isSppOpened)
    {
        Serial.println("isSppOpened == false : DISCONNECTED");
        Serial.println("Reset to re-connect to BT");
        connected = false;
        SerialBT.connect(ServerMAC);
        delay(10000);
        if (isSppOpened == true)
        {
            Serial.println("CONNECTED to Transmitter from loop");
        }
    }

    if (Serial.available())
    {
        SerialBT.write(Serial.read());
    }
    if (SerialBT.available())
    {
        char c = SerialBT.read();
        Serial.write(c);
        // Serial.write(SerialBT.read());
    }
    delay(20);
}
