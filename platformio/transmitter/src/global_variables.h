#pragma once

#include <Arduino.h>
#include <Keypad.h>
#include <FirebaseESP32.h>
#include "global_variables.h"


/************ Counter on firebase declarations *****************************/
int counter = 0; // global counter value
boolean updated_on_cloud = false;
boolean updated_locally = false;
boolean counter_needs_push = false;
String global_timestamp; // global time stamp of update
int d_100, d_10, d_1; // the digits of the number with the number being d_100, d_10, d_1

// keypad constant value and layout
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char Keys[ROWS][COLS] = { {'1','2','3','A'},  {'4','5','6','B'},  {'7','8','9','C'},  {'*','0','#','D'}};

//ATTN: pins used with keypad, these can be changed based on pcb version
byte rowPins[ROWS] = {32, 33, 25, 26}; //{R1, R2, R3, R4} 34, 35, 32, 33 // CHANGE THIS!!
byte colPins[COLS] = {27, 14, 12, 13}; 

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 
/********** Large Display Setting ***************************/
#define NUM_OF_DISPLAY 3

int strobePin = 27; //Pin connected to Strobe (pin 1) of 4094
int dataPin = 14;  //Pin connected to Data (pin 2) of 4094
int clockPin =26;  //Pin connected to Clock (pin 3) of 4094
int buzzerPin = 16;
int selectWifiPin = 4;

int shiftOutBuffer[NUM_OF_DISPLAY]={0};
// 0-9 and none
byte segChar[]={ //.gfedcba 0, 1, 2, .. 9, all off
  0b00111111,  0b00000110,  0b01011011,  0b01001111,  0b01100110,  0b01101101,
  0b01111101,  0b00000111,  0b01111111,  0b01101111,  0b00000000,

  // 0b00111111,  0b00000110,  0b01011011,  0b01001111,  0b01100110,  0b01101101,
  // 0b01111101,  0b00000111,  0b01111111,  0b01101111,  0b00000000,
};


#define WIFI_TIMEOUT_MS 5000

/*************************** Global variables for Wifi Credentials change ***********/
//Variables
const char* WIFI_SSID = "fake network";//"HUAWEI-8bz8" // Change THIS!! netis_C8B281
const char* WIFI_PASSWORD = ""; //"qpwjUsMH" // Change THIS!! 04517196

const char* ssid_ap = "dr_irfan_counter";//"HUAWEI-8bz8" // Change THIS!! netis_C8B281
const char* pass_ap = "usman@kms.com"; //"qpwjUsMH" // Change THIS!! 04517196


String e_ssid;
String e_pass;

int32_t channel;