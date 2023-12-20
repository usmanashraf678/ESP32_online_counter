#pragma once

#include <Arduino.h>
#include <FirebaseESP32.h>
#include "global_variables.h"
#include <WebServer.h>
/*************** Async web server *************************************/
WebServer server_for_AP(80);

/************ Counter on firebase declarations *****************************/
boolean updated_on_cloud = false;
boolean updated_locally = false;
bool try_wifi = false;
int counter = 0; // global counter value
int incoming_counter = 0;
String counter_str;
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

/********** Large Display Setting ***************************/
#define NUM_OF_DISPLAY 3

int strobePin = 27; //Pin connected to Strobe 
int dataPin = 14;  //Pin connected to Data 
int clockPin =26;  //Pin connected to Clock 
int buzzerPin = 4;
int selectWifiPin = 18;

int shiftOutBuffer[NUM_OF_DISPLAY]={0};
// 0-9 and none
byte segChar[]={ //.gfedcba 0, 1, 2, .. 9, all off
  // 0b00000010,//1
  // 0b00000100,//2
  // 0b00001000,//3
  // 0b00010000,//4
  // 0b00000001,//0
  // 0b00100000,//5
  // 0b01000000,//6
  // 0b10000000,  0b01111111,  0b01101111,  0b00000000,

  0b00111111,  0b00000110,  0b01011011,  0b01001111,  0b01100110,  0b01101101,
  0b01111101,  0b00000111,  0b01111111,  0b01101111,  0b00000000,

  // 0b00111111,  0b00000110,  0b01011011,  0b01001111,  0b01100110,  0b01101101,
  // 0b01111101,  0b00000111,  0b01111111,  0b01101111,  0b00000000,
};

/**************** Firebase Settings ****************************************/
#define FIREBASE_HOST "queue-mgmt-947d1-default-rtdb.asia-southeast1.firebasedatabase.app"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "RMaoeR1ArcF2fo0BUaVa5eKI1KAutrmhcgMNoIh9" //Your Firebase Database Secret goes here


#define WIFI_TIMEOUT_MS 5000

// Declare the Data object in the global scope
FirebaseData fbdo;

// Declare global variable to store value, initial counter value
int val=0;

/*************************** Global variables for Wifi Credentials change ***********/
//Variables
int i = 0;
int statusCode;
const char* WIFI_SSID = "HUAWEI-8bz8";//"HUAWEI-8bz8" // Change THIS!! netis_C8B281
const char* WIFI_PASSWORD = ""; //"qpwjUsMH" // Change THIS!! 04517196

String e_ssid;
String e_pass;