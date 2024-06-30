#include "Arduino.h"

class LEDsTutorial {
  public:
    LEDsTutorial(){};
    
    void runPattern();
  
  private:
    void blend_loop();
    void runBlendPattern(uint8_t pattern, CRGB *LEDArray);
    void nextPattern();
    void LEDsWavesAndBlur(CRGB *LEDarray);
    void LEDsWavesAndBlurWithPalette(CRGB *LEDarray);
    void LEDsBlur(CRGB *LEDarray);
    void FillLEDsFromPaletteColors( uint8_t colorIndex, CRGB *LEDarray);
    void ChangePalettePeriodically();
    void SetupTotallyRandomPalette();
    void SetupBlackAndWhiteStripedPalette();
    void SetupPurpleAndGreenPalette();

    uint8_t blendAmount = 0;
    uint8_t patternCounter = 0;
    uint8_t source1Pattern = 0;
    uint8_t source2Pattern = 1;
    bool useSource1 = 1;

    CRGB source1[NUM_LEDS];
    CRGB source2[NUM_LEDS];
    
    // LED palette variables
    CRGBPalette16 currentPalette;
    TBlendType    currentBlending;

    CRGBPalette16 myRedWhiteBluePalette;
    TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
};

void LEDsTutorial::runPattern() {
  EVERY_N_MILLISECONDS(30) {
    blend_loop();
  }
  // FastLED.show();
}

////////////////////////// Blend /////////////////////////////////////////////
void LEDsTutorial::blend_loop() {
  ChangePalettePeriodically();
  EVERY_N_MILLISECONDS(10) {
    // Blend between the two sources into output
    blend(source1, source2, leds, NUM_LEDS, blendAmount);

    if (useSource1) {
      if (blendAmount < 255) { blendAmount++; }
    } else {
      if (blendAmount > 0) { blendAmount--; }
    }
  }

  EVERY_N_SECONDS(20) {
    nextPattern();
  }

  runBlendPattern(source1Pattern, source1);
  runBlendPattern(source2Pattern, source2);
}

void LEDsTutorial::nextPattern() {
  patternCounter = ( patternCounter + 1 ) % 3;
  if (useSource1) { source1Pattern = patternCounter; }
  else { source2Pattern = patternCounter; }

  switch (patternCounter) {
    case 0:
      if (bSerialPrinterALL) {Serial.println(" LEDsWavesAndBlur");}
      break;
    case 1:
      if (bSerialPrinterALL) {Serial.println(" LEDsWavesAndBlurWithPalette");}
      break;
    case 2:
      if (bSerialPrinterALL) {Serial.println(" FillLEDsFromPaletteColors");}
      break;
  }
  useSource1 = !useSource1;
}

void LEDsTutorial::runBlendPattern(uint8_t pattern, CRGB *LEDArray) {
  switch (pattern) {
    case 0:
      LEDsWavesAndBlur(LEDArray);
      break;
    case 1:
      LEDsWavesAndBlurWithPalette(LEDArray);
      break;
    case 2:
      static uint8_t startIndex = 0; // static will only be created and initialized the first time a function is called and are visible to only one function
      startIndex = startIndex + 1; /* motion speed */
      FillLEDsFromPaletteColors(startIndex, LEDArray);
      break;
  }
}

/////////////////////////// Waves and Blur //////////////////////////////////////
void LEDsTutorial::LEDsWavesAndBlur(CRGB *LEDarray) {
  // Waves for LED position
  // beatsin8 generates sine wave. Inputs are:
  // bpm: beats pr minute (waves pr min, frequency)
  // lowVal and highVal: min and max values (0 and 255 smallest and largest possible)
  // timeBase and phaseOffset: wave offset in either time (calc based on bpm) or phase (between 0 and 255)
  
  // Waves using CHSV color
  uint8_t posBeat1 = beatsin8(30, 0, NUM_LEDS-1, 0, 0); 
  uint8_t posBeat2 = beatsin8(60, 0, NUM_LEDS-1, 0, 0);

  uint8_t posBeat3 = beatsin8(30, 0, NUM_LEDS-1, 0, 127);
  uint8_t posBeat4 = beatsin8(60, 0, NUM_LEDS-1, 0, 127);

  // Wave for LED color
  uint8_t colBeat = beatsin8(45, 0, 255, 0, 0);

  // Add wave index of led to color with changing wave index
  // Waves are combined to make unpredictable pattern, and divide by 2 to decrease amplitude (there are 2 wavs)
  LEDarray[(posBeat1 + posBeat2) / 2] = CHSV(colBeat, 255, 255); // hue, saturation and value
  LEDarray[(posBeat3 + posBeat4) / 2] = CHSV(colBeat, 255, 255); // hue, saturation and value
  fadeToBlackBy(LEDarray, NUM_LEDS, 10);

  LEDsBlur(LEDarray);
}

void LEDsTutorial::LEDsWavesAndBlurWithPalette(CRGB *LEDarray) { 
  // Waves using fill_palette using the color palettes defined below
  uint8_t posBeat1 = beatsin8(30, 0, 255); 
  uint8_t posBeat2 = beatsin8(20, 0, 255);
  fill_palette(LEDarray, NUM_LEDS, (posBeat1 + posBeat2) / 2, 10, currentPalette, 255, currentBlending);

  LEDsBlur(LEDarray);
}

  
void LEDsTutorial::LEDsBlur(CRGB *LEDarray) {
  EVERY_N_MILLISECONDS(1) {
    for (int i = 0; i < 4; i++) {
      // Blurs the led pixels to its two immediate neighbours, dims the light
      // Call blur multiple times to spread to more than two neighbours
      blur1d(LEDarray, NUM_LEDS, 50); // last element is blur amount (from 0 to 255 "spreading")
    }
  }
}


/////////////////////////// Palettes //////////////////////////////////////
void LEDsTutorial::FillLEDsFromPaletteColors( uint8_t colorIndex, CRGB *LEDarray)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; ++i) {
      // ColorFromPalette sets color from palette to leds, but doesn't show them. We can use FastLED.show() to show
      // Inputs: 
      // currentPalette: current color scheme
      // colorIndex: index of color in the currentPalette from 0 to 255 to use
      // brightness: level of light intensity
      // currentBlending: NOBLEND is not soft transitioning between color, whereas LINEARBLEND softens the transition between colors 
      LEDarray[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      colorIndex += 3; // Scales between 0 and 255 on color palette
  }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void LEDsTutorial::ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    bool switchPalette = false;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  5)  { switchPalette = true; currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { switchPalette = true; currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { switchPalette = true; currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { switchPalette = true; SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { switchPalette = true; SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { switchPalette = true; SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { switchPalette = true; SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { switchPalette = true; currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { switchPalette = true; currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { switchPalette = true; currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { switchPalette = true; currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
        if( switchPalette && false && bSerialPrinterALL) { Serial.println(lastSecond); };
    }
}

// This function fills the palette with totally random colors.
void LEDsTutorial::SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void LEDsTutorial::SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void LEDsTutorial::SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
