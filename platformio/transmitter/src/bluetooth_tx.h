#include "BluetoothSerial.h"
// #include "esp_bt_device.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
const String deviceName = "ESP32_SPP";
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void setup_BT_TX();
void BT_tx_loop();

// String getMAC(){
//   const uint8_t* point = esp_bt_dev_get_address();
//   String s = "";
//   for (int i = 0; i < 6; i++) {
//     char str[3];
//     sprintf(str, "%02X", (int)point[i]);
//     s = s + str;
//     if (i < 5){
//       s = s+ ":";
//     }
//   }
//   return s;
// }

void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  switch (event)
    {
    case ESP_SPP_INIT_EVT:
        Serial.println("ESP_SPP_INIT_EVT");
        //tft.println("ESP_SPP_INIT_EVT");
        break;

    case ESP_SPP_SRV_OPEN_EVT://Server connection open
        Serial.println("ESP_SPP_SRV_OPEN_EVT");
        //tft.println("ESP_SPP_SRV_OPEN_EVT");
        break;

    case ESP_SPP_CLOSE_EVT://Client connection closed
        Serial.println("ESP_SPP_CLOSE_EVT");
        //tft.println("ESP_SPP_CLOSE_EVT");
        break;

    case ESP_SPP_CONG_EVT://connection congestion status changed
        Serial.println("ESP_SPP_CONG_EVT");
        //tft.println("ESP_SPP_CONG_EVT");
        break;

    case ESP_SPP_WRITE_EVT://write operation completed
        Serial.println("ESP_SPP_WRITE_EVT");
        //tft.println("ESP_SPP_WRITE_EVT");
        break;

    case ESP_SPP_DATA_IND_EVT://connection received data
        Serial.println("ESP_SPP_DATA_IND_EVT");
        //tft.println("ESP_SPP_DATA_IND_EVT");
        break;

    case ESP_SPP_DISCOVERY_COMP_EVT://discovery complete
        Serial.println("ESP_SPP_DISCOVERY_COMP_EVT");
        //tft.println("ESP_SPP_DISCOVERY_COMP_EVT");
        break;

    case ESP_SPP_OPEN_EVT://Client connection open
        Serial.println("ESP_SPP_OPEN_EVT");
        //tft.println("ESP_SPP_OPEN_EVT");
        break;

    case ESP_SPP_START_EVT://server started
        Serial.println("ESP_SPP_START_EVT");
        //tft.println("ESP_SPP_START_EVT");
        break;

    case ESP_SPP_CL_INIT_EVT://client initiated a connection
        Serial.println("ESP_SPP_CL_INIT_EVT");
        //tft.println("ESP_SPP_CL_INIT_EVT");
        break;

    default:
        Serial.println("unknown event!");
        //tft.println("unknown event!");
        break;
    }
}

void setup_BT_TX() {
  Serial.println("Hi I am the server, now you can pair me with bluetooth!");
  btStart(); 
  SerialBT.begin(deviceName); //Bluetooth device name, slave device
  Serial.println("Device Name: " + deviceName);
  // Serial.print("BT MAC: ");
  // Serial.print(getMAC());
//   Serial.println();
  // SerialBT.register_callback(btCallback);
}
void BT_tx_loop() {
  if(updated_locally == true){
    SerialBT.print(counter);
    updated_locally = false;
  }
  // if (Serial.available()) {
  //   SerialBT.write(Serial.read());
  // }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}