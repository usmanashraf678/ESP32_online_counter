/*
This test will check if the seven segment 
attached with shift register and ULN2003 are working

*/
#include <Keypad.h>
//
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//
char Keys[ROWS][COLS] = 
//{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
//
byte rowPins[ROWS] = {D0, D1, D2, D3}; //{R1, R2, R3, R4}
byte colPins[COLS] = {D4, D5, D6, D7}; //{C1, C2, C3, C4}

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);


#define NUM_OF_DISPLAY 3

//Pin connected to Strobe (pin 1) of 4094
//int strobePin = SD3; ////atmega(pin-25)
//Pin connected to Data (pin 2) of 4094
//int dataPin = D7;  //atmega(pin-24)
//Pin connected to Clock (pin 3) of 4094
//int clockPin = D8;  //atmega(pin-23)
char shiftOutBuffer[NUM_OF_DISPLAY]={0};
// 0-9 and none
byte segChar[]={ 

  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111,
  0b00000000,
};

void update_display()
{
  int i=0;      
  digitalWrite(strobePin, LOW);
  for(i=NUM_OF_DISPLAY-1;i>-1;i--)
    {     
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[shiftOutBuffer[i]]);             
    }
  digitalWrite(strobePin, HIGH);      
}


void reset_digits(){
  int i=0;
  for(i=0;i<NUM_OF_DISPLAY;i++)
  {
   shiftOutBuffer[i]=0; 
    
  }
  update_display();

  
}


void setup() {

  Serial.begin(115200);


  int i=0;
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
//  for(i=0;i<NUM_OF_DISPLAY;i++)
//  {
//   shiftOutBuffer[i]=i+1; 
//    
//  }

  
  shiftOutBuffer[2]=4;
  shiftOutBuffer[1]=1;
  shiftOutBuffer[0]=3;
  
  update_display();
  
}
void loop()
{
//  char customKey = customKeypad.getKey(); 
//  if(customKey)
//  { 
//    Serial.println(customKey);
//  }
}
