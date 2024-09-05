
// #include <esp_now.h>
// // #include "ESP32_NOW.h"
// #include <WiFi.h>

//////////////////// Rotary Encoder variables ////////////////////////////////
#include <stdlib.h>
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 33
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

// namespace ButtonStates {
//   enum states {
//     soundState, // 0
//     soundStateBrightness, // 1
//     patternState, // 2
//     patternStateBrightness // 3
//   }
// }
// enum ButtonStates buttonState = soundState;
uint8_t buttonState = 0;
uint8_t rotaryEncoderVal = 0;
bool buttonPushed = false;

///////////////////// LED lamp & Battery /////////////////////////////////////////
#define REDPIN 27
#define GREENPIN 14
#define BLUEPIN 12

//////////////////// Timers ///////////////////////////
unsigned long current_time  = 0;

//////////////////// Print variables ///////////////////
bool bPrint_mic     = false;
bool bPrint_rot     = false;
bool bPrint_battery = false;
char inputBuffer[50];  // Buffer to store incoming data
uint8_t ii_buffer = 0;  // ii_buffer to keep track of where to store the next character

//////////////////// Microphone variables ////////////////////////////////
// select the input  pin for  the potentiometer
const int micPinAnalog = 35;
const int micArrLen = 20;

uint16_t longTermAverage;
uint16_t useVal;
struct averageCounter *samples;
struct averageCounter *longTermSamples;
#define MIC_LOW 0
#define MIC_HIGH 644

// Microphone struct values
struct averageCounter{
  unsigned short *samples;
  unsigned short sample_size;
  unsigned char counter;

  averageCounter(unsigned short size) {
    counter = 0;
    sample_size = size;
    samples = (unsigned short*) malloc(sizeof(unsigned short) * sample_size); 
    // This set the size of samples to the memory allocation of ONE unsigned short times the sample_size
  }

  bool setSample(unsigned short val) {
    if (counter < sample_size) {
      samples[counter++] = val; // sets counter'th index value
      return true;
    }
    else {
      counter = 0;
      samples[counter] = val; // sets 0th index value
      return false;
    }
  }

  int computeAverage() {
    int accumulator = 0;
    for (int i = 0; i < sample_size; i++) {
      accumulator += samples[i];
    }
    return (int)(accumulator / sample_size); // average value
  }
};


////////////////////// ESP NOW variables /////////////////////////
// struct containing data to send
typedef struct test_struct {
  uint16_t useVal;
  uint16_t longTermAverage;
  uint8_t buttonState;
  uint8_t rotaryEncoderVal;
} test_struct;

// Create a new variable of this struct type
test_struct test;

// Create a variable to store information about the peer
// esp_now_peer_info_t peerInfo;

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
// 9C:9C:1F:E2:0B:30
uint8_t broadcastAddress1[] = {0x9C, 0x9C, 0x1F, 0xE2, 0x0B, 0x30};
uint8_t broadcastAddress2[] = {0x08, 0x3A, 0xF2, 0xAC, 0xFF, 0xB0};
uint8_t broadcastAddress3[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
