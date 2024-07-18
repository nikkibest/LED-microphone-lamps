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

  for (int i=10; i>1; i--){
    int clrVal = (int) 255*(i/10);
    setLEDColor(clrVal, clrVal, clrVal);
    delay(50); // never use delay, but this is used in setup, so okay
  }
}

void activateYellowLEDAlarm() {
  int clr_bright[3] = {250, 250, 51};
  int clr_damp[3]   = {255, 250, 160};

  if (ii_LED < 10) {

    ii_LED++;
  } else {
    ii_LED = 0;
  }
}

void activateRedLEDAlarm() {
  
}

void buttonLEDstate() {
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