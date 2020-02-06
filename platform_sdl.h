/**
  @file platform_sdl.h

  This is an SDL2 implementation of the game front end. It can be used to
  compile a native executable or a transpired JS browser version with
  emscripten.

  To compile with emscripten run:

  emcc ./main.c -s USE_SDL=2 -O3 --shell-file HTMLshell.html -o game.html

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_PLATFORM_H
#define _SFG_PLATFORM_H

#include "settings.h"

#include <stdio.h>
#include <SDL2/SDL.h>

#include "palette.h"

#include "sounds.h"

#undef SFG_LOG
#define SFG_LOG(str) printf("game: %s\n",str);

#undef SFG_BACKGROUND_BLUR
#define SFG_BACKGROUND_BLUR 1

const uint8_t *sdlKeyboardState;

uint16_t screen[SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y]; // RGB565 format

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Surface *screenSurface;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  screen[y * SFG_SCREEN_RESOLUTION_X + x] = paletteRGB565[colorIndex];
}

uint32_t SFG_getTimeMs()
{
  return SDL_GetTicks();
}

void SFG_sleepMs(uint16_t timeMs)
{
#ifndef __EMSCRIPTEN__
  usleep(timeMs * 1000);
#endif
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
  int mX, mY;

  SDL_GetMouseState(&mX,&mY);

  *x = mX - SFG_SCREEN_RESOLUTION_X / 2;
  *y = mY - SFG_SCREEN_RESOLUTION_Y / 2;

  SDL_WarpMouseInWindow(window,
    SFG_SCREEN_RESOLUTION_X / 2, SFG_SCREEN_RESOLUTION_Y / 2);
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP:
      return sdlKeyboardState[SDL_SCANCODE_UP] ||
             sdlKeyboardState[SDL_SCANCODE_W] ||
             sdlKeyboardState[SDL_SCANCODE_KP_8];
      break;

    case SFG_KEY_RIGHT:
      return sdlKeyboardState[SDL_SCANCODE_RIGHT] ||
             sdlKeyboardState[SDL_SCANCODE_E] ||
             sdlKeyboardState[SDL_SCANCODE_KP_6];
      break;

    case SFG_KEY_DOWN:
      return sdlKeyboardState[SDL_SCANCODE_DOWN] ||
             sdlKeyboardState[SDL_SCANCODE_S] ||
             sdlKeyboardState[SDL_SCANCODE_KP_5] ||
             sdlKeyboardState[SDL_SCANCODE_KP_2]; 
      break;

    case SFG_KEY_LEFT:
      return sdlKeyboardState[SDL_SCANCODE_LEFT] ||
             sdlKeyboardState[SDL_SCANCODE_Q] ||
             sdlKeyboardState[SDL_SCANCODE_KP_4];
      break;

    case SFG_KEY_A:
      return sdlKeyboardState[SDL_SCANCODE_G];
      break;

    case SFG_KEY_B:
      return sdlKeyboardState[SDL_SCANCODE_H] || SDL_GetMouseState(0,0);
      break;

    case SFG_KEY_C:
      return sdlKeyboardState[SDL_SCANCODE_J];
      break;

    case SFG_KEY_JUMP:
      return sdlKeyboardState[SDL_SCANCODE_SPACE];
      break;

    case SFG_KEY_STRAFE_LEFT:
      return sdlKeyboardState[SDL_SCANCODE_A] ||
             sdlKeyboardState[SDL_SCANCODE_KP_7];
      break;

    case SFG_KEY_STRAFE_RIGHT:
      return sdlKeyboardState[SDL_SCANCODE_D] ||
             sdlKeyboardState[SDL_SCANCODE_KP_9];
      break;

    case SFG_KEY_MAP:
      return sdlKeyboardState[SDL_SCANCODE_TAB];
      break;

    case SFG_KEY_TOGGLE_FREELOOK:
      return sdlKeyboardState[SDL_SCANCODE_T];
      break;

    case SFG_KEY_NEXT_WEAPON:
      return sdlKeyboardState[SDL_SCANCODE_M];
      break;

    case SFG_KEY_PREVIOUS_WEAPON:
      return sdlKeyboardState[SDL_SCANCODE_N];
      break;

    default: return 0; break;
  }
}
  
int running;

void mainLoopIteration()
{
  SDL_PumpEvents(); // updates the keyboard state

  if (sdlKeyboardState[SDL_SCANCODE_ESCAPE])
    running = 0;

  SFG_mainLoopBody();

  SDL_UpdateTexture(texture,NULL,screen,SFG_SCREEN_RESOLUTION_X * sizeof(uint16_t));

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer,texture,NULL,NULL);
  SDL_RenderPresent(renderer);
}   

#ifdef __EMSCRIPTEN__
typedef void (*em_callback_func)(void);
void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
#endif

uint8_t audioBuff[SFG_SFX_SAMPLE_COUNT];
uint32_t audioPos = 0;

void audioFillCallback(void *userdata, uint8_t *s, int l)
{
  if (audioPos >= SFG_SFX_SAMPLE_COUNT)
    SDL_PauseAudio(1);

  for (int i = 0; i < l; ++i)
    if (audioPos < SFG_SFX_SAMPLE_COUNT)
    {
      s[i] = audioBuff[audioPos];
      audioPos++;
    }
    else
      s[i] = 127;
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  uint8_t volumeStep = volume / 16;

  for (int i = 0; i < SFG_SFX_SAMPLE_COUNT; ++i)
    audioBuff[i] = SFG_GET_SFX_SAMPLE(soundIndex,i) * volumeStep;

  audioPos = 0;
 
  SDL_PauseAudio(0);
}

int main(int argc, char *argv[])
{
  uint8_t argHelp = 0;
  uint8_t argForceWindow = 0;
  uint8_t argForceFullscreen = 0;

  for (uint8_t i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == 0)
      argHelp = 1;
    else if (argv[i][0] == '-' && argv[i][1] == 'w' && argv[i][2] == 0)       
      argForceWindow = 1;
    else if (argv[i][0] == '-' && argv[i][1] == 'f' && argv[i][2] == 0)       
      argForceFullscreen = 1;
    else
      printf("SDL: unknown argument: %s\n",argv[i]); 
  }

  if (argHelp)
  {
    printf("TODOGAME, a suckless first person shooter game (SDL2 frontend)\n\n");
    printf("version TODO, by Miloslav Ciz, released under CC0 1.0 + waiver of all IP\n");
    printf("possible arguments:\n\n");
    printf("-h       print this help and end\n");
    printf("-w       force run in window\n");
    printf("-f       force run fullscreen\n\n");
    printf("controls:\n");
    printf("TODO\n");

    return 0;
  }

  printf("SDL: starting\n");

  printf("SDL: initializing SDL\n");

  window =
    SDL_CreateWindow("raycasting", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, SFG_SCREEN_RESOLUTION_X, SFG_SCREEN_RESOLUTION_Y,
    SDL_WINDOW_SHOWN); 

  renderer = SDL_CreateRenderer(window,-1,0);

  texture =
    SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB565,SDL_TEXTUREACCESS_STATIC,
    SFG_SCREEN_RESOLUTION_X,SFG_SCREEN_RESOLUTION_Y);

  screenSurface = SDL_GetWindowSurface(window);

#if SFG_FULLSCREEN
  argForceFullscreen = 1;
#endif

  if (!argForceWindow && argForceFullscreen)
  {
    printf("SDL: setting fullscreen\n");
    SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
  }

  sdlKeyboardState = SDL_GetKeyboardState(NULL);

  SDL_ShowCursor(0);

  SFG_init(SDL_INIT_AUDIO);

  SDL_AudioSpec audioSpec;

  audioSpec.callback = audioFillCallback;
  audioSpec.userdata = 0;
  audioSpec.freq = 8000;
  audioSpec.format = AUDIO_U8;
  audioSpec.channels = 1;

  if (SDL_OpenAudio(&audioSpec, NULL) < 0)
    printf("SDL: could not initialize audio\n");

  running = 1;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainLoopIteration,0,1);
#else
  while (running)
    mainLoopIteration();
#endif

  printf("SDL: freeing SDL\n");

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer); 
  SDL_DestroyWindow(window); 
  SDL_CloseAudio();

  printf("SDL: ending\n");

  return 0;
}

#endif // guard
