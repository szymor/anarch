/**
  @file smallinput.h

  Small API for getting keyboard/mouse input, with possiblity to record it and
  play back.

  The Linux Input API requires root pirivileges (sudo).

  by Milsolav "drummyfish" Ciz, released under CC0 1.0 (public domain)
*/

#ifndef _SMALLINPUT_H
#define _SMALLINPUT_H

#include <stdint.h>

#define SMALLINPUT_KEY_NONE    0
#define SMALLINPUT_ARROW_UP    128
#define SMALLINPUT_ARROW_RIGHT 129
#define SMALLINPUT_ARROW_DOWN  130
#define SMALLINPUT_ARROW_LEFT  131
#define SMALLINPUT_SPACE       ' '
#define SMALLINPUT_BACKSPACE   8
#define SMALLINPUT_TAB         9
#define SMALLINPUT_RETURN      13
#define SMALLINPUT_SHIFT       14
#define SMALLINPUT_ESCAPE      27
#define SMALLINPUT_DELETE      127
#define SMALLINPUT_MOUSE_L     253
#define SMALLINPUT_MOUSE_M     254
#define SMALLINPUT_MOUSE_R     255

uint8_t input_keyStates[256];
int32_t input_mousePosition[2];
uint32_t input_frame = 0;

#if 1 // TODO: add other options for input handling (SDL, xinput, ...)
  /*
    This is using Linux Input Subsystem API. Defines can be found in
    include/uapi/linux/input-event-codes.h.
  */

  #include <fcntl.h>
  #include <linux/input.h>
  #include <sys/time.h>
  #include <unistd.h>

  typedef struct
  {
    struct timeval time;
    uint16_t type;
    uint16_t code;
    int32_t value;
  } LinuxInputEvent;

  #define INPUT_KEYBOARD_FILE "/dev/input/event0"
  #define INPUT_MOUSE_FILE "/dev/input/event1"

  int input_keyboardFile = 0;
  int input_mouseFile = 0;

  /**
    Maps this library's key codes to linux input key codes.
  */
  static const int input_linuxCodes[256] =
    {
      #define no KEY_RESERVED
      no,no,no,no,no,no,no,no,KEY_BACKSPACE,KEY_TAB,no,no,no,KEY_ENTER,KEY_LEFTSHIFT,no,
      no,no,no,no,no,no,no,no,no,no,no,KEY_ESC,no,no,no,no,
      KEY_SPACE,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,KEY_O,
      KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,no,no,no,no,no,
      no,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,KEY_O,
      KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,no,no,no,no,KEY_DELETE,
      KEY_UP,KEY_RIGHT,KEY_DOWN,KEY_LEFT,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,
      no,no,no,no,no,no,no,no,no,no,no,no,no,no,no,no
      #undef no
    };
#endif

/**
  Initializes the library.
*/
void input_init(void)
{
  input_mousePosition[0] = 0;
  input_mousePosition[1] = 0;
  input_frame = 0;

  for (int16_t i = 0; i < 256; ++i)
    input_keyStates[i] = 0;

  input_keyboardFile = open(INPUT_KEYBOARD_FILE, O_RDONLY);
  fcntl(input_keyboardFile, F_SETFL, O_NONBLOCK); 

  input_mouseFile = open(INPUT_MOUSE_FILE, O_RDONLY);
  fcntl(input_mouseFile, F_SETFL, O_NONBLOCK); 
}

void input_end(void)
{
  close(input_keyboardFile);
  close(input_mouseFile);
}

/**
  Should be called once every main loop iteration to retrieve current input
  state.
*/
void input_update(void)
{
  LinuxInputEvent event;

  while (1) // keyboard
  {
    if (read(input_keyboardFile, &event, sizeof(event)) <= 0)
      break;

    if (event.type == EV_KEY && (event.value == 1 || event.value == 0))
      for (uint16_t i = 0; i < 256; ++i)
        if (event.code == input_linuxCodes[i])
        {
          input_keyStates[i] = event.value;
          break;
        }
  }

  while (1) // mouse
  {
    if (read(input_mouseFile, &event, sizeof(event)) <= 0)
      break;

    if (event.type == EV_REL)
      input_mousePosition[event.code % 2] += event.value;
    else if (event.type == EV_KEY)
    {
      input_keyStates[  
        event.code == BTN_LEFT ? SMALLINPUT_MOUSE_L :
        (event.code == BTN_RIGHT ? SMALLINPUT_MOUSE_R : SMALLINPUT_MOUSE_M)]
        = event.value; 
    }
  }

  for (uint16_t i = 0; i < 256; ++i)
    if (input_keyStates[i] && input_keyStates[i] < 255)
      input_keyStates[i]++;

  input_frame++;
}

/**
  Returns the number of input frames for which given key has been pressed (> 1:
  key is pressed, == 1: key was just pressed, == 0: key is not pressed).
*/
static inline uint8_t input_getKey(uint8_t key)
{
  if (key >= 'a' && key <= 'z')
    key = 'A' + (key - 'a');

  return input_keyStates[key];
}

/**
  Gets the mouse position.
*/
static inline void input_getMousePos(int16_t *x, int16_t *y)
{
  *x = input_mousePosition[0];
  *y = input_mousePosition[1];
}

static inline void input_setMousePos(int32_t x, int32_t y)
{
  input_mousePosition[0] = x;
  input_mousePosition[1] = y;
}

#if 0 // this can be used to test this
mates

#include <stdio.h>

int main(void)
{
  input_init();

  while (1)
  {
    input_update();

    puts("\n\n\n\n");

    for (uint16_t i = 0; i < 256; ++i)
      printf("%d (%c): %d, ",i,i,input_keyStates[i]);   

    printf("mouse: %d %d (%d %d %d)\n",input_mousePosition[0],input_mousePosition[1],input_getKey(SMALLINPUT_MOUSE_L),input_getKey(SMALLINPUT_MOUSE_M),input_getKey(SMALLINPUT_MOUSE_R));

    puts("");

    usleep(10000);
  }
 
  input_end();
}

#endif

#endif
