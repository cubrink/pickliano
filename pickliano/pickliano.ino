#include <FastLED.h>
#include <Wire.h>
#include <MIDI.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// Location of buttons on the arduino
const int button0 = 8;
const int button1 = 9;
const int button2 = 10;
const int button3 = 11;

// Addresses used for each MPR121
uint8_t keys_address[2] = {0x5A, 0x5B};
Adafruit_MPR121 keys[2] = {Adafruit_MPR121(), Adafruit_MPR121()};

// Track state between loops
uint32_t changed;
uint32_t state;
uint32_t prev_state;

// Track offsets of MIDI values
int32_t octave;
int32_t transpose;


uint32_t get_state();
uint32_t reverse_int32();

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
    // put your setup code here, to run once:
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
    Serial.begin(115200);

    // Wait until both MPR121's are initialized
    while(!keys[0].begin(keys_address[0])) {}
    while(!keys[1].begin(keys_address[1])) {}

    pinMode(button0, OUTPUT);
    pinMode(button1, OUTPUT);
    pinMode(button2, OUTPUT);
    pinMode(button3, OUTPUT);
    
    changed = 0;
    state = 0;
    prev_state = 0;

    octave = 0;
    transpose = 0;
}

void loop() {
    uint32_t mask;
    uint32_t value;

    prev_state = state;
    state = get_state();
    changed = state ^ prev_state;

    for (uint8_t i=0; i<24; i++) {
        mask = (1 << i);
        if (changed & mask) {
            value = (state & mask) >> i;
            MIDI.sendNoteOn(
                36 + i + (12*octave + transpose), // MIDI note
                127 * value,                      // Velocity
                1                                 // Channel
            );
        }
    }

    delay(10);
}

uint32_t get_state() {
    // MPR121 represents the 12 channels as a 16 bit int
    // Shift left 12 so that and bitwise OR to combine the result 
    // into a 24 bit result
    uint32_t tmp = (keys[1].touched() << 12) | keys[0].touched() 
    // The board is actually inverted with respect to a piano
    // Flip this around so that the rightmost bit corresponds to the rightmost key
    tmp = reverse_int32(tmp);
    // Shift 32-24=8 bits left to remove empty bits from reversing
    tmp = tmp >> 8
    return tmp
}

uint32_t reverse_int32(uint32_t n) {
  n = (n >> 16) | (n << 16);
  n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
  n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
  n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
  n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
  return n;
}