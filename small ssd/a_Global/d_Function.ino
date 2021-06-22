void post_wifi_setup(){
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", "PKT-5", 1);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  Firebase.reconnectWiFi(true);
  delay(500);
}

String get_time(){
  time_t now;
  time(&now);
  char time_output[30];
  // See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions
  strftime(time_output, 30, "%r %a %d-%m-%y", localtime(&now)); 
  return String(time_output); // returns Sat 20-Apr-19 12:31:45
}

void attempt_wifi_connect(){
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long attempt_start_time = millis(); //try to connect with wifi
  Serial.println("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED && millis() - attempt_start_time < WIFI_TIMEOUT_MS) {
    // wait for some time untill WiFi is connected
    Serial.print(".");
    delay(500);
    }

  if(WiFi.status() != WL_CONNECTED){ // not connected still
    Serial.println("failed connecting to wifi");
    WiFi.disconnect();
    WiFi.reconnect();
    if(WiFi.status() == WL_CONNECTED){
        post_wifi_setup();
      }
  }
  else{
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Address is : ");
    Serial.println(WiFi.localIP());    //print local IP address
    post_wifi_setup();
    }
}


void reattempt_wifi_connect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Reattempt connecting to ");
  Serial.println(WIFI_SSID);
  delay(500);
  
  if(WiFi.status() != WL_CONNECTED){ // not connected 
      Serial.println("failed connecting to wifi");
      WiFi.disconnect();
      WiFi.reconnect();
      if(WiFi.status() == WL_CONNECTED){
        post_wifi_setup();
        }
      }
  else{
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Address is : ");
    Serial.println(WiFi.localIP());    //print local IP address
    post_wifi_setup();

    }
}


/************** Functions implementation for small SSDs ****************/
void lights_off()
// Turns off all three digits  
{
  for (int i = 0; i < 3; i++) 
  {digitalWrite(digit[i], LOW);}
}

void lights_on()
// Turns off all three digits  
{
  for (int i = 0; i < 3; i++) 
  {digitalWrite(digit[i], HIGH);}
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

void displayNum(int number) { // sets 4 bit input to the 7447 to output a particular number
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
