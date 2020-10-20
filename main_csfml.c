/**
  @file main_csfml.c

  This is an csfml (C binding for SFML) implementation of the game front end.
  This is another alternative to the SDL for the PC. This front end is more
  minimal and simple than the SDL, so it's better as a learning resource.

  by Miloslav Ciz (drummyfish), 2020

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <stdint.h>
#include <stdio.h>

#define SFG_SCREEN_RESOLUTION_X 640
#define SFG_SCREEN_RESOLUTION_Y 480

#define SFG_DITHERED_SHADOW 1
#define SFG_DIMINISH_SPRITES 1
#define SFG_RESOLUTION_SCALEDOWN 1

#define WINDOW_SIZE (SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y)

#include "game.h"

uint32_t windowPixels[WINDOW_SIZE];

uint32_t paletteRGB32[256]; // SFML can't do 565, so precompute RGB here

sfClock *clock;
sfRenderWindow* window;

int8_t SFG_keyPressed(uint8_t key)
{
  #define k(x) sfKeyboard_isKeyPressed(x)

  switch (key)
  {
    case SFG_KEY_UP:
      return k(sfKeyW) || k(sfKeyUp) || k(sfKeyNum8);
      break;

    case SFG_KEY_RIGHT:
      return k(sfKeyE) || k(sfKeyRight) || k(sfKeyNum6);
      break;

    case SFG_KEY_DOWN:
      return k(sfKeyS) || k(sfKeyDown) || k(sfKeyNum5) || k (sfKeyNum2);
      break;

    case SFG_KEY_LEFT:
      return k(sfKeyQ) || k(sfKeyLeft) || k(sfKeyNum4);
      break;

    case SFG_KEY_A:
      return k(sfKeyG) || k(sfKeyReturn) || k(sfKeyLShift);
      break;

    case SFG_KEY_B:
      return k(sfKeyH) || k(sfKeyLControl) || k(sfKeyRControl)
             || sfMouse_isButtonPressed(sfMouseLeft);
      break;

    case SFG_KEY_C:
      return k(sfKeyJ);
      break;

    case SFG_KEY_JUMP:
      return k(sfKeySpace);
      break;

    case SFG_KEY_STRAFE_LEFT:
      return k(sfKeyA) || k(sfKeyNum7);
      break;

    case SFG_KEY_STRAFE_RIGHT:
      return k(sfKeyD) || k(sfKeyNum9);
      break;

    case SFG_KEY_MAP:
      return k(sfKeyTab);
      break;

    case SFG_KEY_TOGGLE_FREELOOK:
      return sfMouse_isButtonPressed(sfMouseRight);
      break;

    case SFG_KEY_NEXT_WEAPON:
/* TODO
      if (sdlKeyboardState[SDL_SCANCODE_P] || sdlKeyboardState[SDL_SCANCODE_X])
        return 1;

      if (sdlMouseWheelState > 0)
      {
        sdlMouseWheelState--;
        return 1;
      }
      */  
      return 0;
      break;

    case SFG_KEY_PREVIOUS_WEAPON:
/* TODO
      if (sdlKeyboardState[SDL_SCANCODE_O] || sdlKeyboardState[SDL_SCANCODE_Y]
          || sdlKeyboardState[SDL_SCANCODE_Z])
        return 1;

      if (sdlMouseWheelState < 0)
      {
        sdlMouseWheelState++;
        return 1;
      }
        
      return 0;
*/
      break;

    case SFG_KEY_MENU:
      return sfKeyboard_isKeyPressed(sfKeyEscape);
      break;

    default: return 0; break;

    #undef k
  }
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
  sfVector2u s = sfWindow_getSize(window);
  sfVector2i p = sfMouse_getPosition(window);

  s.x /= 2;
  s.y /= 2;

  *x = p.x - s.x;
  *y = p.y - s.y;

  p.x = s.x;
  p.y = s.y;

  sfMouse_setPosition(p,window);
}

uint32_t SFG_getTimeMs()
{
  return sfClock_getElapsedTime(clock).microseconds / 1000 ;
}

void SFG_sleepMs(uint16_t timeMs)
{
  sfTime t;
  t.microseconds = timeMs * 1000;
  sfSleep(t);
}

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  windowPixels[y * SFG_SCREEN_RESOLUTION_X + x] = paletteRGB32[colorIndex];
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
}

void SFG_enableMusic(uint8_t enable)
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

int main()
{
  sfVideoMode mode = {SFG_SCREEN_RESOLUTION_X, SFG_SCREEN_RESOLUTION_Y, 32};
  sfEvent event;
  clock = sfClock_create();
  sfClock_restart(clock);

  SFG_init();

  for (int i = 0; i < 256; ++i) // precompute RGB palette
  {
    uint16_t col565 = paletteRGB565[i];

    paletteRGB32[i] = 0xff000000 | ((col565 << 19) & 0xf80000) |
      ((col565 << 5) & 0xfc00) | ((col565 >> 8) & 0xf8);
  }

  sfTexture* windowTexture = sfTexture_create(SFG_SCREEN_RESOLUTION_X,SFG_SCREEN_RESOLUTION_Y);
  sfTexture_setSmooth(windowTexture,sfTrue);

  sfSprite* windowSprite = sfSprite_create();

  window = sfRenderWindow_create(mode, "Anarch", sfResize | sfClose, NULL);
  sfSprite_setTexture(windowSprite, windowTexture, sfTrue);

  sfWindow_setMouseCursorVisible(window,sfFalse);
  sfWindow_setVerticalSyncEnabled(window,sfFalse);

  while (sfRenderWindow_isOpen(window))
  {
    while (sfRenderWindow_pollEvent(window,&event))
      if (event.type == sfEvtClosed)
        sfRenderWindow_close(window);

    if (!SFG_mainLoopBody())
      break;

    sfTexture_updateFromPixels(windowTexture,windowPixels,SFG_SCREEN_RESOLUTION_X,SFG_SCREEN_RESOLUTION_Y,0,0);
    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawSprite(window, windowSprite, NULL);
    sfRenderWindow_display(window);
  }

  sfSprite_destroy(windowSprite);
  sfTexture_destroy(windowTexture);
  sfRenderWindow_destroy(window);
  sfClock_destroy(clock);

  return 0;
}
