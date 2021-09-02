/**
  @file main_saf.c

  This is a SAF (smallabstractfish) implementation of the game front end.

  by Miloslav Ciz (drummyfish), 2020

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#define SAF_PROGRAM_NAME "Anarch"
#define SAF_PLATFORM_SDL2

#include "saf.h"

#define SFG_FPS 25
#define SFG_DIMINISH_SPRITES 1
#define SFG_RAYCASTING_MAX_HITS 7
#define SFG_CAN_EXIT 1

#define SFG_SCREEN_RESOLUTION_X SAF_SCREEN_WIDTH
#define SFG_SCREEN_RESOLUTION_Y SAF_SCREEN_HEIGHT

#define SFG_DITHERED_SHADOW 1

#include "game.h"

uint8_t palette[256];

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  SAF_drawPixel(x,y,palette[colorIndex]);
}

uint32_t SFG_getTimeMs()
{
  return SAF_time();
}

void SFG_sleepMs(uint16_t timeMs)
{
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP: return SAF_buttonPressed(SAF_BUTTON_UP); break; 
    case SFG_KEY_DOWN: return SAF_buttonPressed(SAF_BUTTON_DOWN); break;
    case SFG_KEY_RIGHT: return SAF_buttonPressed(SAF_BUTTON_RIGHT); break;
    case SFG_KEY_LEFT: return SAF_buttonPressed(SAF_BUTTON_LEFT); break;
    case SFG_KEY_A: return SAF_buttonPressed(SAF_BUTTON_A); break;
    case SFG_KEY_B: return SAF_buttonPressed(SAF_BUTTON_B); break;
    case SFG_KEY_C: return SAF_buttonPressed(SAF_BUTTON_C); break;
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
  for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
    SAF_save(i,data[i]);
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
    data[i] = SAF_load(i);

  return 1;
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  switch (soundIndex)
  {
    case 0: SAF_playSound(SAF_SOUND_BUMP); break;
    case 1: SAF_playSound(SAF_SOUND_CLICK); break;
    case 2: SAF_playSound(SAF_SOUND_BOOM); break;
    case 3: SAF_playSound(SAF_SOUND_CLICK); break;
    case 4: SAF_playSound(SAF_SOUND_BUMP); break;
    case 5: SAF_playSound(SAF_SOUND_BEEP); break;
    default: break;
  }
}

void SAF_init(void)
{
  for (uint16_t i = 0; i < 256; ++i)
  {
    uint16_t c = paletteRGB565[i];

    uint8_t 
      r = (c >> 8) & 0xf8,
      g = (c >> 3) & 0xf8,
      b = (c << 3) & 0xf8;

    palette[i] = SAF_colorFromRGB(r,g,b);
  }

  SFG_init();
}

uint8_t SAF_loop(void)
{
  return SFG_mainLoopBody();
}
