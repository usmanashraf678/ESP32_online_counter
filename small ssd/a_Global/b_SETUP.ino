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

  attempt_wifi_connect();


 // ----- counter setup code ----//
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  reset_digits();

} // end setup()
