#include "Arduino.h"

class Sound2023 {
  public:
    Sound2023(){};
    void runPattern(CRGB *LEDarray, float useVal, float longTermAverage, uint8_t patternCounter);
  
  private:
    void soundLoopBeat(CRGB *LEDarray, float useVal);
    void soundReactive(CRGB *LEDarray, float useVal, float longTermAverage);
    uint16_t fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve);
    
    float globalHue = 0;
    int   hueOffset = 120;
    float fadeScale = 1.3;
    float hueIncrement = 0.7;
    // LED palette variables
    CRGBPalette16 currentPalette;
    TBlendType    currentBlending;
};

void Sound2023::runPattern(CRGB *LEDarray, float useVal, float longTermAverage, uint8_t patternCounter) {
  EVERY_N_MILLISECONDS(50){
    if (patternCounter == 0) { soundReactive(LEDarray, useVal, longTermAverage); }
    else                     { soundLoopBeat(LEDarray, useVal); }
  }
}

void Sound2023::soundReactive(CRGB *LEDarray, float useVal, float longTermAverage) {
  int diff = (useVal - longTermAverage);
  if (diff > 5)        { if (globalHue < 235)    {globalHue += hueIncrement; }  }
  else if (diff < -5)  { if (globalHue > 2)      {globalHue -= hueIncrement; }  }

  int curshow = fscale(MIC_LOW, MIC_HIGH, 0.0, (float)NUM_LEDS, (float)useVal, 0);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    if (i < curshow) {LEDarray[i] = CHSV(globalHue + hueOffset + (i * 2), 255, 255);    }
    else             {LEDarray[i] = CRGB(LEDarray[i].r / fadeScale, LEDarray[i].g / fadeScale, LEDarray[i].b / fadeScale);}
  }
}

void Sound2023::soundLoopBeat(CRGB *LEDarray, float useVal) {
  uint8_t posBeat1 = beatsin8(30, 0, 255); 
  uint8_t posBeat2 = beatsin8(20, 0, 255);
  currentPalette = RainbowColors_p;         
  currentBlending = LINEARBLEND;

  uint16_t beatVal = fscale(MIC_LOW, MIC_HIGH, 1.0, (float)NUM_LEDS, useVal, 0); // Goes from 1 to NUM_LEDS
  
  fadeToBlackBy(LEDarray, NUM_LEDS, 200);
  fill_palette(LEDarray, beatVal, (posBeat1 + posBeat2) / 2, 10, currentPalette, 255, currentBlending);
}

uint16_t Sound2023::fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve){
  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;


  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  /*
   Serial.println(curve * 100, DEC);   // multply by 100 to preserve resolution  
   Serial.println();
   */

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin){
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float


  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0){
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {  
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return (uint16_t)rangedValue;
}