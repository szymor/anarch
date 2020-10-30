/**
  @file main_pokitto.cpp

  This is Pokitto implementation of the game front end, using the official
  PokittoLib.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

// #define JOYHAT  // compiles the version for Pokitto with joystick hat

#if 0
// for debug:
  #define SFG_LOG(s) puts(s);
  #define SFG_CPU_LOAD(x) printf("CPU: %d\n",x);
#endif

#define SFG_FPS 22
#define SFG_CAN_EXIT 0
#define SFG_PLAYER_TURN_SPEED 135

#ifndef JOYHAT
  #define SFG_SCREEN_RESOLUTION_X 110
  #define SFG_SCREEN_RESOLUTION_Y 88
#else
  #define SFG_SCREEN_RESOLUTION_X 88
  #define SFG_SCREEN_RESOLUTION_Y 110
#endif

#define SFG_RESOLUTION_SCALEDOWN 1
#define SFG_DITHERED_SHADOW 0
#define SFG_DIMINISH_SPRITES 0
#define SFG_FOG_DIMINISH_STEP 2048
#define SFG_RAYCASTING_MAX_STEPS 20  
#define SFG_RAYCASTING_MAX_HITS 5
#define SFG_RAYCASTING_SUBSAMPLE 2

#include "game.h"
#include "sounds.h"

#include "../PokittoLib/Pokitto/POKITTO_HW/clock_11u6x.h"
#include "../PokittoLib/Pokitto/POKITTO_HW/timer_11u6x.h"

#include "../PokittoLib/Pokitto/Pokitto.h"
#include "PokittoCookie.h"

class SaveCookie: public Pokitto::Cookie
{
  public:
    uint8_t data[SFG_SAVE_SIZE];
};

SaveCookie save;

Pokitto::Core pokitto;

uint8_t *pokittoScreen;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
#ifndef JOYHAT
  pokittoScreen[y * SFG_SCREEN_RESOLUTION_X + x] = colorIndex;
#else
  pokittoScreen[x * SFG_SCREEN_RESOLUTION_Y + (SFG_SCREEN_RESOLUTION_Y - 1 - y)]
    = colorIndex;
#endif
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
}

uint8_t audioBuff[SFG_SFX_SAMPLE_COUNT];
uint16_t audioPos = 0;

uint8_t musicOn = 0;

void SFG_setMusic(uint8_t value)
{
  switch (value)
  {
    case SFG_MUSIC_TURN_ON: musicOn = 1; break;
    case SFG_MUSIC_TURN_OFF: musicOn = 0; break;
    case SFG_MUSIC_NEXT: SFG_nextMusicTrack(); break;
    defaule: break;
  }
}

static inline uint8_t mixSamples(uint8_t sample1, uint8_t sample2)
{
  return (sample1 >> 1) + (sample2 >> 1);
}

void onTimer() // for sound
{
  if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1))
  {
    Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);

    Pokitto::dac_write(
      musicOn ?
      mixSamples(audioBuff[audioPos],SFG_getNextMusicSample() / 2) :
      audioBuff[audioPos]
      );

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

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
  for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
    save.data[i] = data[i];

  save.saveCookie();

  /* ^ This causes sound to stop as it writes something to timer32, we need to
    reinit the audio: */

  timerInit(8000);
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
#ifdef JOYHAT
  // TODO: vibrate
#endif
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
    data[i] = save.data[i];

  return 1;
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  uint8_t volumeShift = 7 - volume / 32;
  uint16_t baseLevel = 128 - (128 >> volumeShift);

  uint16_t pos = audioPos;

  for (int i = 0; i < SFG_SFX_SAMPLE_COUNT; ++i)
  {
    audioBuff[pos] = mixSamples(audioBuff[pos],baseLevel +
      (SFG_GET_SFX_SAMPLE(soundIndex,i) >> volumeShift));

    pos = (pos < SFG_SFX_SAMPLE_COUNT - 1) ? (pos + 1) : 0;
  }
}

int main()
{
  save.begin("ANARCH",sizeof(save),(char*) &save);

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
    if (pokitto.update())
      SFG_mainLoopBody();

  return 0;
}

