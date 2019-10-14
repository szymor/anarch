/**
  @file globals.h

  This file contains global variables.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_GLOBALS_H
#define _SFG_GLOBALS_H

/**
  Pressed states of keys, LSB of each value means current pessed states, other
  bits store states in previous frames.
*/
uint8_t SFG_keyStates[SFG_KEY_COUNT]; 

uint8_t SFG_zBuffer[SFG_Z_BUFFER_SIZE];

int8_t SFG_backgroundScaleMap[SFG_GAME_RESOLUTION_Y];
uint16_t SFG_backgroundScroll;

#endif // guard
