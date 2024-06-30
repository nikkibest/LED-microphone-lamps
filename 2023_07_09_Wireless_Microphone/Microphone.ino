void mic_loop() {
  // mic.raw[0] = analogRead(micPinAnalog);
  uint32_t analogRaw = analogRead(micPinAnalog);
  
  float analogRaw_fscale = fscale(MIC_LOW, MIC_HIGH, MIC_LOW, MIC_HIGH, analogRaw, 0.4); // 0.4 is curve value. Can be between 10 and -10

  if (samples->setSample(analogRaw_fscale))
    return;
  
  longTermAverage = longTermSamples->computeAverage();
  useVal = samples->computeAverage();
  longTermSamples->setSample(useVal);

  int diff = (useVal - longTermAverage);

  if (0) { printMic(analogRaw, analogRaw_fscale, diff); }
  
//   queueOperation(mic.raw, analogRaw);
//   // LPF(mic.raw, mic.raw_lpf, micArrLen, Ts, 0.3);

//   int Top = 0, Bot = 32767;
//   for (int ii = 0; ii < micArrLen; ii++) {
//     Top = max(Top, mic.raw[ii]);
//     Bot = min(Bot, mic.raw[ii]);
//   }
//   mic.diffTopBot = mic.raw[0] - Bot + Top - mic.raw[0];
//   mic.top = Top;
//   mic.bot = Bot;
}

// // Fun fact: Arrays (which actually are pointers) pass by reference by default
// void queueOperation(int Array[], int newValue) {
//   for (int ii = 1; ii < micArrLen; ii++) {
//     // Puts all elements one index down, so micArrLen-1 is the oldest value
//     Array[ii] = Array[ii - 1];
//   }
//   Array[0] = newValue;  // Index 0 is the newest value
// }

// float average(int Array[], int len)  // assuming array is int.
// {
//   long sum = 0L;  // sum will be larger than an item, long for safety.
//   for (int i = 0; i < len; i++)
//     sum += Array[i];
//   return ((float)sum) / len;  // average will be fractional, so float may be appropriate.
// }

// void LPF(int x[], float *y, int len, float dt, float alpha) {
//   // alpha is the filter tuning parameter between 0 and 1. Closer to 0 means more filtering, but more delay. 0.3-0.4 is appropriate
//   // Note since, x comes in a queue array (index 0 is newest value), the loop down below goes from len-1 to 0
//   y[len - 1] = alpha * x[len - 1];
//   for (int i = len - 2; i >= 0; i--) {
//     y[i] = alpha * x[i] + (1 - alpha) * y[i + 1];
//   }
// }

void printMic(uint32_t analogRaw, uint32_t analogRaw_fscale, int diff) {
  Serial.print("analogRaw:");
  Serial.print(analogRaw);
  Serial.print(",");
  Serial.print("analogRaw_fscale:");
  Serial.print(analogRaw_fscale);
  Serial.print(",");
  Serial.print("useVal:");
  Serial.print(useVal);
  Serial.print(",");
  Serial.print("longTermAverage:");
  Serial.print(longTermAverage);
  Serial.print(",");
  Serial.print("diff:");
  Serial.println(diff);
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

  /*
  Serial.print(OriginalRange, DEC);  
   Serial.print("   ");  
   Serial.print(NewRange, DEC);  
   Serial.print("   ");  
   Serial.println(zeroRefCurVal, DEC);  
   Serial.println();  
   */

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

  return rangedValue;
}
