
// void setLEDColor(int redValue, int greenValue, int blueValue) {
//   float scaleVal = 0.3;
//   analogWrite(REDPIN,   int(scaleVal*redValue));
//   analogWrite(GREENPIN, int(scaleVal*greenValue));
//   analogWrite(BLUEPIN,  int(scaleVal*blueValue));
// }

// void setupLED() {
//   pinMode(REDPIN, OUTPUT);
//   pinMode(GREENPIN, OUTPUT);
//   pinMode(BLUEPIN, OUTPUT);
//   // do color show on setup
//   for (int i=10; i>1; i--){
//     int clrVal = (int) 255*(i/10);
//     setLEDColor(clrVal, clrVal, clrVal);
//     delay(50); // never use delay, but this is used in setup, so okay
//   }
// }

// void buttonLEDstate() { // used in Rotary.ino for buttonState check
//   switch (buttonState) {
//     case 0:
//       setLEDColor(255, 0, 0); break;
//     case 1:
//       setLEDColor(0, 255, 0); break;
//     case 2:
//       setLEDColor(0, 0, 255); break;
//     case 3:
//       setLEDColor(170, 0, 255); break;
//     default:
//       Serial.println("Undefined buttonState!"); break;
//   }
// }







