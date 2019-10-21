/**
  @file settings.h

  This file contains settings (or setting hints) for the game. Values here can
  fine tune performance vs quality and personalize the final compiled game. Some
  of these settings may be overriden by the specific platform used according to
  its limitations.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_SETTINGS_H
#define _SFG_SETTINGS_H

/**
  Target FPS (frames per second). This sets the game logic FPS and will try to
  render at the same rate. If such fast rendering can't be achieved, frames will
  be droped, but the game logic will still be forced to run at this speed, so
  the game may actually become slowed down if FPS is set too high. Too high FPS
  can also negatively affect game speeds which are computed as integers and
  rounding errors can occur soon, so don't set this to extreme values.
*/
#define SFG_FPS 60

/**
  Width of the screen in pixels. Set this to ACTUAL resolution. If you want the
  game to run at smaller resolution (with bigger pixels), do his using
  SFG_RESOLUTION_SCALEDOWN;  
*/
#define SFG_SCREEN_RESOLUTION_X 1024

/**
  Height of the screen in pixels. Set this to ACTUAL resolution. If you want the
  game to run at smaller resolution (with bigger pixels), do his using
  SFG_RESOLUTION_SCALEDOWN;  
*/
#define SFG_SCREEN_RESOLUTION_Y 768

/**
  How many times the screen resolution will be divided (how many times a game
  pixel will be bigger than the screen pixel).
*/
#define SFG_RESOLUTION_SCALEDOWN 1

/**
  Turn on for previes mode for map editing (flying, noclip, fast movement etc.).
*/
#define SFG_PREVIEW_MODE 0

/**
  How much faster movement is in the preview mode.
*/
#define SFG_PREVIEW_MODE_SPEED_MULTIPLIER 2

/**
  Hint as to whether run in fullscreen, if the platform allows it.
*/
#define SFG_FULLSCREEN 0

/**
  Whether shadows (fog) should be dithered, i.e. more smooth (needs a bit more
  CPU performance and memory).
*/
#define SFG_DITHERED_SHADOW 1

/**
  Maximum number of squares that will be traversed by any cast ray. Smaller
  number is faster but can cause visual artifacts.
*/
#define SFG_RAYCASTING_MAX_STEPS 30

/**
  Maximum number of hits any cast ray will register. Smaller number is faster
  but can cause visual artifacts.
*/
#define SFG_RAYCASTING_MAX_HITS 10

/**
  How many times rendering should be subsampled horizontally. Bigger number
  can significantly improve performance (by casting fewer rays), but can look
  a little worse.
*/
#define SFG_RAYCASTING_SUBSAMPLE 1

/**
  Enables or disables fog (darkness) due to distance. Recommended to keep on
  for good look, but can be turned off for performance.
*/
#define SFG_ENABLE_FOG 1

/**
  Says whether sprites should diminish in fog. This takes more performance but
  looks better.
*/
#define SFG_DIMINISH_SPRITES 1

/**
  How quick player head bob is, 1024 meaning once per second. 0 Means turn off
  head bob.
*/
#define SFG_HEADBOB_SPEED 900

/**
  Sets head bob offset, in RCL_UNITS_PER_SQUARE. 0 Means turn off head bob.
*/
#define SFG_HEADBOB_OFFSET 200

/**
  Weapon bobbing offset in weapon image pixels.
*/
#define SFG_WEAPONBOB_OFFSET 4 

/**
  Camera shearing (looking up/down) speed, in vertical resolutions per second.
*/
#define SFG_CAMERA_SHEAR_SPEED 3

/**
  Maximum camera shear (vertical angle). 1024 means 1.0 * vertical resolution.
*/
#define SFG_CAMERA_MAX_SHEAR 1024

/**
  Duration in ms of the explosion animation.
*/
#define SFG_EXPLOSION_DURATION 70

#endif // guard
