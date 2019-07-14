#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#include <avr/sleep.h>  

const byte interruptPin1 = 2;
const byte interruptPin2 = 3;

uint8_t dataPin  = 4;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 5;    // Green wire on Adafruit Pixels

int state;

// interrupt service routine for when button 1 pressed
void b1()
{
  Serial.println(state);
  state = 1;
}  // end of wake

void b1_scenario()
{
  state = 0;
  rainbow(20);
  colorWipe(Color(0, 0, 0), 0);
}

void b2() {
  state = 2;
  Serial.println(state);
}

void b2_scenario()
{
  int i;
  state = 0;
  colorWipe(Color(0, 0, 255), 100);
  for (i=0; i < 6; i++) {
    colorWipe(Color(0, 0, 0), 0);
    colorWipe(Color(0, 0, 255), 100);
    delay(9000);
  }
  colorWipe(Color(255, 182, 0), 10);
  for (i=0; i < 6; i++) {
    colorWipe(Color(0, 0, 0), 0);
    colorWipe(Color(255, 0, 182), 100);
    delay(9000);
  }
  for (i=0; i < 6; i++) {
    colorWipe(Color(0, 0, 0), 10);
    colorWipe(Color(255, 0, 0), 10);
    delay(9000);
  }
  colorWipe(Color(0, 0, 0), 0);
}

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(9, dataPin, clockPin);

void setup() {
      
  Serial.begin(9600);

  strip.begin();

  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), b1, FALLING);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), b2, FALLING);
  
  state = 0;
  // Update LED contents, to start they are all 'off'
  strip.show();
}

void sleepNow() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    sleep_enable();          // enables the sleep bit in the mcucr register
    sleep_mode();            // here the device is actually put to sleep!!  
    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP  
    sleep_disable();         // first thing after waking from sleep: disable sleep...  
}  

void loop() {
  int i, j;
  
  switch (state) {
  case 1:
    Serial.println(state);
    b1_scenario();
    break;
  case 2:
    Serial.println(state);
    b2_scenario();
    break;
  default:
    Serial.println(state);
    break;
  }
  Serial.println("device is actually put to sleep");
  delay(100);
  sleepNow();     // sleep function called here  
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

 // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(3, 19, 95), 50); // Heavy Blue
//  colorWipe(strip.Color(185, 211, 147), 50); // Peachy
//  colorWipe(strip.Color(210, 255, 140), 50); // Orangey
//  colorWipe(strip.Color(254, 255, 112), 50); // Yellow
//  colorWipe(strip.Color(114, 250, 51), 50); // Orange
//  colorWipe(strip.Color(34, 186, 100), 50); // Purple
//  colorWipe(strip.Color(23, 169, 37), 50); // Red
//  colorWipe(strip.Color(0, 23, 85), 50); // Dark Blue
