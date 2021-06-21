/******************** Functions for firebase and large ssd connection ********/
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
//  delay(500);

  shiftOutBuffer[0] = counter % 10;
  shiftOutBuffer[1] = (counter / 10) % 10;
  shiftOutBuffer[2] = (counter / 100) % 10;

  update_display();
//  delay(500);  
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


/***************** Functions for small SSDs **********************/
void count2nine()
// Counts from 0-9 
{ 
  for (int i = 0; i<10; i++)
  {displayNum(i);
    delay(500);} 
}


// Displays 3 different numbers simultaneously 
void displayNum3(int one, int two, int three){ 
  //display first number on digit 1
  turnON_Digit(DIGIT_THREE);
  turnOFF_Digit(DIGIT_TWO);
  turnOFF_Digit(DIGIT_ONE); 
  displayNum(three); 
  delay(5);
  //display second number on digit 2
  turnOFF_Digit(DIGIT_THREE);
  turnON_Digit(DIGIT_TWO);
  turnOFF_Digit(DIGIT_ONE); 
  displayNum(two); 
  delay(5);
  //display third number on digit 3
  turnOFF_Digit(DIGIT_THREE);
  turnOFF_Digit(DIGIT_TWO);
  turnON_Digit(DIGIT_ONE); 
  displayNum(one); 
  delay(5);
}
