#include <stdint.h>
#include "constants.h"
#include "levels.h"
#include "assets.h"

#define SFG_KEY_UP 0
#define SFG_KEY_RIGHT 1
#define SFG_KEY_DOWN 2
#define SFG_KEY_LEFT 3
#define SFG_KEY_A 4
#define SFG_KEY_B 5
#define SFG_KEY_C 6

/* ============================= PORTING =================================== */

/* When porting, define the following in your specific platform_*.h. Also you
   have to call SFG_mainLoopBody() in the platform's main loop and SFG_init()
   in the platform initialization. */

// SFG_RESOLUTION_X    #define this to screen width in pixels
// SFG_RESOLUTION_Y    #define this to screen height in pixels
// SFG_FPS             #define this to desired FPS

/** Return 1 (0) if given key is pressed (not pressed). */
int8_t SFG_keyPressed(uint8_t key);

/** Return time in ms sice program start. */
uint32_t SFG_getTimeMs();

/** Sleep (yield CPU) for specified amount of ms. This is used to relieve CPU
  usage. If your platform doesn't need this or handles it in other way, this
  function can do nothing. */
void SFG_sleepMs(uint16_t timeMs);

/** Set specified screen pixel. The function doesn't have to check whether
  the coordinates are within screen. */
static inline void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex);

/* ========================================================================= */

/**
  Game main loop body, call this inside the platform's specific main loop.
*/
void SFG_mainLoopBody();

/**
  Initializes the whole program, call this in the platform initialization.
*/
void SFG_init();

#include "platform_sdl.h"

#define SFG_MS_PER_FRAME (1000 / SFG_FPS) // ms per frame with target FPS

#define RCL_PIXEL_FUNCTION SFG_pixelFunc
#define RCL_TEXTURE_VERTICAL_STRETCH 0

#include "raycastlib.h" 

RCL_Camera SFG_camera;
RCL_RayConstraints SFG_rayConstraints;

uint8_t SFG_backgroundScaleMap[SFG_RESOLUTION_Y];
uint16_t SFG_backgroundScroll;

void SFG_pixelFunc(RCL_PixelInfo *pixel)
{
  uint8_t color;
  uint8_t shadow = 0;

  if (pixel->isWall)
  {
    color =
      pixel->hit.type != SFG_TILE_TEXTURE_TRANSPARENT ?
      (SFG_getTexel(SFG_texturesWall[pixel->hit.type],pixel->texCoords.x / 32,pixel->texCoords.y / 32)) :
      SFG_TRANSPARENT_COLOR;

    shadow = pixel->hit.direction >> 1;
  }
  else
  {
    color = pixel->isFloor ? 20 : 50;
  }

  if (color != SFG_TRANSPARENT_COLOR)
  {
    shadow += pixel->depth / (RCL_UNITS_PER_SQUARE * 2);
    color = palette_minusValue(color,shadow);
  }
  else
  {
    color = SFG_getTexel(SFG_backgrounds[0],
    SFG_backgroundScaleMap[(pixel->position.x + SFG_backgroundScroll) % SFG_RESOLUTION_Y],
    SFG_backgroundScaleMap[pixel->position.y]);
  }

  SFG_setPixel(pixel->position.x,pixel->position.y,color); 
}

RCL_Unit SFG_textureAt(int16_t x, int16_t y)
{
  SFG_TileDefinition tile = SFG_getMapTile(&(SFG_level0.map),x,y);
  return SFG_TILE_FLOOR_TEXTURE(tile);
}

RCL_Unit SFG_floorHeightAt(int16_t x, int16_t y)
{
  SFG_TileDefinition tile = SFG_getMapTile(&(SFG_level0.map),x,y);

  return SFG_TILE_FLOOR_HEIGHT(tile) * (RCL_UNITS_PER_SQUARE / 4);
}

RCL_Unit SFG_ceilingHeightAt(int16_t x, int16_t y)
{
  SFG_TileDefinition tile = SFG_getMapTile(&(SFG_level0.map),x,y);

  uint8_t height = SFG_TILE_CEILING_HEIGHT(tile);

  return height != SFG_TILE_CEILING_MAX_HEIGHT ?
    ((SFG_TILE_FLOOR_HEIGHT(tile) + height) * (RCL_UNITS_PER_SQUARE / 4)) :
    (RCL_UNITS_PER_SQUARE * 32);
}

uint32_t SFG_frame;
uint32_t SFG_lastFrameTimeMs;

void SFG_init()
{
  SFG_frame = 0;
  SFG_lastFrameTimeMs = 0;

  RCL_initCamera(&SFG_camera);
  RCL_initRayConstraints(&SFG_rayConstraints);

  SFG_camera.resolution.x = SFG_RESOLUTION_X;
  SFG_camera.resolution.y = SFG_RESOLUTION_Y;
  SFG_camera.height = RCL_UNITS_PER_SQUARE;
  SFG_camera.position.x = RCL_UNITS_PER_SQUARE * 5;
  SFG_camera.position.y = RCL_UNITS_PER_SQUARE * 5;

  SFG_rayConstraints.maxHits = 10;
  SFG_rayConstraints.maxSteps = 32;

  for (uint16_t i = 0; i < SFG_RESOLUTION_Y; ++i)
    SFG_backgroundScaleMap[i] = (i * SFG_TEXTURE_SIZE) / SFG_RESOLUTION_Y;

  SFG_backgroundScroll = 0;
}

#define SFG_PLAYER_TURN_UNITS_PER_FRAME\
  ((SFG_PLAYER_TURN_SPEED * RCL_UNITS_PER_SQUARE) / (360 * SFG_FPS))

#define SFG_PLAYER_MOVE_UNITS_PER_FRAME\
  ((SFG_PLAYER_MOVE_SPEED * RCL_UNITS_PER_SQUARE) / SFG_FPS)

RCL_Vector2D playerDirection;

/**
  Performs one game step (logic, physics), happening SFG_MS_PER_FRAME after
  previous frame. 
*/
void SFG_gameStep()
{
  int8_t recomputeDirection = 0;

  if (SFG_keyPressed(SFG_KEY_LEFT))
  {
    SFG_camera.direction -= SFG_PLAYER_TURN_UNITS_PER_FRAME;
    recomputeDirection = 1;
  }
  else if (SFG_keyPressed(SFG_KEY_RIGHT))
  {
    SFG_camera.direction += SFG_PLAYER_TURN_UNITS_PER_FRAME;
    recomputeDirection = 1;
  }

  if (recomputeDirection)
  {
    SFG_camera.direction = RCL_wrap(SFG_camera.direction,RCL_UNITS_PER_SQUARE);

    playerDirection = RCL_angleToDirection(SFG_camera.direction);

    playerDirection.x = (playerDirection.x * SFG_PLAYER_MOVE_UNITS_PER_FRAME) / RCL_UNITS_PER_SQUARE;
    playerDirection.y = (playerDirection.y * SFG_PLAYER_MOVE_UNITS_PER_FRAME) / RCL_UNITS_PER_SQUARE;

    SFG_backgroundScroll =
      ((SFG_camera.direction * 8) * SFG_RESOLUTION_Y) / RCL_UNITS_PER_SQUARE;
  }

  if (SFG_keyPressed(SFG_KEY_UP))
  {
    SFG_camera.position.x += playerDirection.x;
    SFG_camera.position.y += playerDirection.y;
  }
  else if (SFG_keyPressed(SFG_KEY_DOWN))
  {
    SFG_camera.position.x -= playerDirection.x;
    SFG_camera.position.y -= playerDirection.y;
  }

  if (SFG_keyPressed(SFG_KEY_A))
    SFG_camera.height += SFG_PLAYER_MOVE_UNITS_PER_FRAME;
  else if (SFG_keyPressed(SFG_KEY_B))
    SFG_camera.height -= SFG_PLAYER_MOVE_UNITS_PER_FRAME;
}

void SFG_mainLoopBody()
{
  /* standard deterministic game loop, independed on actuall achieved FPS,
     each game logic (physics) frame is performed with the SFG_MS_PER_FRAME
     delta time. */

  uint32_t timeNow = SFG_getTimeMs();
  uint16_t timeSinceLastFrame = timeNow - SFG_lastFrameTimeMs;

  if (timeSinceLastFrame >= SFG_MS_PER_FRAME)
  {
    // perform game logic (physics), for each frame
    while (timeSinceLastFrame >= SFG_MS_PER_FRAME)
    {
      SFG_gameStep();

      timeSinceLastFrame -= SFG_MS_PER_FRAME;

      SFG_frame++;
    }

    // render noly once
    RCL_renderComplex(SFG_camera,SFG_floorHeightAt,SFG_ceilingHeightAt,SFG_textureAt,SFG_rayConstraints);

    SFG_lastFrameTimeMs = timeNow;
  }

  uint32_t timeNextFrame = timeNow + SFG_MS_PER_FRAME;
  timeNow = SFG_getTimeMs();

  if (timeNextFrame > timeNow)
    SFG_sleepMs((timeNextFrame - timeNow) / 2); // wait, relieve CPU
}
