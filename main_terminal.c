/**
  @file main_terminal.c

  WARNING: VERY EXPERIMENTAL

  This is Linux terminal implementation of the game front end. If you replace
  the input methods, it will most likely run in other terminals as well. This
  needs root priviledges (sudo) to work (because we need to read keyboard and
  mouse inputs)! This frontend is more of an experiment, don't expect it to work
  perfectly and everywhere.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef USE_LINUX_FRAMEBUFFER
  #define USE_LINUX_FRAMEBUFFER 0
#endif

// IMPORTANT: You must set these files correctly:
#define DEV_KEYBOARD "/dev/input/event3"
#define DEV_MOUSE "/dev/input/event1"
#define DEV_TTY "/dev/tty3"
#define DEV_FRAMBUFFER "/dev/fb0"

#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "smallinput.h"

#if USE_LINUX_FRAMEBUFFER
  #include <stdlib.h>
  #include <linux/fb.h>
  #include <linux/kd.h>
  #include <sys/mman.h>
  #include <sys/ioctl.h>
#endif

#if USE_LINUX_FRAMEBUFFER
  #define SFG_SCREEN_RESOLUTION_X 640
  #define SFG_SCREEN_RESOLUTION_Y 480
#else
  #define SFG_SCREEN_RESOLUTION_X 127
  #define SFG_SCREEN_RESOLUTION_Y 42
#endif

#define SFG_DITHERED_SHADOW 1
#define SFG_FPS 30

#include "game.h"

#define SCREENSIZE ((SFG_SCREEN_RESOLUTION_X + 1) * SFG_SCREEN_RESOLUTION_Y + 1)

char screen[SCREENSIZE];
uint8_t *screenUnsigned = (uint8_t *) screen;

const char shades[] = // adjust according to your terminal
  {
    ' ','.','-',':','\\','h','M','@',  // grey
    '`','.',',',';','/','r','=','n'    // non-grey
  }; 

uint32_t timeStart;

uint32_t getTime()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
#if USE_LINUX_FRAMEBUFFER
  screenUnsigned[y * SFG_SCREEN_RESOLUTION_X + x] = 
    colorIndex;
#else
  screen[y * (SFG_SCREEN_RESOLUTION_X + 1) + x] = 
    shades[(colorIndex > 7) * 8 + colorIndex % 8];
#endif
}

uint32_t SFG_getTimeMs()
{
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
  int32_t a,b;

  input_getMousePos(&a,&b);
  *x = a;
  *y = b;
  input_setMousePos(0,0);
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP:     return input_getKey('w') || input_getKey(SMALLINPUT_ARROW_UP); break;
    case SFG_KEY_RIGHT:  return input_getKey('d') || input_getKey(SMALLINPUT_ARROW_RIGHT); break;
    case SFG_KEY_DOWN:   return input_getKey('s') || input_getKey(SMALLINPUT_ARROW_DOWN); break;
    case SFG_KEY_LEFT:   return input_getKey('a') || input_getKey(SMALLINPUT_ARROW_LEFT); break;
    case SFG_KEY_A:      return input_getKey('j') || input_getKey(SMALLINPUT_RETURN) || input_getKey(SMALLINPUT_MOUSE_L); break;
    case SFG_KEY_B:      return input_getKey('k') || input_getKey(SMALLINPUT_CTRL); break;
    case SFG_KEY_C:      return input_getKey('l'); break;
    case SFG_KEY_MAP:    return input_getKey(SMALLINPUT_TAB); break;
    case SFG_KEY_JUMP:   return input_getKey(' '); break;
    case SFG_KEY_MENU:   return input_getKey(SMALLINPUT_ESCAPE); break;
    case SFG_KEY_NEXT_WEAPON: return input_getKey('2'); break;
    case SFG_KEY_PREVIOUS_WEAPON: return input_getKey('1'); break;
    case SFG_KEY_CYCLE_WEAPON: return input_getKey('f'); break;
    case SFG_KEY_TOGGLE_FREELOOK: return input_getKey(SMALLINPUT_MOUSE_R); break;
    default:             return 0; break;
  }
}

void SFG_setMusic(uint8_t value)
{
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
}

int running = 1;

void handleSignal(int signal)
{
#if !USE_LINUX_FRAMEBUFFER
  puts("\033[?25h"); // show cursor
#endif
  running = 0;
}

int main()
{
  signal(SIGINT,handleSignal);
  signal(SIGQUIT,handleSignal);
  signal(SIGTERM,handleSignal);

  timeStart = getTime();

  input_init(SMALLINPUT_MODE_NORMAL,0,0);
  SFG_init();

  screen[SCREENSIZE - 1] = 0; // string terminator

#if USE_LINUX_FRAMEBUFFER

  #define CHECK(w,err,msg)\
    if ((w) == (err)) { puts(msg); return 1; }

  int tty = open(DEV_TTY,O_RDWR);
  CHECK(tty,-1,"couldn't open TTY device")

  CHECK(ioctl(tty,KDSETMODE,KD_GRAPHICS),-1,"couldn't set graphic mode")

  int fb = open(DEV_FRAMBUFFER,O_RDWR);
  CHECK(fb,-1,"couldn't open framebuffer device")

  struct fb_fix_screeninfo fixInfo;
  struct fb_var_screeninfo varInfo;

  CHECK(ioctl(fb,FBIOGET_FSCREENINFO,&fixInfo),-1,"couldn't get fixInfo")
  CHECK(ioctl(fb,FBIOGET_VSCREENINFO,&varInfo),-1,"couldn't get varInfo")

  int bpp = varInfo.bits_per_pixel / 8;
  uint64_t screenSize = varInfo.xres * varInfo.yres * bpp;

  char *fbScreen = mmap(0,screenSize,PROT_READ | PROT_WRITE,MAP_SHARED,fb,0);

  CHECK(fbScreen,MAP_FAILED,"couldn't map framebuffer")

  int r = varInfo.red.offset / 8, 
      g = varInfo.green.offset / 8, 
      b = varInfo.blue.offset / 8,
      t = varInfo.transp.offset / 8;

#else
  for (uint16_t i = 1; i <= SFG_SCREEN_RESOLUTION_Y; ++i)
    screen[i * (SFG_SCREEN_RESOLUTION_X + 1) - 1] = '\n';

  setvbuf(stdout, NULL, _IOFBF, SCREENSIZE + 1);

  for (uint8_t i = 0; i < 100; ++i) // clear screen
    putchar('\n');
      
  puts("\033[?25l"); // hide cursor
#endif  

  while (running)
  {
    input_update();

#if USE_LINUX_FRAMEBUFFER

    char *p = fbScreen;

    int linePad = fixInfo.line_length - SFG_SCREEN_RESOLUTION_X * bpp;

    int index = 0;

    for (int y = 0; y < SFG_SCREEN_RESOLUTION_Y; ++y)
    {
      for (int x = 0; x < SFG_SCREEN_RESOLUTION_X; ++x)
      {
        // inefficient, should be a precomputed RGB32 palette
        uint16_t c = paletteRGB565[screenUnsigned[index]];

        *(p + b) = (c << 3) & 0xf8;
        *(p + g) = (c >> 3) & 0xfc;
        *(p + r) = (c >> 8) & 0xf8;
        p += bpp;
        index++;
      }

      p += linePad;
    }

#else
    puts("\033[0;0H"); // move cursor to 0;0
    puts(screen);
    fflush(stdout);
#endif

    if (!SFG_mainLoopBody())
      running = 0;

//  if (SFG_game.frame > 1000) break; // uncomment for testing, prevents locking

  }
    
  input_end();

#if USE_LINUX_FRAMEBUFFER
  munmap(screen,screenSize);

  ioctl(tty,KDSETMODE,KD_TEXT); // set back to text mode

  close(fb);
  close(tty);
#else
  puts("\033[?25h"); // show cursor
#endif

}
