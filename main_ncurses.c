/**
  @file main_ncurses.c

  This is ncurses (terminal) implementation of the game front end. This isn't
  a full fledged version but rather a "show off" of what can be done with the
  game; especially the limited ability of ncurses to handle input makes this
  version very hard to play.

  by Miloslav Ciz (drummyfish), 2024

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is to
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define SFG_SCREEN_RESOLUTION_X 120
#define SFG_SCREEN_RESOLUTION_Y 40
#define SFG_FPS 30

#include "game.h"

uint8_t ncScreen[SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y];
uint8_t ncButtonStates[SFG_KEY_COUNT];
uint32_t timeStart;

const char asciiPalette[] = 
#if 1
  " .',:;lcoxkXK0MW";
#else
  " -.,;!/clfsxaVO#"; 
#endif

uint32_t currentTime()
{
  struct timespec t;
  timespec_get(&t,TIME_UTC);
  return 1000 * ((int64_t) t.tv_sec) + ((int64_t) t.tv_nsec) / 1000000;
}

int8_t SFG_keyPressed(uint8_t key)
{
  return ncButtonStates[key];
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
  return;
}

uint32_t SFG_getTimeMs()
{
  return currentTime() - timeStart;
}

void SFG_sleepMs(uint16_t timeMs)
{
  usleep(1000 * timeMs);
}

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  ncScreen[y * SFG_SCREEN_RESOLUTION_X + x] = asciiPalette[colorIndex % 16];
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
  return 0;
}

int main(void)
{
  timeStart = currentTime();

  initscr();
  halfdelay(1);
  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);

  SFG_init();

  int goOn = 1;

  while (goOn)
  {
    erase();
    move(0,0);

    const uint8_t *scr = ncScreen;

    for (int y = 0; y < SFG_SCREEN_RESOLUTION_Y; ++y)
    {
      move(y,0);

      for (int x = 0; x < SFG_SCREEN_RESOLUTION_X; ++x)
      {
        addch(*scr);
        scr++;
      }
    }

    refresh();

    goOn = SFG_mainLoopBody();

    for (int i = 0; i < SFG_KEY_COUNT; ++i)
      ncButtonStates[i] = 0;

    while (1)
    {
      int c = getch();

      if (c == ERR)
        break;

      switch (c)
      {
        case KEY_UP:      ncButtonStates[SFG_KEY_UP] = 1; break;
        case KEY_LEFT:    ncButtonStates[SFG_KEY_LEFT] = 1; break;
        case KEY_RIGHT:   ncButtonStates[SFG_KEY_RIGHT] = 1; break;
        case KEY_DOWN:    ncButtonStates[SFG_KEY_DOWN] = 1; break;
        case 'a':
        case KEY_ENTER:   ncButtonStates[SFG_KEY_A] = 1; break;
        case KEY_CANCEL:
        case KEY_CLOSE:
        case 's':         ncButtonStates[SFG_KEY_B] = 1; break;
        case 'd':         ncButtonStates[SFG_KEY_C] = 1; break;
        case ' ':         ncButtonStates[SFG_KEY_JUMP] = 1; break;
        case 'q':         ncButtonStates[SFG_KEY_MENU] = 1; break;
        case 'f':         ncButtonStates[SFG_KEY_NEXT_WEAPON] = 1; break;
        case 'g':         ncButtonStates[SFG_KEY_PREVIOUS_WEAPON] = 1; break;
        case 'n':         ncButtonStates[SFG_KEY_STRAFE_LEFT] = 1; break;
        case 'm':         ncButtonStates[SFG_KEY_STRAFE_RIGHT] = 1; break;
        default: break;
      }
    }
  }

  endwin();

  return 0;
}
