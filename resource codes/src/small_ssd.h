#pragma once
#include<Arduino.h>
#include <global_variables.h>
#include <large_ssd.h>

/****************** Constant declarations for small SSD ****************/
// Pins to control each digit of small SSD (digit_one, digit_two, digit_three)
const int DIGIT_ONE = 23, DIGIT_TWO = 5, DIGIT_THREE = 17; // CHANGE THIS!!

// BCD Encoder Input for small SSDs (Binary Num is DCBA)
const int BIT_A = 18, BIT_B = 22, BIT_C = 21, BIT_D = 19;

// Array to control digit's common pins
const int digit[] = { DIGIT_ONE, DIGIT_TWO, DIGIT_THREE};

// Array to control input of 7447
const int input_to_7447[] = { BIT_A, BIT_B, BIT_C, BIT_D};

/************Function Prototypes***************************************************/
// input: DIGIT_ONE, DIGIT_TWO, OR DIGIT_THREE
void turnOFF_Digit(int number);
void turnON_Digit(int number);  

void lights_off();  // turns off all 3 digits
void lights_on(); // turns on all 3 digits
void displayNum(int number);  // input integer between 0-9
void count2nine();  // counts from 0-9
void displayNum3(); // displays 3 numbers simultaneously

/***************** Functions for small SSDs **********************/
void turnOFF_Digit(int number){ // Turns OFF a single digit 
  switch (number) {
    // first digit
    case DIGIT_ONE:  
      digitalWrite(DIGIT_ONE, LOW);
      break;
    // second digit 
    case DIGIT_TWO:
      digitalWrite(DIGIT_TWO, LOW);
      break;
    // third digit 
    case DIGIT_THREE:
      digitalWrite(DIGIT_THREE, LOW);
      break;
  }
}

void turnON_Digit(int number){ // Turns on a single digit 
  switch (number) {
    // first digit
    case DIGIT_ONE:  
      digitalWrite(DIGIT_ONE, HIGH);
      break;
    // second digit 
    case DIGIT_TWO:
      digitalWrite(DIGIT_TWO, HIGH);
      break;
    // third digit 
    case DIGIT_THREE:
      digitalWrite(DIGIT_THREE, HIGH);
      break;
      }
}

void lights_off(){// Turns off all three digits  
  for (int i = 0; i < 3; i++) 
  {digitalWrite(digit[i], LOW);}
}

void lights_on(){// Turns on all three digits  
  for (int i = 0; i < 3; i++) 
  {digitalWrite(digit[i], HIGH);}
}

void displayNum(int number) { // encodes number and outputs on bit_abcd pins
  switch (number) {
    case 0:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, LOW);
      digitalWrite(BIT_B, LOW);
      digitalWrite(BIT_A, LOW);
      break;
    case 1:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, LOW);
      digitalWrite(BIT_B, LOW);
      digitalWrite(BIT_A, HIGH);
      break;
    case 2:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, LOW);
      digitalWrite(BIT_B, HIGH);
      digitalWrite(BIT_A, LOW);
      break;  
   case 3:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, LOW);
      digitalWrite(BIT_B, HIGH);
      digitalWrite(BIT_A, HIGH);
      break;
   case 4:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, HIGH);
      digitalWrite(BIT_B, LOW);
      digitalWrite(BIT_A, LOW);
      break;
  case 5:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, HIGH);
      digitalWrite(BIT_B, LOW);
      digitalWrite(BIT_A, HIGH);
      break;  
  case 6:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, HIGH);
      digitalWrite(BIT_B, HIGH);
      digitalWrite(BIT_A, LOW);
      break;   
  case 7:
      digitalWrite(BIT_D, LOW);
      digitalWrite(BIT_C, HIGH);
      digitalWrite(BIT_B, HIGH);
      digitalWrite(BIT_A, HIGH);
      break;
  case 8:
      digitalWrite(BIT_D, HIGH);
      digitalWrite(BIT_C, LOW);
      digitalWrite(BIT_B, LOW);
      digitalWrite(BIT_A, LOW);
      break;
  case 9:
      digitalWrite(BIT_D, HIGH);
      digitalWrite(BIT_C, LOW);
      digitalWrite(BIT_B, LOW);
      digitalWrite(BIT_A, HIGH);
      break;
      }
}

void count2nine(){ // displays number 0, 1, 2, .. 9
  for (int i = 0; i<10; i++){
    displayNum(i);
    delay(500);}
}

void displayNum3(){// Displays 3 different numbers simultaneously 
  update_global_digits();
  
  //display first number on digit 1
  turnON_Digit(DIGIT_THREE);
  turnOFF_Digit(DIGIT_TWO);
  turnOFF_Digit(DIGIT_ONE); 
  displayNum(d_1); 
  delay(5);
  //display second number on digit 2
  turnOFF_Digit(DIGIT_THREE);
  turnON_Digit(DIGIT_TWO);
  turnOFF_Digit(DIGIT_ONE); 
  displayNum(d_10); 
  delay(5);
  //display third number on digit 3
  turnOFF_Digit(DIGIT_THREE);
  turnOFF_Digit(DIGIT_TWO);
  turnON_Digit(DIGIT_ONE); 
  displayNum(d_100); 
  delay(5);
}

