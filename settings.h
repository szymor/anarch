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

#define SFG_FPS 60
#define SFG_RESOLUTION_X 1024
#define SFG_RESOLUTION_Y 768

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

#endif // guard
