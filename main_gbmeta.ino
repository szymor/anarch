/**
  @file main_gbmeta.ino

  This is Gamebuino Meta implementation of the game front end, using the
  official library.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <Gamebuino-Meta.h>

#define SFG_ARDUINO 1
#define SFG_CAN_EXIT 0
#define SFG_FPS 30
#define SFG_SCREEN_RESOLUTION_X 80
#define SFG_SCREEN_RESOLUTION_Y 64
#define SFG_RESOLUTION_SCALEDOWN 1
#define SFG_RAYCASTING_MAX_STEPS 20  
#define SFG_RAYCASTING_MAX_HITS 5
#define SFG_RAYCASTING_SUBSAMPLE 2

#define SFG_DITHERED_SHADOW 0

#include "game.h"

Gamebuino_Meta::Color palette[256];

const Gamebuino_Meta::SaveDefault saveDefault[] =
  { { 0, SAVETYPE_BLOB, SFG_SAVE_SIZE, 0 } };

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  Gamebuino_Meta::Color c = palette[colorIndex];
  gb.display.drawPixel(x,y,c);
}

void SFG_sleepMs(uint16_t timeMs)
{
}

int8_t SFG_keyPressed(uint8_t key)
{
  Gamebuino_Meta::Button button;

  switch (key)
  {
    case SFG_KEY_UP: button = BUTTON_UP; break;
    case SFG_KEY_RIGHT: button = BUTTON_RIGHT; break;
    case SFG_KEY_DOWN: button = BUTTON_DOWN; break;
    case SFG_KEY_LEFT: button = BUTTON_LEFT; break;
    case SFG_KEY_A: button = BUTTON_A; break;
    case SFG_KEY_B: button = BUTTON_B; break;
    case SFG_KEY_C: button = BUTTON_MENU; break;
    default: return 0; break;
  }

  return gb.buttons.timeHeld(button) > 0;
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
}

void SFG_enableMusic(uint8_t enable)
{
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
  gb.save.set(0,data,SFG_SAVE_SIZE);
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  gb.save.get(0,data,SFG_SAVE_SIZE);
  return 1;
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  switch (soundIndex)
  {
    case 2: 
      gb.sound.playCancel();
      break;

    case 5:
      gb.sound.playOK();
      break;

    default:
      gb.sound.playTick();
      break;
  }
}

uint32_t SFG_getTimeMs()
{
  return gb.frameStartMicros / 1000;
}

void setup()
{
  gb.begin();
  gb.setFrameRate(SFG_FPS);
  gb.save.config(saveDefault);

  for (int i = 0; i < 256; ++i)
  {
    uint16_t rgb565 = paletteRGB565[i];
    palette[i] = gb.createColor((rgb565 & 0xf800) >> 8,(rgb565 & 0x07e0) >> 3,(rgb565 & 0x001f) << 3);
  }

  SFG_init();
}

void loop()
{
  while(!gb.update())
  {
  }

  SFG_mainLoopBody();
}
