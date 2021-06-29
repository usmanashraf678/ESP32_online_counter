#include <WiFi.h>
#include <FirebaseESP32.h>
// ------ counter main code -------------//
#include <Keypad.h>
boolean counter_uploaded = false;
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

byte rowPins[ROWS] = {16, 17, 32, 33}; //{R1, R2, R3, R4} 34, 35, 32, 33
byte colPins[COLS] = {25, 26, 27, 14}; //{C1, C2, C3, C4} 25, 26, 27, 14 # 16 and 17 are TX/RX pins used as digital read

Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 

#define NUM_OF_DISPLAY 3

int strobePin = 15; //Pin connected to Strobe (pin 1) of 4094
int dataPin = 2;  //Pin connected to Data (pin 2) of 4094
int clockPin = 4;  //Pin connected to Clock (pin 3) of 4094

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
//  Serial.println(counter);
}

void update_display_and_counter()
{
  update_display();
  counter = shiftOutBuffer[0]*1 + shiftOutBuffer[1]*10 + shiftOutBuffer[2]*100;
  counter_uploaded = false;
//  Serial.println(counter);
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
  for(i=0;i<NUM_OF_DISPLAY;i++){
    shiftOutBuffer[i]=0;
  }
    
  update_display_and_counter();
  
}

// --- counter main code ends --- //



#define FIREBASE_HOST "queue-mgmt-947d1-default-rtdb.asia-southeast1.firebasedatabase.app"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "RMaoeR1ArcF2fo0BUaVa5eKI1KAutrmhcgMNoIh9" //Your Firebase Database Secret goes here

#define WIFI_SSID "HUAWEI-8bz8"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "qpwjUsMH"                                      //Password of your wifi network 
 
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
int val=0;

void write_to_firebase(int val){
// Firebase Error Handling And Writing Data At Specifed Path************************************************
  if (Firebase.setInt(firebaseData, "/data", val)) {    // On successful Write operation, function returns 1  
       Serial.println("Value Uploaded Successfully");
       Serial.print("Val = ");
       Serial.println(val);
       Serial.println("\n");
       
//       val++;
//       delay(2000);
       }
  else {        
      Serial.println(firebaseData.errorReason());
    }
    counter_uploaded = true;
}
// --- code inside the loop --//

void setup() {

  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor

  Serial.println("Serial communication started\n\n");  

  unsigned long previousMillis = 0;
  unsigned long interval = 10000; // reset after 10 sec
  unsigned long currentMillis = 0;
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);

    // reset every 5 seconds
    currentMillis = millis();
    if(currentMillis - previousMillis >=interval){
      WiFi.disconnect();
      WiFi.reconnect();
      previousMillis = currentMillis;
    }
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);

 // ----- counter setup code ----//
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  reset_digits();
}

void loop() { 
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
      blink_display();
      blink_display();
      break;
   
   case '#':
      blink_display();
      blink_display();
      break;
      
//     case '*':
//      shiftOutBuffer[2]=1;
//      update_display();
//      break;
    default:
//      shift_left(3);
      break;
  }
  if (counter_uploaded == false){
    write_to_firebase(counter);
  }
// -- code inside the loop for counter main ---//
}


/* NOTE:
 *  To upload value, command is ===> Firebase.setInt(firebaseData, "path", variable);
 *  Example                     ===>  Firebase.setInt(firebaseData, "/data", val);
 */