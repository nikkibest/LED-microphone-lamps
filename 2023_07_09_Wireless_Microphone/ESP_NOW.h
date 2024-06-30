// callback when data is sent. It prints whether or not the send was successful or not
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (0) {
    char macStr[18];
    Serial.print("Packet to: ");
    // Copies the sender mac address to a string
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print(macStr);
    Serial.print(" send status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
}

void ESPNOW_setup() {
  // Set the device as a Wi-Fi station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Then register the callback function OnDataSent, that will be called when a message is sent
  // So OnDataSent will always be called whenever we send a message
  esp_now_register_send_cb(OnDataSent);
   
  // Then we need to pair with the other ESP-NOW devices, so we register peers
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void ESPNOW_loop(uint16_t in1, uint16_t in2, uint8_t in3, uint8_t in4) {
  // Create random variables
  test.useVal = in1;
  test.longTermAverage = in2;
  test.buttonState = in3;
  test.rotaryEncoderVal = in4;

  // Then we send the message and store the result of the sent message
  /* REMEMBER - OnDataSent is also being called when we use the esp_now_send function */
  // The first input refers to the MAC address to send the data to. A 0 or NULL means that data is sent to all peers added to the peer list
  if (sizeof(test_struct) > ESP_NOW_MAX_DATA_LEN) {
    Serial.print("Data to be sent via test_struct is too large");
  }
  
  esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));  

  // Check if the message was successfully sent
  if (result == ESP_OK) {
    // Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
