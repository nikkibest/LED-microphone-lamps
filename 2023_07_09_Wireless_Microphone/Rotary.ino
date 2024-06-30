void rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0; // Soft debouncing
    if (millis() - lastTimePressed < 300)
    {
      return;
    }
    lastTimePressed = millis();
    buttonState++;
    buttonState = buttonState % 4;
    buttonPushed = true;

    if (buttonState == 0) {
      setLEDColor(255, 0, 0);
    } else if (buttonState == 1) {
      setLEDColor(0, 255, 0);
    } else if (buttonState == 2) {
      setLEDColor(0, 0, 255);
    }  else if (buttonState == 3) {
      setLEDColor(170, 0, 255);
    }
}

void rotary_loop()
{
  if (rotaryEncoder.isEncoderButtonClicked())    {
    rotary_onButtonClick();
  }

  static unsigned long lastTimePressed = 0;
  if (buttonState == 1 || buttonState == 3) {
    if (buttonPushed) {
      rotaryEncoder.setBoundaries(0, 10, false);
      buttonPushed = false;
      lastTimePressed = millis();
    }
    if ((millis() - lastTimePressed) > 5000) {
      buttonState--;
      buttonPushed = true;
    }
  } else if (buttonPushed && (buttonState == 0 || buttonState == 2)) {
    rotaryEncoder.setBoundaries(0, 1, true);
    buttonPushed = false;
  }

  if (rotaryEncoder.encoderChanged())    {
    lastTimePressed = millis();
    rotaryEncoderVal = rotaryEncoder.readEncoder();
  }
  
  if (0) {printRotary();}
}

void printRotary() {
  Serial.print("rotaryEncoderVal:");
  Serial.print(rotaryEncoderVal);
  Serial.print(",");
  Serial.print("buttonState:");
  Serial.println(buttonState);
}

void rotarySetup() {
  //we must initialize rotary encoder
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not
  //in this example we will set possible values between 0 and 1000;
  bool circleValues = true;
  rotaryEncoder.setBoundaries(0, 1, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

  /*Rotary acceleration introduced 25.2.2021.
  * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
  * without accelerateion you need long time to get to that number
  * Using acceleration, faster you turn, faster will the value raise.
  * For fine tuning slow down.
  */
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  rotaryEncoder.setAcceleration(10); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  
}