// Pins to control each digit (digit_one, digit_two, digit_three)
const int DIGIT_ONE = 23;
const int DIGIT_TWO = 13;
const int DIGIT_THREE = 12;

// BCD Encoder Input 
const int BIT_A = 18; // Least Significant Bit
const int BIT_B = 22;
const int BIT_C = 21;
const int BIT_D = 19; // Highest Significant Bit

// Array to control digit's common pins
const int digit[] = { DIGIT_ONE, DIGIT_TWO, DIGIT_THREE};

// Array to control input of 7447
const int input_to_7447[] = { BIT_A, BIT_B, BIT_C, BIT_D};

/************Function Prototypes***************************************************/
// input: DIGIT_ONE, DIGIT_TWO, OR DIGIT_THREE
void turnON_Digit(int number);  
void turnOFF_Digit(int number);

void lights_off();  // turns off all 3 digits 
void displayNum(int number);  // input integer between 0-9
void count2nine();  // counts from 0-9
void displayNum3(int one, int two, int three); // displays 3 numbers simultaneously
