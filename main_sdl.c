/**
  @file main_sdl.c

  This is an SDL2 implementation of the game front end. It can be used to
  compile a native executable or a transpiled JS browser version with
  emscripten.

  To compile with emscripten run:

  emcc ./main_sdl.c -s USE_SDL=2 -O3 --shell-file HTMLshell.html -o game.html

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__APPLE__)
  #define SFG_OS_IS_MALWARE 1
#endif

#define SFG_BACKGROUND_BLUR 1

#define SFG_FPS 60

#define SFG_LOG(str) puts(str);

//  #define SFG_START_LEVEL 6
//  #define SFG_IMMORTAL 1
  #define SFG_UNLOCK_DOOR 1
//  #define SFG_REVEAL_MAP 1
//  #define SFG_INFINITE_AMMO 1

//  #define SFG_SCREEN_RESOLUTION_X 80
//  #define SFG_SCREEN_RESOLUTION_Y 64

/*
  SDL is easier to play thanks to nice controls, so make the player take full
  damage to make it a bit harder.
*/
#define SFG_PLAYER_DAMAGE_MULTIPLIER 1024

#define MUSIC_VOLUME 5

#ifdef __EMSCRIPTEN__
  #define SFG_FPS 30
  #define SFG_SCREEN_RESOLUTION_X 640
  #define SFG_SCREEN_RESOLUTION_Y 480
  #define SFG_CAN_EXIT 0
  #define SFG_RESOLUTION_SCALEDOWN 2

  #include <emscripten.h>
#endif

#include <stdio.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "sounds.h"

const uint8_t *sdlKeyboardState;
uint8_t sdlMouseButtonState = 0;
int8_t sdlMouseWheelState = 0;

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

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
  FILE *f = fopen("anarch.sav","wb");

  puts("SDL: opening and writing save file");

  if (f == NULL)
  {
    puts("SDL: could not open the file!");
    return;
  }

  fwrite(data,1,SFG_SAVE_SIZE,f);

  fclose(f);
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
#ifndef __EMSCRIPTEN__
  FILE *f = fopen("anarch.sav","rb");

  puts("SDL: opening and reading save file");

  if (f == NULL)
  {
    puts("SDL: no save file to open");
  }
  else
  {
    fread(data,1,SFG_SAVE_SIZE,f);
    fclose(f);
  }

  return 1;
#else
  return 0;
#endif
}

void SFG_sleepMs(uint16_t timeMs)
{
#ifndef __EMSCRIPTEN__
  usleep(timeMs * 1000);
#endif
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
#ifndef __EMSCRIPTEN__
  int mX, mY;

  SDL_GetMouseState(&mX,&mY);

  *x = mX - SFG_SCREEN_RESOLUTION_X / 2;
  *y = mY - SFG_SCREEN_RESOLUTION_Y / 2;

  SDL_WarpMouseInWindow(window,
    SFG_SCREEN_RESOLUTION_X / 2, SFG_SCREEN_RESOLUTION_Y / 2);
#endif
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
      return sdlKeyboardState[SDL_SCANCODE_G] ||
             sdlKeyboardState[SDL_SCANCODE_RETURN] ||
             sdlKeyboardState[SDL_SCANCODE_LSHIFT];
      break;

    case SFG_KEY_B:
      return sdlKeyboardState[SDL_SCANCODE_H] || 
             (sdlMouseButtonState & SDL_BUTTON_LMASK) ||
             sdlKeyboardState[SDL_SCANCODE_LCTRL] ||
             sdlKeyboardState[SDL_SCANCODE_RCTRL];
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
      return sdlMouseButtonState & SDL_BUTTON_RMASK;
      break;

    case SFG_KEY_NEXT_WEAPON:
      if (sdlKeyboardState[SDL_SCANCODE_P] || sdlKeyboardState[SDL_SCANCODE_X])
        return 1;

      if (sdlMouseWheelState > 0)
      {
        sdlMouseWheelState--;
        return 1;
      }
        
      return 0;
      break;

    case SFG_KEY_PREVIOUS_WEAPON:
      if (sdlKeyboardState[SDL_SCANCODE_O] || sdlKeyboardState[SDL_SCANCODE_Y]
          || sdlKeyboardState[SDL_SCANCODE_Z])
        return 1;

      if (sdlMouseWheelState < 0)
      {
        sdlMouseWheelState++;
        return 1;
      }
        
      return 0;
      break;

    case SFG_KEY_MENU:
      return sdlKeyboardState[SDL_SCANCODE_ESCAPE];
      break;

    default: return 0; break;
  }
}
  
int running;

void mainLoopIteration()
{
  SDL_Event event;

#ifdef __EMSCRIPTEN__
  // Hack, without it sound won't work because of shitty browser audio policies.

  if (SFG_game.frame % 512 == 0)
    SDL_PauseAudio(0);
#endif

  while (SDL_PollEvent(&event)) // also automatically updates sdlKeyboardState
  {
    if(event.type == SDL_MOUSEWHEEL)
    {
      if (event.wheel.y > 0) // scroll up
        sdlMouseWheelState++;
      else if (event.wheel.y < 0) // scroll down
        sdlMouseWheelState--;
    }
  }

  sdlMouseButtonState = SDL_GetMouseState(NULL,NULL);

  if (!SFG_mainLoopBody())
    running = 0;

  SDL_UpdateTexture(texture,NULL,screen,SFG_SCREEN_RESOLUTION_X * sizeof(uint16_t));

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer,texture,NULL,NULL);
  SDL_RenderPresent(renderer);
}

#ifdef __EMSCRIPTEN__
typedef void (*em_callback_func)(void);
void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
#endif

#define AUDIO_ZERO 32768

uint16_t audioBuff[SFG_SFX_SAMPLE_COUNT];
uint16_t audioPos = 0;

static inline uint16_t mixSamples(uint16_t sample1, uint16_t sample2)
{
  return (sample1 >> 1) + (sample2 >> 1);
}

uint8_t musicOn = 1;

void audioFillCallback(void *userdata, uint8_t *s, int l)
{
  uint16_t *s16 = (uint16_t *) s;

  for (int i = 0; i < l / 2; ++i)
  {
    s16[i] = musicOn ?
      mixSamples(audioBuff[audioPos],SFG_getNextMusicSample() << MUSIC_VOLUME) :
      audioBuff[audioPos];

    audioBuff[audioPos] = AUDIO_ZERO;
    audioPos = (audioPos < SFG_SFX_SAMPLE_COUNT - 1) ? (audioPos + 1) : 0;
  }
}

void SFG_enableMusic(uint8_t enable)
{
  musicOn = enable;
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  uint16_t pos = audioPos;
  uint8_t volumeShift = 15 - volume / 16;
  uint16_t baseLevel = AUDIO_ZERO - (0x8000 >> volumeShift);

  for (int i = 0; i < SFG_SFX_SAMPLE_COUNT; ++i)
  {
    audioBuff[pos] = mixSamples(audioBuff[pos],baseLevel +
     ((SFG_GET_SFX_SAMPLE(soundIndex,i) << 8) >> volumeShift));

    pos = (pos < SFG_SFX_SAMPLE_COUNT - 1) ? (pos + 1) : 0;
  }
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
      puts("SDL: unknown argument"); 
  }

  if (argHelp)
  {
    puts("Anarch, version " SFG_VERSION_STRING "\n");
    puts("Anarch is a unique suckless FPS game. Collect weapons and items and destroy");
    puts("robot enemies in your way in order to get to the level finish. Some door are");
    puts("locked and require access cards. Good luck!\n");
    puts("created by Miloslav \"drummyfish\" Ciz, released under CC0 1.0 (public domain)\n");
    puts("CLI flags:\n");
    puts("-h   print this help and exit");
    puts("-w   force window");
    puts("-f   force fullscreen\n");
    puts("controls:\n");
    puts("- arrows, numpad, [W] [S] [A] [D] [Q] [R]: movement");
    puts("- mouse: rotation, [LMB] shoot, [RMB] toggle free look, wheel weapon change");
    puts("- [SPACE]: jump");
    puts("- [G] [RETURN] [SHIFT]: game A button (confirm, strafe)");
    puts("- [H] [CTRL]: game B button (cancel, shoot)");
    puts("- [O] [P] [X] [Y] [Z]: change weapons");
    puts("- [TAB]: map");
    puts("- [ESCAPE]: menu");

    return 0;
  }

  puts("SDL: initializing SDL");

  SFG_init();

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
    puts("SDL: setting fullscreen");
    SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN_DESKTOP);
  }

  sdlKeyboardState = SDL_GetKeyboardState(NULL);

  SDL_ShowCursor(0);

  SDL_Init(SDL_INIT_AUDIO);

  SDL_AudioSpec audioSpec;

  SDL_memset(&audioSpec, 0, sizeof(audioSpec));
  audioSpec.callback = audioFillCallback;
  audioSpec.freq = 8000;
  audioSpec.format = AUDIO_U16;
  audioSpec.channels = 1;
#ifdef __EMSCRIPTEN__
  audioSpec.samples = 1024;
#else
  audioSpec.samples = 256;
#endif

  if (SDL_OpenAudio(&audioSpec,NULL) < 0)
    puts("SDL: could not initialize audio");

  for (int16_t i = 0; i < SFG_SFX_SAMPLE_COUNT; ++i)
    audioBuff[i] = AUDIO_ZERO;

  SDL_PauseAudio(0);

  running = 1;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainLoopIteration,0,1);
#else
  while (running)
    mainLoopIteration();
#endif

  puts("SDL: freeing SDL");

  SDL_PauseAudio(1);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer); 
  SDL_DestroyWindow(window); 
  SDL_CloseAudio();

  puts("SDL: ending");

  return 0;
}
