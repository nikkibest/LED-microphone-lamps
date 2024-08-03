void setLEDColor(int redValue, int greenValue, int blueValue) {
  float scaleVal = 0.3;
  analogWrite(REDPIN,   int(scaleVal*redValue));
  analogWrite(GREENPIN, int(scaleVal*greenValue));
  analogWrite(BLUEPIN,  int(scaleVal*blueValue));
}

void setupLED() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  // do color show on setup
  for (int i=10; i>1; i--){
    int clrVal = (int) 255*(i/10);
    setLEDColor(clrVal, clrVal, clrVal);
    delay(50); // never use delay, but this is used in setup, so okay
  }
}

void buttonLEDstate() { // used in Rotary.ino for buttonState check
  switch (buttonState) {
    case soundState:
      setLEDColor(255, 0, 0); break;
    case soundStateBrightness:
      setLEDColor(0, 255, 0); break;
    case patternState:
      setLEDColor(0, 0, 255); break;
    case patternStateBrightness:
      setLEDColor(170, 0, 255); break;
    default:
      Serial.println("Undefined buttonState!"); break;
  }
}

void activateYellowLEDAlarm() { // used in batteryLvl.h for alarming user of batteryLvl
  int clr_bright[3] = {250, 250, 51}; // yellow colors
  int clr_damp[3]   = {255, 250, 160};
  batteryAlarmLoop(clr_bright, clr_damp);  
}

void activateRedLEDAlarm() {
  int clr_bright[3] = {250, 250, 250}; // red colors
  int clr_damp[3]   = {155, 100, 50};
  batteryAlarmLoop(clr_bright, clr_damp);  
}

void batteryAlarmLoop(int clr_bright[], int clr_damp[]) { // 10 ms step size
  if (ii_LED <= ii_LED_Max) {
    int clr1 = (int) fscale((float) 0.0, (float) ii_LED_Max, (float) clr_bright[0], (float) clr_damp[0], (float) ii_LED, 0.4); // fscale function from Microphone.ino
    int clr2 = (int) fscale((float) 0.0, (float) ii_LED_Max, (float) clr_bright[1], (float) clr_damp[1], (float) ii_LED, 0.4);
    int clr3 = (int) fscale((float) 0.0, (float) ii_LED_Max, (float) clr_bright[2], (float) clr_damp[2], (float) ii_LED, 0.4);
    setLEDColor(clr1, clr2, clr3);
    ii_LED++;
  } else {
    endBatteryAlarm();
  }
}





