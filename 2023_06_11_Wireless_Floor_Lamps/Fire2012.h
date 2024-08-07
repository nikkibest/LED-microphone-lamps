#include "Arduino.h"

class Fire2012 {
  public:
    Fire2012(){};
    void runPattern(CRGB *LEDArray);
  
  private:
    void fireLoop(CRGB *LEDArray);
    
    const uint8_t cooling = 80;
    const uint8_t sparking = 200;
    bool gReverseDirection = false;
};

void Fire2012::runPattern(CRGB *LEDArray) {
  EVERY_N_MILLISECONDS(30){
    if(random8() > 150) {
      fireLoop(LEDArray);
    }
  }
}

void Fire2012::fireLoop(CRGB *LEDArray) {
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    // qsub8 is a subtracting function, that makes sure the result is not below zero
    heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    // heat value is weighted average of the two cells under it
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }
  
  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if( random8() < sparking ) {
    int y = random8(7); // random between 0 and 7
    heat[y] = qadd8( heat[y], random8(160,255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]); // 0=cool color (white), 255=hot color (red)
    int pixelnumber;
    if( gReverseDirection ) {
      pixelnumber = (NUM_LEDS-1) - j;
    } else {
      pixelnumber = j;
    }
    LEDArray[pixelnumber] = color;
  }
}