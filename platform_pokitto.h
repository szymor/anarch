/**
  @file platform_pokitto.h

  This is Pokitto implementation of the game front end, using the official
  PokittoLib.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_PLATFORM_H
#define _SFG_PLATFORM_H

#include "settings.h"

#define SFG_LOG(str) printf("game: %s\n",str); // for debug only

#define SFG_TEXTURE_DISTANCE 5000

#undef SFG_FPS
#define SFG_FPS 35

#undef SFG_SCREEN_RESOLUTION_X
#define SFG_SCREEN_RESOLUTION_X 110

#undef SFG_SCREEN_RESOLUTION_Y
#define SFG_SCREEN_RESOLUTION_Y 88

#undef SFG_RESOLUTION_SCALEDOWN
#define SFG_RESOLUTION_SCALEDOWN 1

#undef SFG_DITHERED_SHADOW
#define SFG_DITHERED_SHADOW 0

#undef SFG_FOG_DIMINISH_STEP
#define SFG_FOG_DIMINISH_STEP 2048

#undef SFG_RAYCASTING_MAX_STEPS
#define SFG_RAYCASTING_MAX_STEPS 20  

#undef SFG_RAYCASTING_MAX_HITS
#define SFG_RAYCASTING_MAX_HITS 6

#undef SFG_RAYCASTING_SUBSAMPLE
#define SFG_RAYCASTING_SUBSAMPLE 2

#include "Pokitto.h"
#include "clock_11u6x.h"
#include "timer_11u6x.h"
#include "palette.h"

#include "sounds.h"

Pokitto::Core pokitto;

uint8_t *pokittoScreen;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  pokittoScreen[y * SFG_SCREEN_RESOLUTION_X + x] = colorIndex;
}

uint32_t SFG_getTimeMs()
{
  return pokitto.getTime();
}

void SFG_sleepMs(uint16_t timeMs)
{
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP: return pokitto.upBtn(); break;
    case SFG_KEY_RIGHT: return pokitto.rightBtn(); break;
    case SFG_KEY_DOWN: return pokitto.downBtn(); break;
    case SFG_KEY_LEFT: return pokitto.leftBtn(); break;
    case SFG_KEY_A: return pokitto.aBtn(); break;
    case SFG_KEY_B: return pokitto.bBtn(); break;
    case SFG_KEY_C: return pokitto.cBtn(); break;
    default: return 0; break;
  }
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
  *x = 0;
  *y = 0;
}

uint8_t audioBuff[SFG_SFX_SAMPLE_COUNT];
uint16_t audioPos = 0;

void onTimer() // for sound
{
  if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1))
  {
    Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);

    Pokitto::dac_write(audioBuff[audioPos]);

    audioBuff[audioPos] = 127;

    audioPos = (audioPos + 1) % SFG_SFX_SAMPLE_COUNT;
  }
}

void timerInit(uint32_t samplingRate)
{
  Chip_TIMER_Init(LPC_TIMER32_0);
  Chip_TIMER_Reset(LPC_TIMER32_0);
  Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 1);
  Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, 
    (Chip_Clock_GetSystemClockRate() / samplingRate));
  Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 1);
  Chip_TIMER_Enable(LPC_TIMER32_0);

  #define weirdNumber ((IRQn_Type) 18)
  NVIC_ClearPendingIRQ(weirdNumber);
  NVIC_SetVector(weirdNumber, (uint32_t) &onTimer);
  NVIC_EnableIRQ(weirdNumber);
  #undef weirdNumber
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  uint8_t volumeStep = volume / 16;

  uint16_t pos = audioPos;

  for (int i = 0; i < SFG_SFX_SAMPLE_COUNT; ++i)
  {
    int16_t mixedValue =
      audioBuff[pos] - 127 + SFG_GET_SFX_SAMPLE(soundIndex,i) * volumeStep;

    mixedValue = (mixedValue > 0) ? ((mixedValue < 255) ? mixedValue : 255) : 0;

    audioBuff[pos] = mixedValue;// SFG_GET_SFX_SAMPLE(soundIndex,i) * volumeStep;

    pos = (pos < SFG_SFX_SAMPLE_COUNT - 1) ? (pos + 1) : 0;
  }
}

int main()
{
  pokitto.begin(); 
  timerInit(8000);

  for (uint16_t i = 0; i < SFG_SFX_SAMPLE_COUNT; ++i)
    audioBuff[i] = 127;

  pokitto.setFrameRate(255);
  pokitto.display.setFont(fontTiny);
  pokitto.display.persistence = 1;
  pokitto.display.setInvisibleColor(-1);
  pokitto.display.load565Palette(paletteRGB565);

  pokittoScreen = pokitto.display.screenbuffer;

  SFG_init();

  while (pokitto.isRunning())
  {
    if (pokitto.update())
    {
      SFG_mainLoopBody();
    }
  }

  return 0;
}

#endif // guard

