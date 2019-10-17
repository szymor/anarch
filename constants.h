/**
  @file constants.h

  This file contains definitions of game constants that are not considered
  part of game settings and whose change can ffect the game balance and
  playability, e.g. physics constants.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_CONSTANTS_H
#define _SFG_CONSTANTS_H

/**
  How quickly player turns left/right, in degrees per second.
*/
#define SFG_PLAYER_TURN_SPEED 210

/**
  How quickly player moves, in squares per second.
*/
#define SFG_PLAYER_MOVE_SPEED 10

/**
  Gravity acceleration in squares / (second^2).
*/

#define SFG_GRAVITY_ACCELERATION 30

/**
  Initial upwards speed of player's jump, in squares per second. 
*/

#define SFG_PLAYER_JUMP_SPEED 80

/**
  How quickly elevators and squeezers move, in RCL_Unit per second.
*/
#define SFG_MOVING_WALL_SPEED 1024

/**
  How quickly doors open and close, in RCL_Unit per second.
*/
#define SFG_DOOR_OPEN_SPEED 2048

/**
  Says the (Chebyshev) distance in game squares at which level elements
  (items, monsters etc.) become active.
*/
#define SFG_LEVEL_ELEMENT_ACTIVE_DISTANCE 8

/**
  Time in ms after which AI will be recomputing its state (which also affects
  how quickly death animation plays etc).
*/
#define SFG_AI_UPDATE_INTERVAL 500

#endif // guard
