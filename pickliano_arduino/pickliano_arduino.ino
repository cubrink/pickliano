#include <FastLED.h>

const int NUM_LEDS = 60;
const int DATA_PIN = 6;

const int button1 = 8;
const int button2 = 9;
const int button3 = 10;
const int button4 = 11;
const int button5 = 12;

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
  // put your main code here, to run repeatedly:
  // // Turn the first led red for 1 second
  // leds[0] = CRGB::Red; 
  // FastLED.show();
  // delay(1000);
  
  // // Set the first led back to black for 1 second
  // leds[0] = CRGB::Black;
  // FastLED.show();
  // delay(1000);
//   led_offset = (led_offset + 1) % 5;
//   for(int dot = 0; dot < NUM_LEDS; dot++) { 
//       if ((dot + led_offset) % 5 == 0) {
//         leds[dot] = CRGB::Green;
//       }
//       else {
//         leds[dot] = CRGB::Black;
//       }
      
//       // clear this led for the next time around the loop
//   }
//   FastLED.show();
//   delay(500);
//   led_offset++;

  if(digitalRead(button1))
  {
    if(led_counter < NUM_LEDS)
    {
      leds[led_counter] = CRGB::Green;
      led_counter++;
      FastLED.setBrightness(255);
      FastLED.show();
    }
    delay(10);
  }
  else
  {
    if(led_counter >= 0)
    {
      leds[led_counter] = CRGB::Black;
      if(led_counter != 0)
      {
        led_counter--;
      }
      FastLED.setBrightness(50);
      FastLED.show();
    }
    delay(10);
  }
  // for(int i = 0; i < NUM_LEDS; i++)
  // {
  //   leds[i] = CRGB::Black;
  // }
  // FastLED.show();
  // led_counter = 0;
  // delay(100);



}
