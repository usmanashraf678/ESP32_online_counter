#pragma once
#include <Arduino.h>
#include "wifi_helpers.h"

/**************** Function Declarations ******************/
void update_global_digits();
void blink_display();             // blink the large display
void shift_left(int new_entrant); // shift new number to accomodate new entrant on left
void read_from_firebase();

void update_large_display();             // update the large display

void update_global_digits()
{
  d_1 = counter % 10;
  d_10 = (counter / 10) % 10;
  d_100 = (counter / 100) % 10;
}


void shift_left(int new_entrant)
{
  counter = d_10 * 100 + d_1 * 10 + new_entrant * 1; 
}


/******************** May be used for receiver - not required for Transmitter ****************************/
void decrement_counter();         // decrease the counter value
void increment_counter();         // increase the counter value
void update_display_and_counter(); // update the online counter and display
void update_shiftOutBuffer();
void blink_display();

void update_large_display()
{
  int i = 0;
  digitalWrite(strobePin, LOW);
  for (i = NUM_OF_DISPLAY - 1; i > -1; i--)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, segChar[shiftOutBuffer[i]]); //check if MSB is required for Tx
  }
  digitalWrite(strobePin, HIGH);
  // updated_locally = true;
  Serial.println("updated data successfully");
}
void decrement_counter()
{
  counter = max(counter - 1, 0);

  update_global_digits();

  // shiftOutBuffer[0] = d_1;
  // shiftOutBuffer[1] = d_10;
  // shiftOutBuffer[2] = d_100;
}
void increment_counter()
{
  counter = counter + 1;

  update_global_digits();

  // shiftOutBuffer[0] = d_1;
  // shiftOutBuffer[1] = d_10;
  // shiftOutBuffer[2] = d_100;
}
void update_display_and_counter()
{
  update_large_display();
  // counter = shiftOutBuffer[0] * 1 + shiftOutBuffer[1] * 10 + shiftOutBuffer[2] * 100;
  // updated_on_cloud = false;
  //  Serial.println(counter);
}
void update_shiftOutBuffer()
{
  shiftOutBuffer[2] = counter % 10;
  shiftOutBuffer[1] = (counter / 10) % 10;
  shiftOutBuffer[0] = (counter / 100) % 10;
}
void blink_display()
{
  shiftOutBuffer[0] = 10;
  shiftOutBuffer[1] = 10;
  shiftOutBuffer[2] = 10;

  update_large_display();

  unsigned long blink_time_start = millis();
  digitalWrite(buzzerPin, HIGH);

  while (millis() - blink_time_start < 500)
  {
    // wait for some time untill SSD blinks
  }
  digitalWrite(buzzerPin, LOW);

  update_global_digits();

  // shiftOutBuffer[0] = d_1;
  // shiftOutBuffer[1] = d_10;
  // shiftOutBuffer[2] = d_100;

  update_large_display();

  blink_time_start = millis();
  while (millis() - blink_time_start < 500)
  { //  delay(500); wait for some time untill SSD blinks
  }
}



// void shift_left_rx(){
//   // shiftOutBuffer[2] = shiftOutBuffer[1];
//   // shiftOutBuffer[1] = shiftOutBuffer[0];
//   // shiftOutBuffer[0] = new_entrant;
// }


 // void read_from_firebase()
  // {
  //   if (Firebase.getInt(fbdo, "/dr_umair_counter"))
  //   {

  //     if (fbdo.dataTypeEnum() == fb_esp_rtdb_data_type_integer)
  //     {
  //       counter = fbdo.to<int>();
  //       Serial.println(counter);
  //       counter_needs_push = true;
  //     }
  //   }
  //   else
  //   {
  //     Serial.println(fbdo.errorReason());
  //   }
  // }