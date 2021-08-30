void loop() 
{
  // Test 7 Digit display // 
//  count2nine();
  AsyncElegantOTA.loop();
  // Displays 3 numbers on 7 segment display 
  displayNum3();

  int customKey = customKeypad.getKey();

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
      
    case 'D':  // press D with no wifi and the secret counter number 
      if (WiFi.status() != WL_CONNECTED && counter == 353){
        Serial.println("Turning the HotSpot On for password change");
        launchWeb();
        setupAP();// Setup HotSpot

        Serial.println();
        Serial.println("Waiting.");

        while ((WiFi.status() != WL_CONNECTED))
        {
          Serial.print(".");
          delay(1000);
          server_for_AP.handleClient();
        }

        Serial.println("new password saved to EEPROM, gonna restart in 10sec");
       
        delay(10000);
        ESP.restart();
      }
      break;
    
    default:
      break;
//      shift_left(3);
  }
  
  if (updated_on_cloud == false && WiFi.status() == WL_CONNECTED){
    // if not updated on cloud and you have a wifi connection, then write to firebase
    global_timestamp = get_time();
    write_to_firebase(counter, global_timestamp);
    }
  if(updated_locally == true && WiFi.status() != WL_CONNECTED){// value changed locally but wifi is not connected
      reattempt_wifi_connect();
      updated_locally = false;
    }
}
