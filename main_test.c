/**
  @file main_test.c

  This is a front end that serves as an automatic test of the game.

  This fronted tries to play the game and see if it behaves how expected. If you
  change anything substantial in the game, this test may start to fail and you 
  may need to adjust it.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <stdio.h>
#include "game.h"
#include "sounds.h"

uint32_t time = 0;

int8_t SFG_keyPressed(uint8_t key)
{
  return 0;
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
}

uint32_t SFG_getTimeMs()
{
  return time;
}

void SFG_sleepMs(uint16_t timeMs)
{
}

static inline void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
}

void SFG_setMusic(uint8_t value)
{
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
}

void printTestHeading(const char *text)
{
  printf("\n~~~~~ testing: %s ~~~~~\n\n",text);
}

int main(void)
{
  puts("===== TESTING ANARCH =====\n");

  puts("initializing");

  #define ASSERT(text,cond) { printf("checking \"%s\": ",text); if (cond)  puts("OK"); else { puts("ERROR"); return 1; }}

  SFG_init();

  ASSERT("frame == 0",SFG_game.frame == 0);

  printTestHeading("music and sounds");

  {
    static const expectedSamples[] = { 1, 0, 0, 0, 0, 0, 255, 251, 80, 240, 240, 10, 0, 6, 4, 0 };

    uint16_t pos = 0;

    for (uint32_t i = 0; i < (SFG_TRACK_COUNT * SFG_TRACK_SAMPLES); ++i)
    {
      uint8_t sample = SFG_getNextMusicSample();

      if (i % 200000 == 0)
      {
        ASSERT("music sample", sample == expectedSamples[pos]);
        pos++;
      }
    }
  }

  puts("DONE\neverything seems OK");

  return 0;
}
