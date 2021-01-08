/**
  @file main_espboy.ino

  This is ESPboy implementation of the game front end, using Arduino
  libraries.

  by Miloslav Ciz (drummyfish), 2021

  Sadly compiling can't be done with any other optimization flag than -Os.

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is to
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#define SFG_CAN_SAVE 1 // for version without saving, set this to 0

#define SFG_FOV_HORIZONTAL 240

#define SFG_SCREEN_RESOLUTION_X 128
#define SFG_SCREEN_RESOLUTION_Y 128
#define SFG_FPS 22
#define SFG_RAYCASTING_MAX_STEPS 20  
#define SFG_RAYCASTING_SUBSAMPLE 2
#define SFG_DIMINISH_SPRITES 1
#define SFG_CAN_EXIT 0
#define SFG_DITHERED_SHADOW 1

#define SFG_AVR 1

#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_MCP4725.h>
#include <TFT_eSPI.h>

#if SFG_CAN_SAVE
  #include <ESP_EEPROM.h>
  #define SAVE_VALID_VALUE 173
  EEPROMClass eeprom;
#endif
    
#define MCP23017address 0
#define MCP4725address  0x60

Adafruit_MCP23017 mcp;
Adafruit_MCP4725 dac;
TFT_eSPI tft;

#include "game.h"

#define SAVE_VALID_VALUE 73

uint8_t gamescreen[SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y];
uint8_t keys;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  gamescreen[y * SFG_SCREEN_RESOLUTION_X + x] = colorIndex;
}

void SFG_sleepMs(uint16_t timeMs)
{
}

uint32_t SFG_getTimeMs()
{
  return millis();
}

int8_t SFG_keyPressed(uint8_t key)
{
 switch (key)
  {
    case SFG_KEY_UP:    return keys & 0x02; break;
    case SFG_KEY_DOWN:  return keys & 0x04; break;
    case SFG_KEY_RIGHT: return keys & 0x08; break;
    case SFG_KEY_LEFT:  return keys & 0x01; break;
    case SFG_KEY_A:     return keys & 0x10; break;
    case SFG_KEY_B:     return keys & 0x20; break;
    case SFG_KEY_C:     return keys & 0x80; break;
    case SFG_KEY_MAP:   return keys & 0x40; break;
    default: return 0; break;
  }
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
}

void SFG_setMusic(uint8_t value)
{
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
#if SFG_CAN_SAVE
  eeprom.write(0,SAVE_VALID_VALUE);

  for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
      eeprom.write(i + 1,data[i]);

  eeprom.commit();
#endif
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
#if SFG_CAN_SAVE
  if (eeprom.read(0) == SAVE_VALID_VALUE)
    for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
      data[i] = eeprom.read(i + 1);

  return 1;
#else
  return 0;
#endif
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  int freq = 400;
  int dur = 75;
 
  switch (soundIndex)
  {
    case 0: freq = 120; dur = 250; break; // shot
    case 1: freq = 200; dur = 260; break; // door
    case 2: freq = 80;  dur = 200; break; // explosion
    case 3: freq = 220; dur = 50; break;  // click
    case 4: freq = 180; dur = 200; break; // plasma
    case 5: freq = 300; dur = 100; break; // monster
    default: break;
  }

  tone(D3,freq,dur);
}

void setup()
{ 
  dac.begin(MCP4725address);
  delay (100);
  dac.setVoltage(0,false);
  mcp.begin(MCP23017address);
  delay(100);

  // buttons
  for (uint8_t i = 0; i < 8; i++)
  {
    mcp.pinMode(i,INPUT);
    mcp.pullUp(i,HIGH);
  }

  mcp.pinMode(8,OUTPUT);
  mcp.digitalWrite(8,LOW);
  
  tft.begin();
  delay(100);
  tft.setRotation(0);

  tft.setAddrWindow(0,128,0,128);
  tft.fillScreen(TFT_BLACK);

  dac.setVoltage(4095,true); // backlight

  pinMode(D3,OUTPUT); // sound

#if SFG_CAN_SAVE
  eeprom.begin(SFG_SAVE_SIZE + 1);
#endif

  SFG_init();
}

void loop()
{
  keys = ~mcp.readGPIOAB() & 255;
  
  SFG_mainLoopBody();

  uint8_t *pixel = gamescreen;

  for (int i = 0; i < SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y; ++i)
  {
    tft.pushColor(pgm_read_word(paletteRGB565 + *pixel));
    pixel++;
  }
}
