#include <esp_now.h>
// #include "ESP32_NOW.h"
#include <WiFi.h>
#include "AiEsp32RotaryEncoder.h"
#include "FS.h"
#include "SPIFFS.h"
#include <floatToString.h>
#include <string.h>

#include "variables.h"
// #include "ESP_NOW.h"
// #include "SPIFFS_fcns.h"
// #include "batteryLvl.h"
// #include <math.h>

// We don't need to include the .ino files, bcs the compiler automatically combines all .ino files

//instead of changing here, rather change numbers above
// AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

// void IRAM_ATTR readEncoderISR()
// {
//   rotaryEncoder.readEncoder_ISR();
// }

void setup() {
  // Initialize serial monitor for debugging purposes
  Serial.begin(115200);

  // samples = new averageCounter(10);
  // longTermSamples = new averageCounter(250);
  // while (longTermSamples->setSample(200) == true) {} // fills longTermSamples up with 200 values

  // // Setup LED
  // setupLED();
  // // Setup rotary
  // rotarySetup();
  // // Set timer and pins
  // pinMode(micPinAnalog, INPUT);
  // // Setup ESP NOW connection
  // ESPNOW_setup();
  // // Test SPIFFS fcns
  // setup_SPIFFS();
}

void loop() {
  // // Microphone and rotary sensor
  // current_time = millis();
  // bool bInterval_mic = ((current_time % 10) > 8.9);   // 10 ms interval
  // bool bInterval_esp = ((current_time % 100) > 98.9); // 100 ms interval
  // serialInput_loop();

  // if (bInterval_mic){ 
  //   rotary_loop(); // sets: buttonPushed & rotaryEncoderVal
  //   mic_loop();    // sets: useVal & longTermAverage
  //   battery_loop();// checks battery level. Overwrite LED
  // }
  // if (bInterval_esp){ 
  //   ESPNOW_loop(useVal, longTermAverage, buttonState, rotaryEncoderVal); // sends to all peers added
  // }
}
