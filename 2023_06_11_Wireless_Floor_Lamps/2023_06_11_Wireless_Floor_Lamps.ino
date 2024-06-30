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
  if (espData.buttonState == 0 || espData.buttonState == 1) {
    runSound2023();
  } else if (espData.buttonState == 2 || espData.buttonState == 3) {
    switch (patternCounter) {
      case 1:
        runFire2012();
        break;
      case 0:
        runPacifica();
        break;
      case 2:
        runPride2015();
        break;
      case 3:
        runLEDsTutorial();
        break;
    }

  }
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
        patternCounter = (patternCounter + 1) % 2; // Goes from 0 to 1
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
      patternCounter          = (patternCounter + 1) % 2; // Goes from 0 to 1
      oldRotaryEncoderVal     = espData.rotaryEncoderVal;
      oldRotaryBrightnessVal  = espData.rotaryEncoderVal;
      rotaryChanged           = false;
    }
  }
}

////////////////////// LED run functions /////////////////////////////
void runSound2023(){
  isRunning = true; if (bSerialPrinterALL) {Serial.println("Sound23");}
  Sound2023 sound2023 = Sound2023();
  while(isRunning) {sound2023.runPattern(espData.useVal, espData.longTermAverage, patternCounter); checkButton(); turnOffLEDs(2);
    EVERY_N_SECONDS(batteryLevelReadingsSec){
      readBatteryLevel();
    }
  }
}

void runLEDsTutorial(){
  isRunning = true; if (bSerialPrinterALL) {Serial.println("LEDsTutorial");}
  LEDsTutorial ledsTut = LEDsTutorial();
  while(isRunning) {ledsTut.runPattern(); checkButton(); checkTimerAndRot(); turnOffLEDs(2);
    EVERY_N_SECONDS(batteryLevelReadingsSec){
      readBatteryLevel();
    }
  }
}

void runFire2012(){
  isRunning = true; if (bSerialPrinterALL) {Serial.println("Fire2012");}
  Fire2012 fire2012 = Fire2012();
  while(isRunning) {fire2012.runPattern(); checkButton(); checkTimerAndRot(); turnOffLEDs(2);
    EVERY_N_SECONDS(batteryLevelReadingsSec){
      readBatteryLevel();
    }
  }
}

void runPacifica(){
  isRunning = true; if (bSerialPrinterALL) {Serial.println("Pacifica");}
  Pacifica pacifica = Pacifica();
  while(isRunning) {pacifica.runPattern(); checkButton(); checkTimerAndRot(); turnOffLEDs(3);
    EVERY_N_SECONDS(batteryLevelReadingsSec){
      readBatteryLevel();
    }
  }
}

void runPride2015(){
  isRunning = true; if (bSerialPrinterALL) {Serial.println("Pride2015");}
  Pride2015 pride2015 = Pride2015();
  while(isRunning) {pride2015.runPattern(); checkButton(); checkTimerAndRot(); turnOffLEDs(2);
    EVERY_N_SECONDS(batteryLevelReadingsSec){
      readBatteryLevel();
    }
  }
}



