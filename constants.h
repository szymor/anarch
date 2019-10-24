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
  Rate at which AI will be updated, which also affects how fast enemies will
  appear.
*/
#define SFG_AI_FPS 4

/**
  Says a probability (0 - 255) of the AI changing its state during one update
  step.
*/
#define SFG_AI_RANDOM_CHANGE_PROBABILITY 40

/**
  Speed of rocket projectile, in squares per second.
*/
#define SFG_ROCKET_SPEED 15

/**
  Distance at which level elements (sprites) collide, in RCL_Unit (1024 per
  square).
*/
#define SFG_ELEMENT_COLLISION_DISTANCE 800

/**
  Height, in RCL_Units, at which collisions happen with level elements
  (sprites).
*/
#define SFG_ELEMENT_COLLISION_HEIGHT 1024

/**
  Distance at which explosion does damage and throws away the player and
  monsters, in RCL_Units.
*/
#define SFG_EXPLOSION_DISTANCE 2048

/**
  How much damage explosion causes in its range.
*/
#define SFG_EXPLOSION_DAMAGE 18

/**
  Time, in ms, after which shotgun can be fired again.
*/
#define SFG_WEAPON_SHOTGUN_COOLDOWN 500

/**
  Maximum player health.
*/
#define SFG_PLAYER_MAX_HEALTH 100

/**
  At which value health indicator shows a warning (red color).
*/
#define SFG_PLAYER_HEALTH_WARNING_LEVEL 20

#endif // guard
