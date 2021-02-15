// LighthouseController
//
// Leuchtfeuer-Simulation per WS2812 LED
//-----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2021 100prznt.de - info@100prznt.de - February 15th 2021
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-----------------------------------------------------------------------------


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN            6  //Pin an welchem die LEDs angeschlossen sind
#define NUMPIXELS      24 //Summe aller angeschlossenen LEDs
#define COLPIXELS      12 //Anzahl der Spalten (LEDs welche gleichzeitig angesteuert werden)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;
uint8_t rowPixels = 1;
uint32_t defaultColor = 0;

void setup() {
  rowPixels = NUMPIXELS / COLPIXELS;
  defaultColor = pixels.Color(0xFF,0xFF,0x0); //gelb
  
  pixels.begin();

  clearAll();
}

void loop() {
  for (int i = 0; i < COLPIXELS; i++)
  {
    setColColor(i, 0x000000);
    setColColor(i + 1, dimColor(defaultColor, 32));
    setColColor(i + 2, dimColor(defaultColor, 64));
    setColColor(i + 3, dimColor(defaultColor, 128));
    setColColor(i + 4, dimColor(defaultColor, 255));
    pixels.show();
    delay(delayval);
  }
}

void setColColor(uint8_t col, uint32_t color)
{
  for (int i = 0; i < rowPixels; i++)
  {
    //Künstlicher Overflow per Modulo  0 .. (COLPIXELS - 1)
    pixels.setPixelColor((col % COLPIXELS) + i, color);
  }
}

uint32_t dimColor(uint32_t color, uint8_t brightness)
{
   return (((color&0xFF0000)/brightness)&0xFF0000) + (((color&0x00FF00)/brightness)&0x00FF00) + (((color&0x0000FF)/brightness)&0x0000FF);
}

void clearAll()
{
  for( int i = 0; i<NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, 0x000000);
    pixels.show();
  }
}
