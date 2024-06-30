void printESP() {
  Serial.print("useVal:");
  Serial.print(espData.useVal);
  Serial.print(",");
  Serial.print("longTermAverage:");
  Serial.print(espData.longTermAverage);
  Serial.print(",");
  Serial.print("buttonState:");
  Serial.print(espData.buttonState);
  Serial.print(",");
  Serial.print("rotaryEncoderVal:");
  Serial.println(espData.rotaryEncoderVal);
}

//callback function that will be executed when data is received
/*OnDataRecv is called whenever this device received some new data */
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // We copy the incoming data into the espData struct to store it here on this device
  memcpy(&espData, incomingData, sizeof(espData));
  if (0 && bSerialPrinterALL) {printESP();}
}

void ESPNOW_setup() {
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_err_t receive_status = esp_now_register_recv_cb(OnDataRecv);
  if (receive_status == ESP_OK) {
    Serial.println("The OnDataRecv has been successfully registered for the ESP-NOW");
    return;
  }
}

void ESPNOW_MAC_address() {
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}