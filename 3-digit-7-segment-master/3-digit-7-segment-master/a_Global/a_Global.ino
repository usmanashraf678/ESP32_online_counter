// Pins to control each digit
const int DIGIT_ONE = 23;
const int DIGIT_TWO = 5;
const int DIGIT_THREE = 17;

// BCD Encoder Input 
const int BIT_A = 18; // Least Significant Bit
const int BIT_B = 22;
const int BIT_C = 21;
const int BIT_D = 19; // Highest Significant Bit

// Array to store digits
const int digit[] = { DIGIT_ONE, DIGIT_TWO, DIGIT_THREE};
           
/************Function Prototypes***************************************************/
// input: DIGIT_ONE, DIGIT_TWO, OR DIGIT_THREE
void turnON_Digit(int number);  
void turnOFF_Digit(int number);

void lights_off();  // turns off all 3 digits 
void displayNum(int number);  // input integer between 0-9
void count2nine();  // counts from 0-9
void displayNum3(int one, int two, int three); // displays 3 numbers simultaneously
