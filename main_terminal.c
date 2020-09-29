/**
  @file main_pokitto.cpp

  This is Linux terminal implementation of the game front end. This needs root
  priviledges (sudo) to work! This frontend is more of an experiment, don't
  expect it to work perfectly and everywhere.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#define SFG_SCREEN_RESOLUTION_X 95
#define SFG_SCREEN_RESOLUTION_Y 43
#define SFG_DITHERED_SHADOW 1
#define SFG_FPS 20

#include "game.h"

#define NEWLINES 10

#define SCREENSIZE \
  (NEWLINES + (SFG_SCREEN_RESOLUTION_X + 1) * SFG_SCREEN_RESOLUTION_Y)

char screen[SCREENSIZE];

const char shades[] = // adjust according to your terminal
  {
    ' ','.','-',':','\\','h','M','@',  // grey
    '`','.',',',';','/','r','=','n'    // non-grey
  }; 

uint32_t timeStart;

typedef struct
{
  struct timeval time;
    __u16 type;
    __u16 code;
    __s32 value;
} InputEvent;

InputEvent event;

#define TOTAL_KEYS 9

uint16_t keyCodes[TOTAL_KEYS] =
  {
    KEY_W,
    KEY_S,
    KEY_A,
    KEY_D,
    KEY_SPACE,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_Q
  };

uint8_t keyStates[TOTAL_KEYS];

uint32_t getTime()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  screen[NEWLINES + y * (SFG_SCREEN_RESOLUTION_X + 1) + x] = 
    shades[(colorIndex > 7) * 8 + colorIndex % 8];
}

uint32_t SFG_getTimeMs()
{
  clock_t timeNow = clock();
  return getTime() - timeStart;
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  return 0;
}

void SFG_sleepMs(uint16_t timeMs)
{
  usleep(timeMs * 1000);
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP:     return keyStates[0]; break;
    case SFG_KEY_RIGHT:  return keyStates[3]; break;
    case SFG_KEY_DOWN:   return keyStates[1]; break;
    case SFG_KEY_LEFT:   return keyStates[2]; break;
    case SFG_KEY_A:      return keyStates[5]; break;
    case SFG_KEY_B:      return keyStates[6]; break;
    case SFG_KEY_C:      return keyStates[7]; break;
    case SFG_KEY_MAP:    return keyStates[8]; break;
    default:             return 0; break;
  }
}

void SFG_enableMusic(uint8_t enable)
{
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
}

int nextFlush = 0;

int main()
{
  int devFile;

  timeStart = getTime();

  devFile = open("/dev/input/event0", O_RDONLY);

  fcntl(devFile, F_SETFL, O_NONBLOCK);

  SFG_init();

  for (uint16_t i = 0; i < TOTAL_KEYS; ++i)
    keyStates[i] = 0;

  for (uint16_t i = 0; i < NEWLINES; ++i)
    screen[i] = '\n';

  for (uint16_t i = 1; i <= SFG_SCREEN_RESOLUTION_Y; ++i)
    screen[NEWLINES + i * (SFG_SCREEN_RESOLUTION_X + 1) - 1] = '\n';

  setvbuf(stdout, NULL, _IOFBF, SCREENSIZE + 1);

  while (1)
  {
    while (1)
    {
      int n = read(devFile, &event, sizeof(event));

      if (n <= 0)
        break;

      if (event.type == EV_KEY && (event.value == 1 || event.value == 0))
      {
        for (uint8_t i = 0; i < TOTAL_KEYS; ++i)
          if (event.code == keyCodes[i])
          {
            keyStates[i] = event.value;
            break;
          }
      }
    }
   
    uint32_t t = SFG_getTimeMs();

    if (t >= nextFlush)
    {
      puts(screen);
      fflush(stdout);
      nextFlush = t + 200; // 5 rendering FPS
    }

    if (!SFG_mainLoopBody())
      break;
  }
}
