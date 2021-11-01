#include <FastLED.h>
#include <Wire.h>
#include <MIDI.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// Location of buttons and sliders on the arduino
const int slider1 = A0;
const int slider2 = A1;
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

uint8_t midi_cc;

// Track offsets of MIDI values
int32_t octave;
int32_t transpose;


uint32_t get_state();
uint8_t read_slider1();
uint8_t read_slider2();
void check_buttons();

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

    midi_cc = 0;
}

void loop() {
    uint32_t mask;
    uint32_t value;
    uint8_t velocity;

    check_buttons();

    prev_state = state;
    state = get_state();
    changed = state ^ prev_state;

    velocity = read_slider1();
    for (uint32_t i=0; i<24; i++) {
        mask = ((uint32_t)1 << i);
        if (changed & mask) {
            value = (state & mask) >> i;
            MIDI.sendNoteOn(
                36 + i + (12*octave + transpose), // MIDI note
                velocity * value,                 // Velocity
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
    uint32_t left = keys[1].touched();
    left = left << 12;
    uint32_t right = keys[0].touched();
    uint32_t tmp = (left) | right;
    return tmp;
}

uint8_t read_slider1() {
    // MIDI values go from 0-127
    return (uint8_t)((analogRead(slider1) / 1023.0) * 127.0);
}

uint8_t read_slider2() {
    // MIDI values go from 0-127
    return (uint8_t)((analogRead(slider2) / 1023.0) * 127.0);
}

void check_buttons() {
    if (digitalRead(button0)) {
        octave++;
        while (digitalRead(button0)) {}
    }
    if (digitalRead(button1)) {
        octave--;
        while (digitalRead(button1)) {}
    }
    if (digitalRead(button2)) {
        transpose++;
        while (digitalRead(button2)) {}
    }
    if (digitalRead(button3)) {
        transpose--;
        while (digitalRead(button3)) {}
    }
}
