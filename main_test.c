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

#define SFG_SCREEN_RESOLUTION_X 67
#define SFG_SCREEN_RESOLUTION_Y 31
#define SFG_BACKGROUND_BLUR 1
#define SFG_DITHERED_SHADOW 1
#define SFG_FPS 30

#include "game.h"
#include "sounds.h"

uint8_t screen[SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y];
uint8_t keys[SFG_KEY_COUNT];

uint32_t time = 0;

int8_t SFG_keyPressed(uint8_t key)
{
  return keys[key];
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
  screen[y * SFG_SCREEN_RESOLUTION_X + x] = colorIndex;
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

const char colors[8] = " .,-;imX";

void printScreen()
{
  const char *c = screen;

  for (uint8_t y = 0; y < SFG_SCREEN_RESOLUTION_Y; ++y)
  {
    for (uint8_t x = 0; x < SFG_SCREEN_RESOLUTION_X; ++x)
    {
      putchar(*c != 7 ? colors[(*c) % 8] : '@');
      ++c;
    }

    putchar('\n');
  }
}

int main(void)
{
  puts("===== TESTING ANARCH =====\n");

  puts("initializing");

  #define ASSERT(text,cond) { printf("checking \"%s\": ",text); if (cond)  puts("OK"); else { puts("ERROR"); return 1; }}

  SFG_init();

  ASSERT("frame == 0",SFG_game.frame == 0);

  {
    printTestHeading("music and sounds");

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

    ASSERT("sfx sample",SFG_GET_SFX_SAMPLE(0,0) == 128);
    ASSERT("sfx sample",SFG_GET_SFX_SAMPLE(1,200) == 112);
    ASSERT("sfx sample",SFG_GET_SFX_SAMPLE(3,512) == 112);
    ASSERT("sfx sample",SFG_GET_SFX_SAMPLE(4,1000) == 128);
  }

  {
    printTestHeading("levels");
    
    SFG_TileDefinition t;
    uint8_t p;

    t = SFG_getMapTile(&SFG_level1,10,8,&p);
    ASSERT("level1 tile",SFG_TILE_FLOOR_HEIGHT(t) == 14 && SFG_TILE_CEILING_HEIGHT(t) == 0 && SFG_TILE_FLOOR_TEXTURE(t) == 4 && p == 0);

    t = SFG_getMapTile(&SFG_level3,9,20,&p);
    ASSERT("level3 tile",SFG_TILE_FLOOR_HEIGHT(t) == 17 && SFG_TILE_CEILING_HEIGHT(t) == 13 && SFG_TILE_FLOOR_TEXTURE(t) == 0 && p == 128);

    t = SFG_getMapTile(&SFG_level5,-9,0,&p);
    ASSERT("outside tile",SFG_TILE_FLOOR_HEIGHT(t) == 31 && SFG_TILE_CEILING_HEIGHT(t) == 0 && SFG_TILE_FLOOR_TEXTURE(t) == 7 && p == 0);
  }
 
  {
    printTestHeading("gameplay");

    for (uint8_t i = 0; i < SFG_KEY_COUNT; ++i)
      keys[i] = 0;

    #define STEP(ms) { printf("(fr %d, step %d ms) ",SFG_game.frame,ms); time += ms; SFG_mainLoopBody(); }

    #define PRESS(k) { printf("(press %d) ",k); keys[k] = 1; }
    #define RELEASE(k) { printf("(release %d) ",k); keys[k] = 0; }

    STEP(10)
    STEP(100)
    PRESS(SFG_KEY_DOWN);
    STEP(1000)
//    RELEASE(SFG_KEY_A);
//    STEP(5000)

printScreen();

    #undef PRESS
    #undef RELEASE
    #undef STEP
  }
 
  puts("======================================\n\nDone.\nEverything seems OK.");

  return 0;
}
