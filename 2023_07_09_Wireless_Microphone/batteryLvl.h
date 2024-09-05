// #include "Arduino.h"
// #include "globalFunctions.h"

// class BatteryLVL {
//   public:
//     BatteryLVL(){};
//     void battery_loop(unsigned long current_time);
//     float getBatteryVsource();
//   private:
//     void readBatteryLevel();
//     void checkBatteryLevel();
//     void activateBatteryAlarm();
//     void endBatteryAlarm();
//     void checkBatteryTimeoutTimer(unsigned long current_time);
//     void batteryAlarmLoop(int clr_bright[], int clr_damp[]);
//     void activateYellowLEDAlarm();
//     void activateRedLEDAlarm();

//     float battery_adc;      // battery ADC value
//     float battery_Vout;     // battery voltage value
//     float battery_Vsource;  // battery voltage value at source
//     uint16_t ii_LED = 0;
//     uint16_t ii_LED_Max = 3*100; // 3sec (converted from 10ms)
//     const float battery_V_yellowAlarm = 3.0; // Voltage value [0-4.2 V]
//     const float battery_V_redAlarm = 2.5; // Voltage value [0-4.2 V]
//     bool bAlarmOn = false;
//     bool bBatteryTimeoutTimerOn = false;
//     bool bPrint_battery = false;
//     unsigned long battery_last_time = 0;
//     const uint16_t battery_timeout_duration = 30*1000; // 30sec (converted from ms)

//     #define BATTERY_PIN 10 // TODO <-----!
//     uint8_t typeOfBatAlarm = 0;// 0=noAlarm, 1=yellowAlarm, 2=redAlarm
//     // const uint8_t R1 = 550; // resistor 1
//     // const uint8_t R2 = 2000;// resistor 2    
// };

// float BatteryLVL::getBatteryVsource() {
//   return battery_Vsource;
// }


// void BatteryLVL::readBatteryLevel() {
//   battery_adc = analogRead(BATTERY_PIN);
//   battery_Vout = fscale(0.00, 4095.00, 0.00, 3.30, battery_adc, 0); // fscale function from globalFunctions
//   battery_Vsource = battery_Vout * 1.275; // (R1+R2)*R2=1.275 grid value
//   if (bPrint_battery) {
//     Serial.print("battery_adc:");
//     Serial.print(battery_adc);
//     Serial.print(",");
//     Serial.print("battery_Vout:");
//     Serial.print(battery_Vout);
//     Serial.print(",");
//     Serial.print("battery_Vsource:");
//     Serial.println(battery_Vsource);
//   }
// }

// void BatteryLVL::checkBatteryLevel() {
//   if (!bAlarmOn && !bBatteryTimeoutTimerOn) {
//     typeOfBatAlarm = 0;
//     if (battery_Vsource < battery_V_redAlarm) {
//       bAlarmOn = true;
//       typeOfBatAlarm = 2; // red warning
//     } else if (battery_Vsource < battery_V_yellowAlarm) {
//       bAlarmOn = true;
//       typeOfBatAlarm = 1; // yellow warning
//     }
//   }
// }

// void BatteryLVL::activateBatteryAlarm() {
//   // Do a color show to present that batteries are running low
//   if (bAlarmOn) {
//     switch (typeOfBatAlarm) {
//       case 1: 
//         activateYellowLEDAlarm();
//         break;
//       case 2:
//         activateRedLEDAlarm();
//         break;
//       default:
//         break;
//     }
//   }
// }

// void BatteryLVL::endBatteryAlarm() {
//   typeOfBatAlarm = 0;
//   bAlarmOn = false;
//   bBatteryTimeoutTimerOn = true;
//   ii_LED = 0;
// }

// void BatteryLVL::checkBatteryTimeoutTimer(unsigned long current_time) {
//   if (bBatteryTimeoutTimerOn) {
//     if ((current_time-battery_last_time) > battery_timeout_duration) {
//       // do nothing but wait
//     } else {
//       bBatteryTimeoutTimerOn = false;
//     }
//   } else {
//     battery_last_time = current_time;
//   }   
// }

// void BatteryLVL::batteryAlarmLoop(int clr_bright[], int clr_damp[]) { // 10 ms step size
//   if (ii_LED <= ii_LED_Max) {
//     int clr1 = (int) fscale((float) 0.0, (float) ii_LED_Max, (float) clr_bright[0], (float) clr_damp[0], (float) ii_LED, 0.4); // fscale function from Microphone.ino
//     int clr2 = (int) fscale((float) 0.0, (float) ii_LED_Max, (float) clr_bright[1], (float) clr_damp[1], (float) ii_LED, 0.4);
//     int clr3 = (int) fscale((float) 0.0, (float) ii_LED_Max, (float) clr_bright[2], (float) clr_damp[2], (float) ii_LED, 0.4);
//     setLEDColor(clr1, clr2, clr3);
//     ii_LED++;
//   } else {
//     endBatteryAlarm();
//   }
// }

// void BatteryLVL::activateYellowLEDAlarm() { // used in batteryLvl.h for alarming user of batteryLvl
//   int clr_bright[3] = {250, 250, 51}; // yellow colors
//   int clr_damp[3]   = {255, 250, 160};
//   batteryAlarmLoop(clr_bright, clr_damp);  
// }

// void BatteryLVL::activateRedLEDAlarm() {
//   int clr_bright[3] = {250, 250, 250}; // red colors
//   int clr_damp[3]   = {155, 100, 50};
//   batteryAlarmLoop(clr_bright, clr_damp);  
// }

// void BatteryLVL::battery_loop(unsigned long current_time) {
//   this->checkBatteryTimeoutTimer(current_time);
//   this->readBatteryLevel();
//   this->checkBatteryLevel();
//   this->activateBatteryAlarm();
// }