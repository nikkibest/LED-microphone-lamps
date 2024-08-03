void battery_loop() {
  checkBatteryTimeoutTimer();
  readBatteryLevel();
  checkBatteryLevel();
  activateBatteryAlarm();
}

void readBatteryLevel() {
  battery_adc = analogRead(BATTERY_PIN);
  battery_Vout = fscale(0.00, 4095.00, 0.00, 3.30, battery_adc, 0); // fscale function from Microphone.ino
  battery_Vsource = battery_Vout * 1.275; // (R1+R2)*R2=1.275 grid value
  if (bPrint_battery) {
    Serial.print("battery_adc:");
    Serial.print(battery_adc);
    Serial.print(",");
    Serial.print("battery_Vout:");
    Serial.print(battery_Vout);
    Serial.print(",");
    Serial.print("battery_Vsource:");
    Serial.println(battery_Vsource);
  }
}

void checkBatteryLevel() {
  if (!bAlarmOn && !bBatteryTimeoutTimerOn) {
    typeOfBatAlarm = 0;
    if (battery_Vsource < battery_V_redAlarm) {
      bAlarmOn = true;
      typeOfBatAlarm = 2; // red warning
    } else if (battery_Vsource < battery_V_yellowAlarm) {
      bAlarmOn = true;
      typeOfBatAlarm = 1; // yellow warning
    }
  }
}

void activateBatteryAlarm() {
  // Do a color show to present that batteries are running low
  if (bAlarmOn) {
    switch (typeOfBatAlarm) {
      case 1: 
        activateYellowLEDAlarm();
        break;
      case 2:
        activateRedLEDAlarm();
        break;
      default:
        break;
    }
  }
}

void endBatteryAlarm() {
  typeOfBatAlarm = 0;
  bAlarmOn = false;
  bBatteryTimeoutTimerOn = true;
  ii_LED = 0;
}

void checkBatteryTimeoutTimer() {
  if (bBatteryTimeoutTimerOn) {
    if ((current_time-battery_last_time) > battery_timeout_duration) {
      // do nothing but wait
    } else {
      bBatteryTimeoutTimerOn = false;
    }
  } else {
    battery_last_time = current_time;
  }
    
}