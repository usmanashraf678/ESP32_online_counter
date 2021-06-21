// include statements
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Keypad.h> // to use keypad

/****************** Constant declarations for small SSD ****************/
// Pins to control each digit of small SSD (digit_one, digit_two, digit_three)
const int DIGIT_ONE = 23, DIGIT_TWO = 13, DIGIT_THREE = 12;

// BCD Encoder Input for small SSDs (Binary Num is DCBA)
const int BIT_A = 18, BIT_B = 22, BIT_C = 21, BIT_D = 19;

// Array to control digit's common pins
const int digit[] = { DIGIT_ONE, DIGIT_TWO, DIGIT_THREE};

// Array to control input of 7447
const int input_to_7447[] = { BIT_A, BIT_B, BIT_C, BIT_D};

/************ Counter on firebase declarations *****************************/
boolean counter_uploaded = false;
int counter; // global counter value

// keypad constant value and layout
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char Keys[ROWS][COLS] = 
{ {'1','2','3','A'},  {'4','5','6','B'},  {'7','8','9','C'},  {'*','0','#','D'}};

//ATTN: pins used with keypad, these can be changed based on pcb version
byte rowPins[ROWS] = {16, 17, 32, 33}; //{R1, R2, R3, R4} 34, 35, 32, 33
byte colPins[COLS] = {25, 26, 27, 14};

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 
/********** Large Display Setting ***************************/
#define NUM_OF_DISPLAY 3

int strobePin = 15; //Pin connected to Strobe (pin 1) of 4094
int dataPin = 2;  //Pin connected to Data (pin 2) of 4094
int clockPin = 4;  //Pin connected to Clock (pin 3) of 4094

char shiftOutBuffer[NUM_OF_DISPLAY]={0};
// 0-9 and none
byte segChar[]={
  0b00111111,  0b00000110,  0b01011011,  0b01001111,  0b01100110,  0b01101101,
  0b01111101,  0b00000111,  0b01111111,  0b01101111,  0b00000000,
};

/**************** Firebase Settings ****************************************/
#define FIREBASE_HOST "queue-mgmt-947d1-default-rtdb.asia-southeast1.firebasedatabase.app"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "RMaoeR1ArcF2fo0BUaVa5eKI1KAutrmhcgMNoIh9" //Your Firebase Database Secret goes here

#define WIFI_SSID "Umer rice mill" // "HUAWEI-8bz8"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "Premier1"// "qpwjUsMH"                                      //Password of your wifi network 

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value, initial counter value
int val=0;
/************Function Prototypes***************************************************/
// input: DIGIT_ONE, DIGIT_TWO, OR DIGIT_THREE
void turnON_Digit(int number);  
void turnOFF_Digit(int number);

void lights_off();  // turns off all 3 digits
void lights_on(); // turns on all 3 digits
void displayNum(int number);  // input integer between 0-9
void count2nine();  // counts from 0-9
void displayNum3(int one, int two, int three); // displays 3 numbers simultaneously

/**************** Functions of updating display ******************/
void update_display(); // update the large display
void update_display_and_counter(); // update the online counter and display
void blink_display(); // blink the large display
void shift_left(int new_entrant); // shift new number to accomodate new entrant on left
void increment_counter(); // increase the counter value
void decrement_counter(); // decrease the counter value
void reset_digits(); // reset the digits of the counter
void write_to_firebase(int val); 
