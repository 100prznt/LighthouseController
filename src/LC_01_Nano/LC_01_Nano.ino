// LighthouseController
//
// Leuchtfeuer-Simulation per WS2812 LED
//-----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2021 ELIAS RUEMMLER - info@100prznt.de - February 15th 2021
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

//-----------------------------------------------------------------------------
// ANFANG - Benutzereinstellungen
//-----------------------------------------------------------------------------

#define PIN               7   // Pin an welchem die WS2812 angeschlossen sind

#define LEDCOUNT          38  // Summe aller angeschlossenen LEDs
#define COLCOUNT          19  // Anzahl der Spalten (LEDs welche gleichzeitig angesteuert werden)

#define DIMCOLSBEFORE     2   // Anzahl der Spalten welche vor der Position gedimmt werden (Wert muss > 0 sein)
#define DIMCOLSAFTER      5   // Anzahl der Spalten welche nach der Position gedimmt werden (Wert muss > 0 sein)

bool ColMode = false;         // Spaltenmodus aktivieren, hierbei sind die Einzelnen Strips in Spalten angeordnet.
int Rpm = 40;                 // "Umdrehungen" je Minute

//-----------------------------------------------------------------------------
// ENDE - Benutzereinstellungen
//-----------------------------------------------------------------------------

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDCOUNT, PIN, NEO_GRB + NEO_KHZ800);

int colDelay = 10;
uint8_t rowPixels = 1;
uint32_t defaultColor = 0;
float dimFactorBefore = 0;
float dimFactorAfter = 0;


void setup()
{
  rowPixels = LEDCOUNT / COLCOUNT;
  dimFactorBefore = 100.0 / DIMCOLSBEFORE ;
  dimFactorAfter = 100.0 / DIMCOLSAFTER;
  colDelay = 60000.0 / ((float)Rpm * COLCOUNT);
  
  defaultColor = pixels.Color(0xFF,0x60,0x05); //gelb
  //defaultColor = pixels.Color(0x00,0x00,0xFF); //blau
  
  pixels.begin();

  clearAll();
}


void loop()
{  
  for (int i = 0; i < COLCOUNT; i++)
  {
    for (int z = 0; z < DIMCOLSBEFORE; z++)
    {
      setColColor(i + z, dimColor(defaultColor, (uint8_t)(z * dimFactorBefore)));
    }
    
    for (int z = 0; z < DIMCOLSAFTER; z++)
    {
      setColColor(DIMCOLSBEFORE + i + z, dimColor(defaultColor, (uint8_t)(100 - z * dimFactorAfter)));
    }
    pixels.show();
    delay(colDelay);
  }
}


void setColColor(uint8_t col, uint32_t color)
{
  //Künstlicher Overflow per Modulo  0 .. (COLPIXELS - 1)
  uint8_t realCol = col % COLCOUNT;

  if (ColMode)
  {
    uint8_t firstPixel = realCol * rowPixels;
  
    for (int i = 0; i < rowPixels; i++)
    {
      pixels.setPixelColor(firstPixel + i, color); 
    }
  }
  else
  {
    for (int i = 0; i < rowPixels; i++)
    {
      pixels.setPixelColor(realCol + (i * COLCOUNT), color); 
    }
  }
}


// brightness: 0 .. 100 (dark .. bright)
uint32_t dimColor(uint32_t color, uint8_t brightness)
{
  float scale = brightness / 100.0;
  return ((uint32_t)((color&0xFF0000)*scale)&0xFF0000) + ((uint32_t)((color&0x00FF00)*scale)&0x00FF00) + ((uint32_t)((color&0x0000FF)*scale)&0x0000FF);
}


void clearAll()
{
  for( int i = 0; i < LEDCOUNT; i++)
  {
    pixels.setPixelColor(i, 0x000000);
    pixels.show();
  }
}
