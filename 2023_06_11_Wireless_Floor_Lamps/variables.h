////////////// ESPNOW variables ///////////////////
//Structure example to receive data
/* Must match the sender structure! */
typedef struct test_struct {
  uint16_t useVal;
  uint16_t longTermAverage;
  uint8_t buttonState;
  uint8_t rotaryEncoderVal;
} test_struct;

//Create a test_struct called espData to hold the received data
test_struct espData;

//////////////////// LED variables ////////////////////////////////
#define MIC_LOW     0
#define MIC_HIGH    644
#define LED_PIN     25
#define NUM_LEDS    144
#define BRIGHTNESS_MAX  25
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BATTERY_PIN  35

//////////////////// Loop logic variables ///////////////////////
bool bSerialPrinterALL = false;
uint8_t oldRotaryEncoderVal = 0;
uint8_t oldbuttonState = 0;
uint8_t patternCounter = 0;
bool rotaryChanged = true;
bool isRunning = false;
uint8_t brightnessVal = (uint8_t)BRIGHTNESS_MAX;
uint8_t oldRotaryBrightnessVal = 0;
bool printButton = true;
double battery_Vout = 0;
double battery_Vsource = 0;
const uint8_t batteryLevelReadingsSec = 1;
const uint8_t R1 = 550;
const uint8_t R2 = 2000;

// CRGB variable. Basically it holds the color of each individual LED
CRGB leds[NUM_LEDS];

void initializeLEDS() {
  // Set LED configurations
  Serial.println("Power up safety. Initializing FASTLED");
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS_MAX );
  FastLED.setCorrection( TypicalLEDStrip ); // Changes to "right" white color, as our eyes are more sensitive to green
  FastLED.setTemperature( Candle ); // set color temperature

  // Do a color show to present that ESP is rebooted
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 235);
  }
  delay(5);
  FastLED.show();
  for (int i = 0; i <= 5; i++) {
    // Blurs the led pixels to its two immediate neighbours, dims the light
    // Call blur multiple times to spread to more than two neighbours
    fadeToBlackBy(leds, NUM_LEDS, 51); // last element is fade amount (from 255 to 0 "fading")
    delay(200);
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  delay(5);
  FastLED.show();
}

void changeBrightness() {
  if (espData.rotaryEncoderVal != oldRotaryBrightnessVal) {
    brightnessVal = (uint8_t)map(espData.rotaryEncoderVal, 0, 10, 5, BRIGHTNESS_MAX);
    // Saturate the brightnessVal, just in case
    brightnessVal = max(brightnessVal, (uint8_t)5);
    brightnessVal = min(brightnessVal, (uint8_t)BRIGHTNESS_MAX);
    if (bSerialPrinterALL) {
      Serial.print("Changing brightnessVal to: ");
      Serial.println(brightnessVal);
    }
    FastLED.setBrightness( brightnessVal );
    oldRotaryBrightnessVal = espData.rotaryEncoderVal;
  }
}

void turnOffLEDs(uint8_t incrment) {
  int k = 0;
  while (k < NUM_LEDS)
  {
    if (!((k % incrment) == 0))
    {
      leds[k] = CRGB(0, 0, 0);
    }
    k++;
  }
  FastLED.show();
}

void batteryAlarm(uint8_t typeOfAlarm) {
  // Do a color show to present that baatteries are running low
  for (int i = 0; i < NUM_LEDS; i++) {
    if (typeOfAlarm == 1) {
      leds[i] = CRGB(255,0,0); // turn all leds red
    } else {
      leds[i] = CRGB(255,255,0); // turn all leds red
    }
  }
  delay(500);
  FastLED.show();
  for (int i = NUM_LEDS-1; i > (int)(NUM_LEDS/5) ; i--) {
    leds[i] = CRGB(0,0,0); // make 80% of the leds go black, one after another
    delay(30);
    FastLED.show();
  }
  int counter=0;
  bool directionDown = true;
  while (counter < 6) {
    if (directionDown) {
      for (int j=brightnessVal; j > 5; j--) {
        FastLED.setBrightness( j );
        delay(5);
      }
    } else {
      for (int j=5; j <= brightnessVal; j++) {
        FastLED.setBrightness( j );
        delay(5);
      }
    }
    directionDown != directionDown;
    counter++;
  }
}

float fscale(float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve){
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

  return (float)rangedValue;
}

void readBatteryLevel() {
  if (true) {
    // battery_Vout = map(analogRead(BATTERY_PIN), 0.0f, 4095.0f, 0.0, 3.3);
    
    battery_Vout = fscale(0.00, 4095.00, 0.00, 3.30, analogRead(BATTERY_PIN), 0);
    battery_Vsource = battery_Vout * 1.275; // (R1+R2)*R2
    if (!bSerialPrinterALL) {
      Serial.print("battery_Vout:");
      Serial.print(battery_Vout);
      Serial.print(",");
      Serial.print("battery_Vsource:");
      Serial.println(battery_Vsource);
    }
    if (battery_Vout < 20) {
      // batteryAlarm(1); //red alarm
    } else if (battery_Vout < 40) {
      // batteryAlarm(2); // yellow warning
    }
  }
}



