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

#include <SFML/Audio/Types.h>

#define SFG_SCREEN_RESOLUTION_X 640
#define SFG_SCREEN_RESOLUTION_Y 480

#define SFG_DITHERED_SHADOW 1
#define SFG_DIMINISH_SPRITES 1
#define SFG_RESOLUTION_SCALEDOWN 1

#define WINDOW_SIZE (SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y)

#include "game.h"
#include "sounds.h"

uint32_t windowPixels[WINDOW_SIZE];

uint32_t paletteRGB32[256]; // SFML can't do 565, so precompute RGB here

sfClock *clock;
sfRenderWindow* window;

int8_t SFG_keyPressed(uint8_t key)
{
  #define k(x) sfKeyboard_isKeyPressed(sfKey ## x)

  switch (key)
  {
    case SFG_KEY_UP: return k(W) || k(Up) || k(Num8); break;
    case SFG_KEY_RIGHT: return k(E) || k(Right) || k(Num6);
      break;
    case SFG_KEY_DOWN:
      return k(S) || k(Down) || k(Num5) || k (Num2); break;
    case SFG_KEY_LEFT: return k(Q) || k(Left) || k(Num4); break;
    case SFG_KEY_A:
      return k(J) || k(Return) || k(LShift) ||
      sfMouse_isButtonPressed(sfMouseLeft); break;
    case SFG_KEY_B: return k(K) || k(LControl) || k(RControl); break;
    case SFG_KEY_C: return k(L); break;
    case SFG_KEY_JUMP: return k(Space); break;
    case SFG_KEY_STRAFE_LEFT: return k(A) || k(Num7); break;
    case SFG_KEY_STRAFE_RIGHT: return k(D) || k(Num9); break;
    case SFG_KEY_MAP: return k(Tab); break;
    case SFG_KEY_CYCLE_WEAPON: return k(F);
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

    case SFG_KEY_MENU: return sfKeyboard_isKeyPressed(sfKeyEscape); break;
    default: return 0; break;

    #undef k
  }

  return 0;
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
  sfVector2u s = sfWindow_getSize((const sfWindow *) window);
  sfVector2i p = sfMouse_getPosition((const sfWindow *) window);

  s.x /= 2;
  s.y /= 2;

  *x = p.x - s.x;
  *y = p.y - s.y;

  p.x = s.x;
  p.y = s.y;

  sfMouse_setPosition(p,(const sfWindow *) window);
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


void SFG_setMusic(uint8_t value)
{
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
  FILE *f = fopen("anarch.sav","wb");

  if (f == NULL)
    return;

  fwrite(data,1,SFG_SAVE_SIZE,f);

  fclose(f);
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  FILE *f = fopen("anarch.sav","rb");

  if (f != NULL)
  {
    fread(data,1,SFG_SAVE_SIZE,f);
    fclose(f);
  }

  return 1;
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{



}



uint64_t soundPos = 0;

sfSoundStream *sound;

#define AUDIO_BUFFER_SIZE (SFG_SFX_SAMPLE_COUNT * 2)

int16_t audioBuffer[AUDIO_BUFFER_SIZE];

sfBool soundFill(sfSoundStreamChunk *data, void *userdata)
{

printf("sasa\n");
  data->samples = audioBuffer;
  data->sampleCount = AUDIO_BUFFER_SIZE;

  return sfTrue;
}

void soundSeek(sfTime t, void *userData)
{

}

int main()
{
  sfVideoMode mode = {SFG_SCREEN_RESOLUTION_X, SFG_SCREEN_RESOLUTION_Y, 32};
  sfEvent event;
  clock = sfClock_create();
  sfClock_restart(clock);

  SFG_init();

for (int i = 0; i < AUDIO_BUFFER_SIZE; ++i)
  audioBuffer[i] = SFG_getNextMusicSample() * 64;

sound = sfSoundStream_create( soundFill,soundSeek  ,1,8000,0);

sfSoundStream_play(sound);




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

  sfWindow_setMouseCursorVisible((sfWindow *) window,sfFalse);
  sfWindow_setVerticalSyncEnabled((sfWindow *) window,sfFalse);

  

  while (sfRenderWindow_isOpen(window))
  {
    while (sfRenderWindow_pollEvent(window,&event))
      if (event.type == sfEvtClosed)
        sfRenderWindow_close(window);

    if (!SFG_mainLoopBody())
      break;

    sfTexture_updateFromPixels(windowTexture,(const sfUint8 *) windowPixels,SFG_SCREEN_RESOLUTION_X,SFG_SCREEN_RESOLUTION_Y,0,0);
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
