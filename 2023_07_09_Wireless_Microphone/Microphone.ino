// void mic_loop() {
//   // mic.raw[0] = analogRead(micPinAnalog);
//   uint32_t analogRaw = analogRead(micPinAnalog);
  
//   float analogRaw_fscale = fscale(MIC_LOW, MIC_HIGH, MIC_LOW, MIC_HIGH, analogRaw, 0.4); // 0.4 is curve value. Can be between 10 and -10

//   samples->setSample(analogRaw_fscale); // adds analogRaw_fscale to samples array
//   useVal = samples->computeAverage();   // computes average of samples, useVal
//   longTermSamples->setSample(useVal);   // adds useVal to longTermSamples
//   longTermAverage = longTermSamples->computeAverage(); // computes longTermSamples average


//   int diff = (useVal - longTermAverage);

//   if (bPrint_mic) { printMic(analogRaw, analogRaw_fscale, diff); }
// }

// // // Fun fact: Arrays (which actually are pointers) pass by reference by default
// // void queueOperation(int Array[], int newValue) {
// //   for (int ii = 1; ii < micArrLen; ii++) {
// //     // Puts all elements one index down, so micArrLen-1 is the oldest value
// //     Array[ii] = Array[ii - 1];
// //   }
// //   Array[0] = newValue;  // Index 0 is the newest value
// // }

// // float average(int Array[], int len)  // assuming array is int.
// // {
// //   long sum = 0L;  // sum will be larger than an item, long for safety.
// //   for (int i = 0; i < len; i++)
// //     sum += Array[i];
// //   return ((float)sum) / len;  // average will be fractional, so float may be appropriate.
// // }

// // void LPF(int x[], float *y, int len, float dt, float alpha) {
// //   // alpha is the filter tuning parameter between 0 and 1. Closer to 0 means more filtering, but more delay. 0.3-0.4 is appropriate
// //   // Note since, x comes in a queue array (index 0 is newest value), the loop down below goes from len-1 to 0
// //   y[len - 1] = alpha * x[len - 1];
// //   for (int i = len - 2; i >= 0; i--) {
// //     y[i] = alpha * x[i] + (1 - alpha) * y[i + 1];
// //   }
// // }

// void printMic(uint32_t analogRaw, uint32_t analogRaw_fscale, int diff) {
//   Serial.print("analogRaw:");
//   Serial.print(analogRaw);
//   Serial.print(",");
//   Serial.print("analogRaw_fscale:");
//   Serial.print(analogRaw_fscale);
//   Serial.print(",");
//   Serial.print("useVal:");
//   Serial.print(useVal);
//   Serial.print(",");
//   Serial.print("longTermAverage:");
//   Serial.print(longTermAverage);
//   Serial.print(",");
//   Serial.print("diff:");
//   Serial.println(diff);
// }

