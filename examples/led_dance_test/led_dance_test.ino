#include <FastLED.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

const int NUM_LEDS = 60;
const int DATA_PIN = 6;
const int NUM_KEYS = 24;

const int button1 = 8;
const int button2 = 9;
const int button3 = 10;
const int button4 = 11;
const int button5 = 12;

Adafruit_MPR121 keys[2] = {Adafruit_MPR121(), Adafruit_MPR121()};

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint32_t touched = 0;

int led_offset = 0;
int led_counter = 0;
CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  pinMode(button1, OUTPUT);
  pinMode(button2, OUTPUT);
  pinMode(button3, OUTPUT);
  pinMode(button4, OUTPUT);
  pinMode(button5, OUTPUT);

}

void loop() {
  // turn on 2 leds in the middle of the strip
  leds[(NUM_LEDS / 2) - 1] = CRGB::Green;
  leds[(NUM_LEDS / 2)] = CRGB::Green;
  leds[(NUM_LEDS / 2) + 1] = CRGB::Green;
  FastLED.show();
  delay(10);

  // loop through and turn on next led, turn off previous
  for(int i = (NUM_LEDS / 2) - 2; i >= 0; i--)
  {
    for(int j = 0; j < NUM_LEDS; j++)
    {
      leds[j] = CRGB::Red;
    }
    //FastLED.showColor(CRGB::Red, 50);
    leds[i] = CRGB::Green;
    leds[NUM_LEDS - 1 - i] = CRGB::Green;
//    leds[i + 1] = CRGB::Black;
//    leds[NUM_LEDS - i - 2] = CRGB::Black;
    //leds[NUM_LEDS - i - 2] = HSV(CRGB::Red, saturation, brightness);
    FastLED.show();
    delay(10);
  }



  FastLED.clear();
  FastLED.show();
  delay(10);



  delay(10);
}
