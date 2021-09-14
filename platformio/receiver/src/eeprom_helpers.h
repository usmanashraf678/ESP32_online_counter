#pragma once
#include <Arduino.h>
#include <EEPROM.h>


String read_ee_ssid();
String read_ee_pass();

String read_ee_ssid(){ // reads wifi name from the EEPROM
  String ee_ssid;

  for (int i = 0; i < 32; ++i){
    ee_ssid += char(EEPROM.read(i));
    }
    Serial.print("\nEEPROM SSID: ");
    Serial.println(ee_ssid);
    return ee_ssid;
}

String read_ee_pass(){ // reads wifi password from the EEPROM
  String ee_pass = "";
  for (int i = 32; i < 96; ++i){
    ee_pass += char(EEPROM.read(i));
    }
    Serial.print("EEPROM Pass:");
    Serial.println(ee_pass);
    return ee_pass;
}