// include statements
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <EEPROM.h>

#include <Keypad.h> // to use keypad
#include <FirebaseESP32.h>
#include <time.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

AsyncWebServer server(80);
WebServer server_for_AP(80);

WiFiManager wm;


/****************** Constant declarations for small SSD ****************/
// Pins to control each digit of small SSD (digit_one, digit_two, digit_three)
const int DIGIT_ONE = 23, DIGIT_TWO = 5, DIGIT_THREE = 17; // CHANGE THIS!!

// BCD Encoder Input for small SSDs (Binary Num is DCBA)
const int BIT_A = 18, BIT_B = 22, BIT_C = 21, BIT_D = 19;

// Array to control digit's common pins
const int digit[] = { DIGIT_ONE, DIGIT_TWO, DIGIT_THREE};

// Array to control input of 7447
const int input_to_7447[] = { BIT_A, BIT_B, BIT_C, BIT_D};

/************ Counter on firebase declarations *****************************/
boolean updated_on_cloud = false;
boolean updated_locally = false;
int counter; // global counter value
String global_timestamp; // global time stamp of update
int d_100, d_10, d_1; // the digits of the number with the number being d_100, d_10, d_1

// keypad constant value and layout
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char Keys[ROWS][COLS] = 
{ {'1','2','3','A'},  {'4','5','6','B'},  {'7','8','9','C'},  {'*','0','#','D'}};

//ATTN: pins used with keypad, these can be changed based on pcb version
byte rowPins[ROWS] = {32, 33, 25, 26}; //{R1, R2, R3, R4} 34, 35, 32, 33 // CHANGE THIS!!
byte colPins[COLS] = {27, 14, 12, 13}; 

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 
/********** Large Display Setting ***************************/
#define NUM_OF_DISPLAY 3

int strobePin = 15; //Pin connected to Strobe (pin 1) of 4094
int dataPin = 2;  //Pin connected to Data (pin 2) of 4094
int clockPin = 4;  //Pin connected to Clock (pin 3) of 4094
int buzzerPin = 16;

char shiftOutBuffer[NUM_OF_DISPLAY]={0};
// 0-9 and none
byte segChar[]={ //.gfedcba 0, 1, 2, .. 9, all off
  0b00111111,  0b00000110,  0b01011011,  0b01001111,  0b01100110,  0b01101101,
  0b01111101,  0b00000111,  0b01111111,  0b01101111,  0b00000000,
};

/**************** Firebase Settings ****************************************/
#define FIREBASE_HOST "queue-mgmt-947d1-default-rtdb.asia-southeast1.firebasedatabase.app"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "RMaoeR1ArcF2fo0BUaVa5eKI1KAutrmhcgMNoIh9" //Your Firebase Database Secret goes here


#define WIFI_TIMEOUT_MS 10000

// Declare the Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value, initial counter value
int val=0;

/*************************** Global variables for Wifi Credentials change ***********/
//Variables
int i = 0;
int statusCode;
const char* WIFI_SSID = "HUAWEI-8bz8";//"HUAWEI-8bz8" // Change THIS!! netis_C8B281
const char* WIFI_PASSWORD = "qpwjUsMH"; //"qpwjUsMH" // Change THIS!! 04517196
String st;
String content;
