#include <Keypad.h>

int counter; // global counter value
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char Keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {13, 12, 14, 27}; //{R1, R2, R3, R4}
byte colPins[COLS] = {26, 25, 33, 32}; //{C1, C2, C3, C4}

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 

#define NUM_OF_DISPLAY 3

//Pin connected to Strobe (pin 1) of 4094
int strobePin = 15; ////atmega(pin-25)
//Pin connected to Data (pin 2) of 4094
int dataPin = 2;  //atmega(pin-24)
//Pin connected to Clock (pin 3) of 4094
int clockPin = 4;  //atmega(pin-23)
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

  Serial.println(counter);
}

void update_display_and_counter()
{
  int i=0;      
  digitalWrite(strobePin, LOW);
  for(i=NUM_OF_DISPLAY-1;i>-1;i--)
    {     
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[shiftOutBuffer[i]]);             
    }
  digitalWrite(strobePin, HIGH);

  counter = shiftOutBuffer[0]*1 + shiftOutBuffer[1]*10 + shiftOutBuffer[2]*100;

  Serial.println(counter);
}

void blink_display()
{
  shiftOutBuffer[0]= 10;
  shiftOutBuffer[1]= 10;
  shiftOutBuffer[2]= 10;

  update_display();
  delay(500);

  shiftOutBuffer[0] = counter % 10;
  shiftOutBuffer[1] = (counter / 10) % 10;
  shiftOutBuffer[2] = (counter / 100) % 10;

  update_display();
  delay(500);

  
}

void shift_left(int new_entrant)
{
  shiftOutBuffer[2]= shiftOutBuffer[1];
  shiftOutBuffer[1]= shiftOutBuffer[0];
  shiftOutBuffer[0]= new_entrant;
   
}

void increment_counter()
{
  counter = counter + 1;
  shiftOutBuffer[0] = counter % 10;
  shiftOutBuffer[1] = (counter / 10) % 10;
  shiftOutBuffer[2] = (counter / 100) % 10;
}

void decrement_counter()
{
  counter = max(counter - 1, 0);
  shiftOutBuffer[0] = counter % 10;
  shiftOutBuffer[1] = (counter / 10) % 10;
  shiftOutBuffer[2] = (counter / 100) % 10;
}

void reset_digits(){
  int i=0;
  for(i=0;i<NUM_OF_DISPLAY;i++)
  {
   shiftOutBuffer[i]=0;
  }
  update_display();
}

void setup()
{
  Serial.begin(115200);
  int i=0;
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  reset_digits();
 
}
  
void loop()
{
  int customKey = customKeypad.getKey(); 
//  if(customKey)
//  { 
//    Serial.println(customKey);
//  }

  switch (customKey) {
    case '1':
      shift_left(1);
      update_display_and_counter();
      break;
    case '2':
      shift_left(2);
      update_display_and_counter();
      break;
    case '3':
      shift_left(3);
      update_display_and_counter();
      break;
    case '4':
      shift_left(4);
      update_display_and_counter();
      break;
    case '5':
      shift_left(5);
      update_display_and_counter();
      break;
    case '6':
      shift_left(6);
      update_display_and_counter();
      break;
    case '7':
      shift_left(7);
      update_display_and_counter();
      break;
    case '8':
      shift_left(8);
      update_display_and_counter();
      break;
    case '9':
      shift_left(9);
      update_display_and_counter();
      break;
    case '0':
      shift_left(0);
      update_display_and_counter();
      break;
    case 'A':
      increment_counter();
      update_display_and_counter();
      blink_display();
      blink_display();
      break;
    case 'B':
      decrement_counter();
      update_display_and_counter();
      blink_display();
      blink_display();
      break;

   case '*':
      reset_digits();
      break;
      
//     case '*':
//      shiftOutBuffer[2]=1;
//      update_display();
//      break;
    default:
//      shift_left(3);
      break;
  }
}
