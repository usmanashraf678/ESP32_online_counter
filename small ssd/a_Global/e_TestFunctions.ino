/******************** Functions for firebase and large ssd connection ********/

void update_global_digits(){
  d_1 = counter % 10;
  d_10 = (counter / 10) % 10;
  d_100 = (counter / 100) % 10;
}

void update_display(){
  int i=0;      
  digitalWrite(strobePin, LOW);
  for(i=NUM_OF_DISPLAY-1;i>-1;i--)
    {     
      shiftOut(dataPin, clockPin, MSBFIRST, segChar[shiftOutBuffer[i]]);             
    }
  digitalWrite(strobePin, HIGH);
  updated_locally = true;
//  Serial.println(counter);
}

void update_display_and_counter(){
  update_display();
  counter = shiftOutBuffer[0]*1 + shiftOutBuffer[1]*10 + shiftOutBuffer[2]*100;
  updated_on_cloud = false;
//  Serial.println(counter);
}

void blink_display(){
  shiftOutBuffer[0]= 10;
  shiftOutBuffer[1]= 10;
  shiftOutBuffer[2]= 10;

  update_display();
  delay(500);

  update_global_digits();
  
  shiftOutBuffer[0] = d_1;
  shiftOutBuffer[1] = d_10;
  shiftOutBuffer[2] = d_100;

  update_display();
  delay(500);  
}

void shift_left(int new_entrant){
  shiftOutBuffer[2]= shiftOutBuffer[1];
  shiftOutBuffer[1]= shiftOutBuffer[0];
  shiftOutBuffer[0]= new_entrant;  
}

void increment_counter(){
  counter = counter + 1;
  
  update_global_digits();
  
  shiftOutBuffer[0] = d_1;
  shiftOutBuffer[1] = d_10;
  shiftOutBuffer[2] = d_100;
}

void decrement_counter(){
  counter = max(counter - 1, 0);
  
  update_global_digits();

  shiftOutBuffer[0] = d_1;
  shiftOutBuffer[1] = d_10;
  shiftOutBuffer[2] = d_100;
}

void reset_digits(){
  int i=0;
  for(i=0;i<NUM_OF_DISPLAY;i++){
    shiftOutBuffer[i]=0;
  }
    
  update_display_and_counter();
  
}

void write_to_firebase(int val, String timestamp){
// Firebase Error Handling And Writing Data At Specifed Path************************************************
  if (Firebase.setInt(firebaseData, "/muhammadi_counter", val) && Firebase.setString(firebaseData, "/muhammadi_time", global_timestamp)) {    // On successful Write operation, function returns 1  
       Serial.println("Value Uploaded Successfully");
       Serial.print("Val = ");
       Serial.println(val);
       updated_on_cloud = true;
       Serial.println(global_timestamp);
       Serial.println("\n");

       }
  else {
    Serial.println(firebaseData.errorReason());
    
    }
}



/***************** Functions for small SSDs **********************/
void count2nine(){ 
  for (int i = 0; i<10; i++){
    displayNum(i);
    delay(500);}
  }


// Displays 3 different numbers simultaneously 
void displayNum3(){
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
