/**
  @file main_ringo.ino

  This is Ringo (MAKERphone) implementation of the game front end, using the
  official library and Arduino libraries.

  by Miloslav Ciz (drummyfish), 2020

  For some reason makerphone display accepts 565 RGB values but displays them
  as 332, so the colors are a little distorted.

  Compile with -O1 (-O3 breaks the libraries).

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is to
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <Arduino.h>
#include <MAKERphone.h>
#include <utility/soundLib/MPWavLib.h>
 
#define SFG_SCREEN_RESOLUTION_X LCDWIDTH
#define SFG_SCREEN_RESOLUTION_Y LCDHEIGHT
#define SFG_FPS 35

#define SFG_RAYCASTING_MAX_STEPS 20  
#define SFG_RAYCASTING_SUBSAMPLE 2
#define SFG_DIMINISH_SPRITES 1
#define SFG_CAN_EXIT 0
#define SFG_DITHERED_SHADOW 1

#define SAVE_FILE_NAME "/Anarch/anarch.sav"

#include "game.h"

MAKERphone mp;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  mp.display.drawPixel(x,y,paletteRGB565[colorIndex]);
}

uint32_t SFG_getTimeMs()
{
  return millis();
}

void SFG_sleepMs(uint16_t timeMs)
{
}

uint8_t arrows;

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    #define b(button) (mp.buttons.timeHeld(button) > 0)
    #define r(button) return b(button); break;

    case SFG_KEY_UP: return (arrows & 0x04) || b(BTN_2); break;
    case SFG_KEY_DOWN: return (arrows & 0x08) || b(BTN_5); break;
    case SFG_KEY_RIGHT: return (arrows & 0x01) || b(BTN_6); break;
    case SFG_KEY_LEFT: return (arrows & 0x02) || b(BTN_4); break;
    case SFG_KEY_STRAFE_RIGHT: r(BTN_3)
    case SFG_KEY_STRAFE_LEFT: r(BTN_1)
    case SFG_KEY_JUMP: r(BTN_HASHTAG)
    case SFG_KEY_MAP: r(BTN_ASTERISK)
    case SFG_KEY_PREVIOUS_WEAPON: r(BTN_FUN_LEFT)
    case SFG_KEY_NEXT_WEAPON: r(BTN_FUN_RIGHT)
    case SFG_KEY_A: return b(BTN_A) || b(BTN_8); break;
    case SFG_KEY_B: return b(BTN_B) || b(BTN_9); break;
    case SFG_KEY_C: r(BTN_7);
    case SFG_KEY_MENU: r(BTN_0);
    default: return 0; break;

    #undef b
    #undef r
  }

  return 0;
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
}

void SFG_setMusic(uint8_t value)
{
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
  SD.remove(SAVE_FILE_NAME);
  File f = SD.open(SAVE_FILE_NAME,FILE_WRITE);
  f.write(data,SFG_SAVE_SIZE);
  f.close();
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
  switch (event)
  {
    case SFG_EVENT_PLAYER_DIES: FastLED.showColor(CRGB::Red); break;
    case SFG_EVENT_PLAYER_TAKES_ITEM: FastLED.showColor(CRGB::Green); break;
    case SFG_EVENT_LEVEL_WON: FastLED.showColor(CRGB::Yellow); break;
    default: break;
  }
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  if (SD.exists(SAVE_FILE_NAME))
  {
    File f = SD.open(SAVE_FILE_NAME,FILE_READ);
    f.read(data,SFG_SAVE_SIZE);
    f.close();
  }

  return 1;
}

Oscillator *osc;

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  uint8_t wave = SINE;
  uint16_t freq = 1000;
  float dur = 0.2;
  uint8_t vol = (((uint16_t) volume) * 64) / 256;

  switch (soundIndex)
  {
    case 0: freq = 120; dur = 0.025; wave = SAW; break;            // shot
    case 1: freq = 200; dur = 0.03; wave = TRIANGLE; break;        // door
    case 2: freq = 80;  dur = 0.02; wave = SAW; break;             // explosion
    case 3: freq = 220; dur = 0.005; wave = SAW; vol /= 4; break;  // click
    case 4: freq = 180; dur = 0.06; wave = TRIANGLE; break;        // plasma
    case 5: freq = 300; dur = 0.1; wave = SQUARE; vol /= 2; break; // monster
    default: break;
  }

  osc->setVolume(vol);
  osc->setWaveform(wave);
  osc->beep(freq,dur);
}

void setup()
{
  mp.begin();
  osc = new Oscillator(SINE);
  addOscillator(osc);
  SFG_init();
}

void loop()
{
  arrows = 0x00 | 
    ((mp.buttons.getJoystickX() < 200)) << 0 |
    ((mp.buttons.getJoystickX() > 900)) << 1 |
    ((mp.buttons.getJoystickY() < 200)) << 2 |
    ((mp.buttons.getJoystickY() > 900)) << 3;

  SFG_mainLoopBody();

  mp.update();
}
