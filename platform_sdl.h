#ifndef _SFG_PLATFORM_H
#define _SFG_PLATFORM_H

#include "settings.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <unistd.h>

#include "palette.h"

#undef SFG_LOG
#define SFG_LOG(str) printf("game: %s\n",str);

const uint8_t *sdlKeyboardState;

uint16_t screen[SFG_RESOLUTION_X * SFG_RESOLUTION_Y]; // RGB565 format

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  screen[y * SFG_RESOLUTION_X + x] = paletteRGB565[colorIndex];
}

uint32_t SFG_getTimeMs()
{
  return SDL_GetTicks();
}

void SFG_sleepMs(uint16_t timeMs)
{
  usleep(timeMs * 1000);
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP: return sdlKeyboardState[SDL_SCANCODE_UP]; break;
    case SFG_KEY_RIGHT: return sdlKeyboardState[SDL_SCANCODE_RIGHT]; break;
    case SFG_KEY_DOWN: return sdlKeyboardState[SDL_SCANCODE_DOWN]; break;
    case SFG_KEY_LEFT: return sdlKeyboardState[SDL_SCANCODE_LEFT]; break;
    case SFG_KEY_A: return sdlKeyboardState[SDL_SCANCODE_A]; break;
    case SFG_KEY_B: return sdlKeyboardState[SDL_SCANCODE_S]; break;
    case SFG_KEY_C: return sdlKeyboardState[SDL_SCANCODE_D]; break;

    case SFG_KEY_JUMP: return sdlKeyboardState[SDL_SCANCODE_SPACE]; break;
    case SFG_KEY_STRAFE_LEFT: return sdlKeyboardState[SDL_SCANCODE_Q]; break;
    case SFG_KEY_STRAFE_RIGHT: return sdlKeyboardState[SDL_SCANCODE_E]; break;

    default: return 0; break;
  }
}

int main()
{
  printf("SDL: starting\n");

  printf("SDL: initializing SDL\n");

  SDL_Window *window =
    SDL_CreateWindow("raycasting", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, SFG_RESOLUTION_X, SFG_RESOLUTION_Y,
    SDL_WINDOW_SHOWN); 

  SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);

  SDL_Texture *texture =
    SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGB565,SDL_TEXTUREACCESS_STATIC,
    SFG_RESOLUTION_X,SFG_RESOLUTION_Y);

  SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

#if SFG_FULLSCREEN
  SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif

  sdlKeyboardState = SDL_GetKeyboardState(NULL);

  SFG_init();

  int running = 1;

  while (running)
  {
    SDL_PumpEvents(); // updates the keyboard state

    if (sdlKeyboardState[SDL_SCANCODE_ESCAPE])
      break;

    SFG_mainLoopBody();

    SDL_UpdateTexture(texture,NULL,screen,SFG_RESOLUTION_X * sizeof(uint16_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,texture,NULL,NULL);
    SDL_RenderPresent(renderer);
  }    

  printf("SDL: freeing SDL\n");

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer); 
  SDL_DestroyWindow(window); 

  printf("SDL: ending\n");

  return 0;
}

#endif // guard
