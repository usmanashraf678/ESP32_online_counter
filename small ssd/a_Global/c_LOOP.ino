void loop() 
{
  // Test 7 Digit display // 
//  count2nine();

  // Displays 3 numbers on 7 segment display 
  displayNum3(8, 2, 4);

  int customKey = customKeypad.getKey(); 
//  if(customKey)
//  { 
//    Serial.println(customKey);
//  }

  switch (customKey) {
    case '1':
      shift_left(1);
      update_display_and_counter();
      break;
    case '2':
      shift_left(2);
      update_display_and_counter();
      break;
    case '3':
      shift_left(3);
      update_display_and_counter();
      break;
    case '4':
      shift_left(4);
      update_display_and_counter();
      break;
    case '5':
      shift_left(5);
      update_display_and_counter();
      break;
    case '6':
      shift_left(6);
      update_display_and_counter();
      break;
    case '7':
      shift_left(7);
      update_display_and_counter();
      break;
    case '8':
      shift_left(8);
      update_display_and_counter();
      break;
    case '9':
      shift_left(9);
      update_display_and_counter();
      break;
    case '0':
      shift_left(0);
      update_display_and_counter();
      break;
    case 'A':
      increment_counter();
      update_display_and_counter();
      blink_display();
      blink_display();
      break;
    case 'B':
      decrement_counter();
      update_display_and_counter();
      blink_display();
      blink_display();
      break;

   case '*':
      reset_digits();
      blink_display();
      blink_display();
      break;
   
   case '#':
      blink_display();
      blink_display();
      break;
      
//     case '*':
//      shiftOutBuffer[2]=1;
//      update_display();
//      break;
    default:
//      shift_left(3);
      break;
  }
  
  if (counter_uploaded == false){
    write_to_firebase(counter);
  }

// -- code inside the loop for counter main ---//
}
