// imported libraries
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager.git
#include <EEPROM.h>

#include <Keypad.h> // to use keypad
#include <FirebaseESP32.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer.git
#include <AsyncElegantOTA.h> 

// user defined header files
#include <declarations.h>
#include <small_ssd.h>
#include <change_wifi.h>
#include <eeprom_helpers.h>
#include <wifi_helpers.h>
#include <large_ssd.h>


void setup() {
  EEPROM.begin(512);

  Serial.begin(115200);  // Select the same baud rate if you want to see the datas on Serial Monitor
  Serial.println("Serial communication started\n\n");  
  
 // ----- counter setup code ----//
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(selectWifiPin, INPUT_PULLUP);
  
  figure_out_wifi();

  // reset_digits();
  // updated_on_cloud = false;

} // end setup()


void loop(){

  // AsyncElegantOTA.loop();
  // Displays 3 numbers on 7 segment display 
//   displayNum3();

//   int customKey = customKeypad.getKey();

//   switch (customKey) {
//     case '0':
//       shift_left(0);
//       update_display_and_counter();
//       break;
//     case 'A':
//       update_display_and_counter();
//       blink_display();
//       break;
//     case 'B':
//       decrement_counter();
//       update_display_and_counter();
//       blink_display();
//       break;
//    case '*':
//       reset_digits();
//       blink_display();
//       break;
   
//    case '#':
//       blink_display();
//       break;
      
//     case 'D':  // press D with no wifi and the secret counter number 
//       if (WiFi.status() != WL_CONNECTED && counter == 353){
//         Serial.println("Turning the HotSpot On for password change");
//         launchWeb();
//         setupAP();// Setup HotSpot

//         Serial.println();
//         Serial.println("Waiting.");

//         while ((WiFi.status() != WL_CONNECTED))
//         {
//           Serial.print(".");
//           delay(1000);
//           server_for_AP.handleClient();
//         }

//         Serial.println("new password saved to EEPROM, gonna restart in 10sec");
       
//         delay(10000);
//         ESP.restart();
//       }
//       break;
    
//     default:
//       break;
// //      shift_left(3);
//   }
  
//   if (updated_on_cloud == false && WiFi.status() == WL_CONNECTED){
//     // if not updated on cloud and you have a wifi connection, then write to firebase
//     global_timestamp = get_time();
//     write_to_firebase(counter, global_timestamp);
//     }
//   if(updated_locally == true && WiFi.status() != WL_CONNECTED){// value changed locally but wifi is not connected
//       reattempt_wifi_connect();
//       updated_locally = false;
//     }
}
