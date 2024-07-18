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
  if (!bAlarmOn) {
    typeOfBatAlarm = 0;
    if (battery_Vsource < 2.5) {
      bAlarmOn = true;
      typeOfBatAlarm = 2; // red warning
    } else if (battery_Vsource < 3.0) {
      bAlarmOn = true;
      typeOfBatAlarm = 1; // yellow warning
    }
  }
}

void activateBatteryAlarm() {
  // Do a color show to present that baatteries are running low
  switch (typeOfBatAlarm) {
    case 1: 
      activateYellowLEDAlarm();
      break;
    case 2:
      break;
    default:
      break;
  }
}