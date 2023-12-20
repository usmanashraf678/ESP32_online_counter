void setup() 
{
  // Set pins as outputs 
  for (int i = 0; i < 3; i++) 
    {pinMode(digit[i], OUTPUT);}

 
  pinMode(18, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(19, OUTPUT);
  //turn on all 3 digits 
  turnON_Digit(DIGIT_ONE);
  turnON_Digit(DIGIT_TWO);
  turnON_Digit(DIGIT_THREE);
} // end setup()
