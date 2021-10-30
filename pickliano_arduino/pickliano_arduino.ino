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
  // Read input from keys
  // Get the currently touched pads
  // touched = reverseBits((keys[1].touched() << 12) && keys[0].touched()) >> 8;

  // record number of keys played for LEDs
  int keys_pressed = 0;
  
   // determine which key is pressed
   for (uint8_t i=0; i<24; i++) 
   {
     // it if *is* touched, alert!
     if (touched & _BV(i)) 
     {
       Serial.println("pickle " + String(i) + " touched");
       keys_pressed++;
     }
   }

  // temp logic to test LED strip
  keys_pressed = digitalRead(button1) + digitalRead(button2) + digitalRead(button3) + digitalRead(button4) + digitalRead(button5);

  // light LEDs corresponding to number of keys pressed
  for(int i = 0; i < NUM_LEDS; i++)
  {
    // turn on LED if <= scaled value
    //if(i < ((keys_pressed / float(NUM_KEYS) * float(NUM_LEDS))))
    if(i < ((keys_pressed / float(5)) * float(NUM_LEDS)))     // temp
    {
      leds[i] = CRGB::Green;
    }
    else
    {
      leds[i] = CRGB::Black;
    }
  }


  //Serial.println(leds[0].g);

  FastLED.show();
  delay(10);


}

uint32_t reverseBits(uint32_t n) {
  n = (n >> 16) | (n << 16);
  n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
  n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
  n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
  n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
  return n;
}
