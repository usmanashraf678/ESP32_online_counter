void setup() 
{
  // Set pins as outputs 
  for (int i = 0; i < 3; i++) 
    {pinMode(digit[i], OUTPUT);}
  for (int i = 0; i < 4; i++) 
    {pinMode(input_to_7447[i], OUTPUT);}

  //turn on all 3 digits 
  lights_on();

  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor

  Serial.println("Serial communication started\n\n");  

  unsigned long interval = 5000; // reset after 10 sec

  unsigned long previousMillis = 0, currentMillis = 0;
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);

    // reset every 10 seconds
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

} // end setup()
