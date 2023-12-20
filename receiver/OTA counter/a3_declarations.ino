/************Function Prototypes***************************************************/
// input: DIGIT_ONE, DIGIT_TWO, OR DIGIT_THREE
void turnON_Digit(int number);  
void turnOFF_Digit(int number);

void lights_off();  // turns off all 3 digits
void lights_on(); // turns on all 3 digits
void displayNum(int number);  // input integer between 0-9
void count2nine();  // counts from 0-9
void displayNum3(); // displays 3 numbers simultaneously

/**************** Functions of updating display ******************/
void attempt_wifi_connect();
void update_display(); // update the large display
void update_display_and_counter(); // update the online counter and display
void blink_display(); // blink the large display
void shift_left(int new_entrant); // shift new number to accomodate new entrant on left
void increment_counter(); // increase the counter value
void decrement_counter(); // decrease the counter value
void reset_digits(); // reset the digits of the counter
void write_to_firebase(int val);
String get_time();

/************ Read and Write from EEPROM *************************/
String read_ee_ssid();
String read_ee_pass();
void wifi_from_hard_code();
void wifi_from_EEPROM();

/********** Wifi Credentials Chnage *********************/
bool testWifi(void);
void launchWeb();
void setupAP(void);
void createWebServer();
