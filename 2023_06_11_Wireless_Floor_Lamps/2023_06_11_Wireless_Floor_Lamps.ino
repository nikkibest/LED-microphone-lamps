#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>
#include "variables.h"
#include "ESPNOW.h"
#include <math.h>

void checkTimer();
void checkButton();

#include "Sound2023.h"
#include "LEDsTutorial.h"
#include "Fire2012.h"
#include "Pacifica.h"
#include "Pride2015.h"

void setup() {
  //Initialize Serial Monitor
  Serial.begin(9600);

  oldbuttonState = espData.buttonState;
  oldRotaryEncoderVal = espData.rotaryEncoderVal;
  oldRotaryBrightnessVal = espData.rotaryEncoderVal;

  // Set LED configurations
  initializeLEDS();

  ESPNOW_MAC_address();
  ESPNOW_setup();
}
 
void loop() {
  // Set led colors:
  if (espData.buttonState == 0 || espData.buttonState == 1) {
    runSound2023();
  } else if (espData.buttonState == 2 || espData.buttonState == 3) {
    blend_loop();
  }
  // Check battery level
  EVERY_N_SECONDS(batteryLevelReadingsSec){
    readBatteryLevel();
  }
  // Check button, timer and turn off leds
  EVERY_N_MILLISECONDS(20){
    checkButton(); checkTimerAndRot(); turnOffLEDs(2);
  }
  FastLED.show();
}

void blend_loop() {
  if (bBlendActive) {
    runBlendPattern(source1Pattern, source1);
    runBlendPattern(source2Pattern, source2);
    EVERY_N_MILLISECONDS(20) {
      // Blend between the two sources into output
      blend(source1, source2, leds, NUM_LEDS, blendAmount); // An blendAmount value of 0 will result in 100% source1, and a value of 255 will result in 100% source2.
      if (useSource1) {
        if (blendAmount > 0) { blendAmount--;}
      } else {
        if (blendAmount < 255) { blendAmount++; }
      }
      if (blendAmount==255 || blendAmount==0)  {bBlendActive = false;}
    }
  } else {
    if (useSource1) runBlendPattern(source1Pattern, leds);
    else            runBlendPattern(source2Pattern, leds);
  }
}

///// Next Pattern /////////////
void nextPattern() {
  patternCounter = (patternCounter+1)%3;
  useSource1 = !useSource1;
  if (useSource1) { source1Pattern = patternCounter; } 
  else { source2Pattern = patternCounter; }
  bBlendActive = true;
}

///////////////// Button, Timer and NextPattern ///////////////////////
void checkButton() {
  if ((espData.buttonState - oldbuttonState) != 0) {
    if (espData.buttonState == 1 || espData.buttonState == 3) {
      if (printButton && bSerialPrinterALL) {
        Serial.print("Button clicked: ");
        Serial.println(espData.buttonState);
        printButton = false;
      }
      changeBrightness();
      
    } else { // state button is 0 or 2
      // if (!((espData.buttonState - oldbuttonState) == -1)) {
        isRunning       = false; // Break pattern
      // }
      printButton     = true; // just for printing button
      rotaryChanged   = true; // reset
      if (bSerialPrinterALL) {
        Serial.print("Button clicked: ");
        Serial.println(espData.buttonState);
      }
      oldbuttonState  = espData.buttonState;
    }
  }
}

void checkTimerAndRot() {
  if (espData.buttonState == 2 || espData.buttonState == 0) {
    if (rotaryChanged) {
      EVERY_N_SECONDS(120) {
        isRunning = false;
        nextPattern();
      }
    }
    
    if ((espData.rotaryEncoderVal - oldRotaryEncoderVal) != 0) {
      static unsigned long lastTimeRotated = 0; // Soft debouncing
      if (millis() - lastTimeRotated < 300)
      {
        return;
      }
      lastTimeRotated         = millis();
      isRunning               = false; // Break pattern
      oldRotaryEncoderVal     = espData.rotaryEncoderVal;
      oldRotaryBrightnessVal  = espData.rotaryEncoderVal;
      rotaryChanged           = false;
      nextPattern();
    }
  }
}

////////////////////// LED pattern functions /////////////////////////////
void runBlendPattern(uint8_t pattern, CRGB *LEDArray) {
  isRunning = true;
  switch (pattern) {
    case 0: {
      if (bSerialPrinterALL) {Serial.println("Fire2012");}
      Fire2012 fire = Fire2012();
      while (isRunning) fire.runPattern(LEDArray);
      break;
    }
    case 1: {
      if (bSerialPrinterALL) {Serial.println("Pacifica");}
      Pacifica paci = Pacifica(); //  turnOffLEDs(3);
      while (isRunning) paci.runPattern(LEDArray);
      break;
    }
    case 2: {
      if (bSerialPrinterALL) {Serial.println("Pride2015");}
      Pride2015 pride = Pride2015();
      while (isRunning) pride.runPattern(LEDArray);
      break;
    }
    case 3: {
      if (bSerialPrinterALL) {Serial.println("LEDsTutorial");}
      LEDsTutorial ledtut = LEDsTutorial();
      while (isRunning) ledtut.runPattern(LEDArray);
      break;
    }
  }
}

void runSound2023(){
  isRunning = true; if (bSerialPrinterALL) {Serial.println("Sound23");}
  Sound2023 sound2023 = Sound2023();
  while(isRunning) {
    sound2023.runPattern(leds, espData.useVal, espData.longTermAverage, patternCounter);
  }
}









