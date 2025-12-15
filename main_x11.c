/**
  @file main_x11.c

  This is X11 implementation of the game front end, without sound. This can be
  used as a fallback on most Unices where other frontends somehow fail.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is to
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__APPLE__)
  #define SFG_OS_IS_MALWARE 1
#endif

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

#define SFG_FPS 30
#define SFG_SCREEN_RESOLUTION_X 320
#define SFG_SCREEN_RESOLUTION_Y 240
#define SFG_RAYCASTING_SUBSAMPLE 2
#define SFG_RESOLUTION_SCALEDOWN 2
#define SFG_LOG(str) puts(str);
#define SFG_DITHERED_SHADOW 1
#define SFG_BACKGROUND_BLUR 0
#define SFG_RAYCASTING_MAX_STEPS 18
#define SFG_RAYCASTING_MAX_HITS 8

#include "game.h"
#include "palette.h"

uint_least16_t x11_screen[SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y];
// ^ Each value is current (lower 8 bits) and previous pixel.

uint8_t x11_buttons[SFG_KEY_COUNT];
unsigned long x11_palette[256];
unsigned int interlaceFrame = 0;
unsigned int drawLine = 0;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  uint_least16_t *word = x11_screen + y * SFG_SCREEN_RESOLUTION_X + x;
  *word = ((*word) & 0xff00) | colorIndex;
}

uint32_t SFG_getTimeMs(void)
{
  struct timeval now;
  gettimeofday(&now,NULL);
  return now.tv_sec * 1000 + now.tv_usec / 1000;
}

void SFG_save(uint8_t data[SFG_SAVE_SIZE])
{
  FILE *f = fopen(SFG_SAVE_FILE_PATH,"wb");

  puts("X11: opening and writing save file");

  if (f == NULL)
  {
    puts("X11: could not open the file!");
    return;
  }

  fwrite(data,1,SFG_SAVE_SIZE,f);

  fclose(f);
}

uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE])
{
  FILE *f = fopen(SFG_SAVE_FILE_PATH,"rb");

  puts("X11: opening and reading save file");

  if (f == NULL)
  {
    puts("X11: no save file to open");
  }
  else
  {
    fread(data,1,SFG_SAVE_SIZE,f);
    fclose(f);
  }

  return 1;
}

void SFG_sleepMs(uint16_t timeMs)
{
  usleep(timeMs * 1000);
}

void SFG_getMouseOffset(int16_t *x, int16_t *y)
{
}

void SFG_processEvent(uint8_t event, uint8_t data)
{
  printf("event: %d\n",event);
}

int8_t SFG_keyPressed(uint8_t key)
{
  return x11_buttons[key];
}

void SFG_setMusic(uint8_t value)
{
  printf("music: %d\n",value);
}

void SFG_playSound(uint8_t soundIndex, uint8_t volume)
{
  printf("sound: %d\n",soundIndex);
}

int main(int argc, char *argv[])
{
  puts(
    "Anarch v" SFG_VERSION_STRING "X11 frontend (no sound)\n"
    "by drummyfish, released under CC0 1.0, public domain");

  SFG_init();

  Display *display = XOpenDisplay(0);
  
  if (display == 0) 
  {
    puts("X11: could not open a display");
    return 0;
  }

  int screen = DefaultScreen(display);

  Window window = XCreateSimpleWindow(display,RootWindow(display,screen),10,10,
    SFG_SCREEN_RESOLUTION_X,SFG_SCREEN_RESOLUTION_Y,1,
    BlackPixel(display,screen),BlackPixel(display,screen));

  XMapWindow(display,window);

  XSizeHints *hints = XAllocSizeHints();
  hints->flags = PMinSize | PMaxSize;
  hints->min_width = SFG_SCREEN_RESOLUTION_X;
  hints->max_width = hints->min_width;
  hints->min_height = SFG_SCREEN_RESOLUTION_Y;
  hints->max_height = hints->min_height;
  XSetWMNormalHints(display,window,hints);
  XFree(hints);

  XSelectInput(display,window,KeyPressMask | KeyReleaseMask | ExposureMask);

  // create the palette:

  for (int i = 0; i < 256; ++i)
  {
    XColor color;

    color.red = ((paletteRGB565[i] & 0xf800) >> 8) * 256;
    color.green = ((paletteRGB565[i] & 0x07e0) >> 3) * 256;
    color.blue = ((paletteRGB565[i] & 0x00ff) << 3) * 256;
    color.flags = DoRed | DoGreen | DoBlue;

    XAllocColor(display,DefaultColormap(display,0),&color);

    x11_palette[i] = color.pixel;
  }

  GC context = DefaultGC(display,screen);

  int running = 1;

  /*
    rendering (and game update) loop follows:

    With xlib it's a little difficult to do realtime rendering efficiently, so
    the algorithm here is a bit sophisticated. Here is a summary:
   
    - We start drawing from global drawLine and draw a certain number of lines,
      the next frame we start drawing from drawLine again etc., so we're drawing
      by lines and rolling over to top after reaching bottom.
    - We don't redraw pixels of same color and we don't call XSetForeground if
      we'd be setting to same color.
    - Each frame we keep track of how many "pixel drawing operations" we've
      performed and stop rendering after certain limit, as drawing too many
      pixels slows the frame rendering (and hence the whole game) down.

    This makes most typical frames render quickly in one go (with only a few
    necessary updates of the previous frames) but frames that would take too
    much time (very different from previous frame) will be split to be drawn in
    several parts, resulting in kind of "vsync" like artifacts but retaining the
    game speed. If the game's still slow, fiddle with the hardcoded limit in the
    loop.
  */

  while (running)
  {
    uint_least16_t *pixel;

    running = SFG_mainLoopBody();

    uint_least16_t pixelNow = 0, pixelPrev = 1;
    XSetForeground(display,context,x11_palette[pixelNow]);

    int drawCounter = 0;

    for (int i = 0; i < SFG_SCREEN_RESOLUTION_Y; ++i)
    {
      pixel = x11_screen + SFG_SCREEN_RESOLUTION_X * drawLine;

      for (int x = 0; x < SFG_SCREEN_RESOLUTION_X; ++x)
      {
        uint_least16_t pixels = *pixel;

        if ((pixels >> 8) != (pixels & 0x00ff)) // don't redraw same color
        {
          pixelNow = pixels & 0x00ff;

          if (pixelNow != pixelPrev)
          {
            XSetForeground(display,context,x11_palette[pixelNow]);
            drawCounter++;
          }

          pixelPrev = pixelNow;
          drawCounter++;

          *pixel = ((*pixel) & 0x00ff) | ((*pixel) << 8);

          XDrawPoint(display,window,context,x,drawLine);
        }

        pixel++;
      }

      drawLine = (drawLine + 1) % SFG_SCREEN_RESOLUTION_Y;

      if (drawCounter > 40000) // if too many pixels drawn, stop
        break;
    }

    XEvent event;

    while (XCheckWindowEvent(display,window,KeyPressMask | KeyReleaseMask | ExposureMask  ,&event) != False)
    {
      if (event.type == Expose)
      {
        // exposed => make next frame render as a whole:
        for (long int i = 0; i < SFG_SCREEN_RESOLUTION_X * SFG_SCREEN_RESOLUTION_Y; ++i)
          x11_screen[i] = 1;
      }
      else
      {
        uint8_t state = event.xkey.type == KeyPress;

        switch (XKeycodeToKeysym(display,event.xkey.keycode,0))
        {
          case XK_Escape: running = 0; break;
          case XK_Up: // fallthrough
          case XK_KP_8:
          case XK_w: x11_buttons[SFG_KEY_UP] = state; break;
          case XK_Left: // fallthrough
          case XK_KP_7:
          case XK_q: x11_buttons[SFG_KEY_LEFT] = state; break;
          case XK_Right: // fallthrough
          case XK_KP_9:
          case XK_e: x11_buttons[SFG_KEY_RIGHT] = state; break;
          case XK_Down: // fallthrough
          case XK_KP_5:
          case XK_s: x11_buttons[SFG_KEY_DOWN] = state; break;
          case XK_y: // fallthrough
          case XK_z: // fallthrough
          case XK_j: // fallthrough
          case XK_Control_L:
          case XK_Control_R:
          case XK_Return: x11_buttons[SFG_KEY_A] = state; break;
          case XK_Alt_L:
          case XK_Alt_R:
          case XK_x: // fallthrough
          case XK_k: x11_buttons[SFG_KEY_B] = state; break;
          case XK_c: // fallthrough
          case XK_l: x11_buttons[SFG_KEY_C] = state; break;

          // extra keys:
          case XK_space: x11_buttons[SFG_KEY_JUMP] = state; break;
          case XK_Tab: x11_buttons[SFG_KEY_MAP] = state; break;
          case XK_a: // fallthrough 
          case XK_KP_4: x11_buttons[SFG_KEY_STRAFE_LEFT] = state; break;
          case XK_d: // fallthrough
          case XK_KP_6: x11_buttons[SFG_KEY_STRAFE_RIGHT] = state; break;
          case XK_v: x11_buttons[SFG_KEY_CYCLE_WEAPON] = state; break;
          case XK_r: // fallthrough
          case XK_F1: // fallthrough
          case XK_Page_Down: x11_buttons[SFG_KEY_PREVIOUS_WEAPON] = state; break;
          case XK_t: // fallthrough
          case XK_F2: // fallthrough
          case XK_Page_Up: x11_buttons[SFG_KEY_NEXT_WEAPON] = state; break;
          case XK_p: // fallthrough
          case XK_Home: x11_buttons[SFG_KEY_TOGGLE_FREELOOK] = state; break;

          default: break;
        }
      }
    }
  }

  puts("X11: ending");
  XCloseDisplay(display);

  return 0;
}
