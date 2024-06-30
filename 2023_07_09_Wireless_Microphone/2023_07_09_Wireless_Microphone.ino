#include <esp_now.h>
#include <WiFi.h>
#include "AiEsp32RotaryEncoder.h"
#include "FS.h"
#include "SPIFFS.h"

#include "variables.h"
#include "ESP_NOW.h"
#include "SPIFFS_fcns.h"
#include <math.h>

// We don't need to include the .ino files, bcs the compiler automatically combines all .ino files

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

void setup() {
  // Initialize serial monitor for debugging purposes
  Serial.begin(115200);

  samples = new averageCounter(10);
  longTermSamples = new averageCounter(250);
  while (longTermSamples->setSample(200) == true) {}

  // Setup LED
  void setupLED();
  // Setup rotary
  rotarySetup();
  // Set timer and pins
  pinMode(micPinAnalog, INPUT);
  // Setup ESP NOW connection
  ESPNOW_setup();
  // Test SPIFFS fcns
  setup_SPIFFS();
}

void loop() {
  // Microphone and rotary sensor
  current_MIC = millis();
  if ((current_MIC-previous_MIC) > 10){ // 10 ms interval
    //rotary encoder values
    rotary_loop();
    // Microphone
    mic_loop();
    current_MIC = previous_MIC;
  }

  // ESP-NOW
  current_ESP = millis();
  if ((current_ESP-previous_ESP) > 100){ // 100 ms interval
    ESPNOW_loop(useVal, longTermAverage, buttonState, rotaryEncoderVal);
    previous_ESP=current_ESP;
  }
}
