#include <FastLED.h>
#include <Wire.h>
#include <MIDI.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

const int NUM_LEDS = 60;
const int DATA_PIN = 6;
const int NUM_KEYS = 24;

const int button0 = 8;
const int button1 = 9;
const int button2 = 10;
const int button3 = 11;
const int button4 = 12;

MIDI_CREATE_DEFAULT_INSTANCE();

uint32_t changed;
uint32_t state;
uint32_t prev_state;



uint32_t read_buttons();

void setup() {
  // put your setup code here, to run once:
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
  Serial.begin(115200);
    
  pinMode(button0, OUTPUT);
  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  pinMode(button3, OUTPUT);
  pinMode(button4, OUTPUT);

  changed = 0;
  state = 0;
  prev_state = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  prev_state = state;
  state = read_buttons();
  changed = state ^ prev_state;

  uint32_t mask;
  uint32_t value;
  for (uint8_t i=0; i<32; i++) {
    mask = (1 << i);
    if (changed & mask) {
      value = (state & mask) >> i;
      MIDI.sendNoteOn(36+i, 127 * value, 1);
//      Serial.print("Changing ");
//      Serial.print(36+i);
//      Serial.print(" to ");
//      Serial.println(127*value);
    }
  }
  delay(10);
}

uint32_t read_buttons() {
  return (
     (digitalRead(button0) << 0) |
     (digitalRead(button1) << 1) | 
     (digitalRead(button2) << 2) |
     (digitalRead(button3) << 3) |
     (digitalRead(button4) << 4)
   );
}
