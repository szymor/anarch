#include <stdint.h>
#include "constants.h"
#include "levels.h"
#include "assets.h"
#include "palette.h"

#define SFG_KEY_UP 0
#define SFG_KEY_RIGHT 1
#define SFG_KEY_DOWN 2
#define SFG_KEY_LEFT 3
#define SFG_KEY_A 4
#define SFG_KEY_B 5
#define SFG_KEY_C 6

/* ============================= PORTING =================================== */

/* When porting, do the following:
   - implement the following functions in your platform_*.h.
   - Call SFG_init() from your platform initialization code.
   - Call SFG_mainLoopBody() from within your platform's main loop.
   - include "settings.h" in your platform_*.h and optionally hard-override
     (redefine) some settings in platform_*.h, according to the platform's
     needs.
*/

#define SFG_LOG(str) ; ///< Can be redefined to log messages for better debug.

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

#ifdef SFG_PLATFORM_POKITTO
  #include "platform_pokitto.h"
#else
  #include "platform_sdl.h"
#endif

#define SFG_MS_PER_FRAME (1000 / SFG_FPS) // ms per frame with target FPS

#define SFG_PLAYER_TURN_UNITS_PER_FRAME\
  ((SFG_PLAYER_TURN_SPEED * RCL_UNITS_PER_SQUARE) / (360 * SFG_FPS))

#define SFG_PLAYER_MOVE_UNITS_PER_FRAME\
  ((SFG_PLAYER_MOVE_SPEED * RCL_UNITS_PER_SQUARE) / SFG_FPS)

#define RCL_PIXEL_FUNCTION SFG_pixelFunc
#define RCL_TEXTURE_VERTICAL_STRETCH 0

#include "raycastlib.h" 

/**
  Step in which walls get higher, in raycastlib units.
*/
#define SFG_WALL_HEIGHT_STEP (RCL_UNITS_PER_SQUARE / 4)

#define SFG_CEILING_MAX_HEIGHT\
  (16 * RCL_UNITS_PER_SQUARE - RCL_UNITS_PER_SQUARE / 2 )

RCL_Camera SFG_camera;
RCL_RayConstraints SFG_rayConstraints;

uint8_t SFG_backgroundScaleMap[SFG_RESOLUTION_Y];
uint16_t SFG_backgroundScroll;

/**
  Stores the current level and helper precomputed vaues for better performance.
*/
struct
{
  const SFG_Level *levelPointer;
  const uint8_t* textures[7];
  uint32_t timeStart;
  uint8_t floorColor;
  uint8_t ceilingColor;
} SFG_currentLevel;

void SFG_pixelFunc(RCL_PixelInfo *pixel)
{
  uint8_t color;
  uint8_t shadow = 0;

  if (pixel->isHorizon && pixel->depth > RCL_UNITS_PER_SQUARE * 16)
  {
    color = SFG_TRANSPARENT_COLOR;
  }
  else if (pixel->isWall)
  {
    uint8_t textureIndex =
      pixel->isFloor ?
      (
        ((pixel->hit.type & SFG_TILE_PROPERTY_MASK) != SFG_TILE_PROPERTY_DOOR) ?
        (pixel->hit.type & 0x7)
        :
        (
          (pixel->texCoords.y > RCL_UNITS_PER_SQUARE) ?
          (pixel->hit.type & 0x7) : 255
        )
      ):
      ((pixel->hit.type & 0x38) >> 3); 

    RCL_Unit textureV = pixel->texCoords.y;

    if ((pixel->hit.type & SFG_TILE_PROPERTY_MASK) ==
      SFG_TILE_PROPERTY_SQUEEZER)
      textureV += pixel->wallHeight;

    color =
      textureIndex != SFG_TILE_TEXTURE_TRANSPARENT ?
      (SFG_getTexel(
        textureIndex != 255 ?
          SFG_currentLevel.textures[textureIndex]:
          SFG_texturesWall[SFG_currentLevel.levelPointer->doorTextureIndex],
        pixel->texCoords.x / 32,
        textureV / 32)
      ) :
      SFG_TRANSPARENT_COLOR;

    shadow = pixel->hit.direction >> 1;

  }
  else
  {
    color = pixel->isFloor ?
      (SFG_currentLevel.floorColor) : 
      (pixel->height < SFG_CEILING_MAX_HEIGHT ?
         SFG_currentLevel.ceilingColor : SFG_TRANSPARENT_COLOR);
  }

  if (color != SFG_TRANSPARENT_COLOR)
  {
#if SFG_DITHERED_SHADOW
    uint8_t fogShadow = (pixel->depth) / RCL_UNITS_PER_SQUARE;

    uint8_t fogShadowPart = fogShadow & 0x03;
    uint8_t xMod2 = pixel->position.x & 0x01;
    uint8_t yMod2 = pixel->position.y & 0x01;

    fogShadow >>= 2;

    if (((fogShadowPart == 1) && xMod2 && yMod2) ||
        ((fogShadowPart == 2) && (xMod2 == yMod2)) ||
        ((fogShadowPart == 3) && (xMod2 || yMod2)))
      fogShadow += 1;

    shadow += fogShadow;
#else
    shadow += pixel->depth / (RCL_UNITS_PER_SQUARE * 2);
#endif

    color = palette_minusValue(color,shadow);
  }
  else
  {
    color = SFG_getTexel(SFG_backgrounds[0],
    SFG_backgroundScaleMap[(pixel->position.x * SFG_RAYCASTING_SUBSAMPLE + SFG_backgroundScroll) % SFG_RESOLUTION_Y],
                                                                   // ^ TODO: get rid of mod?
    SFG_backgroundScaleMap[pixel->position.y]);
  }

  RCL_Unit screenX = pixel->position.x * SFG_RAYCASTING_SUBSAMPLE;

  for (uint8_t i = 0; i < SFG_RAYCASTING_SUBSAMPLE; ++i)
  {
    SFG_setPixel(screenX,pixel->position.y,color);
    screenX++;
  }
}

RCL_Unit SFG_texturesAt(int16_t x, int16_t y)
{
  uint8_t p;

  SFG_TileDefinition tile = SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&p);
  return
    SFG_TILE_FLOOR_TEXTURE(tile) | (SFG_TILE_CEILING_TEXTURE(tile) << 3) | p;
    // ^ store both textures (floor and ceiling) and properties in one number
}

RCL_Unit SFG_movingWallHeight
(
  RCL_Unit low,
  RCL_Unit high,
  uint32_t time    
)
{
  RCL_Unit height = high - low;
  RCL_Unit halfHeight = height / 2;

  RCL_Unit sinArg =
    (time * ((SFG_MOVING_WALL_SPEED * RCL_UNITS_PER_SQUARE) / 1000)) / height;

  return
    low + halfHeight + (RCL_sinInt(sinArg) * halfHeight) / RCL_UNITS_PER_SQUARE;
}

RCL_Unit SFG_floorHeightAt(int16_t x, int16_t y)
{
  uint8_t properties;

  SFG_TileDefinition tile = SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&properties);

  return properties != SFG_TILE_PROPERTY_ELEVATOR ?
    SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP :
    SFG_movingWallHeight(
      SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_TILE_CEILING_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_getTimeMs() - SFG_currentLevel.timeStart);
}

RCL_Unit SFG_ceilingHeightAt(int16_t x, int16_t y)
{
  uint8_t properties;
  SFG_TileDefinition tile = SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&properties);

  if (properties == SFG_TILE_PROPERTY_ELEVATOR)
    return SFG_CEILING_MAX_HEIGHT;

  uint8_t height = SFG_TILE_CEILING_HEIGHT(tile);

  return properties != SFG_TILE_PROPERTY_SQUEEZER ?
    (
      height != SFG_TILE_CEILING_MAX_HEIGHT ?
      ((SFG_TILE_FLOOR_HEIGHT(tile) + height) * SFG_WALL_HEIGHT_STEP) :
      SFG_CEILING_MAX_HEIGHT
    ) :
    SFG_movingWallHeight(
      SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_TILE_CEILING_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_getTimeMs() - SFG_currentLevel.timeStart);
}

uint32_t SFG_frame;
uint32_t SFG_lastFrameTimeMs;

RCL_Vector2D SFG_playerDirection;

void SFG_recompurePLayerDirection()
{
  SFG_camera.direction = RCL_wrap(SFG_camera.direction,RCL_UNITS_PER_SQUARE);

  SFG_playerDirection = RCL_angleToDirection(SFG_camera.direction);

  SFG_playerDirection.x = (SFG_playerDirection.x * SFG_PLAYER_MOVE_UNITS_PER_FRAME) / RCL_UNITS_PER_SQUARE;
  SFG_playerDirection.y = (SFG_playerDirection.y * SFG_PLAYER_MOVE_UNITS_PER_FRAME) / RCL_UNITS_PER_SQUARE;

  SFG_backgroundScroll =
    ((SFG_camera.direction * 8) * SFG_RESOLUTION_Y) / RCL_UNITS_PER_SQUARE; 
}

void SFG_setLevel(const SFG_Level *level)
{
  SFG_LOG("setting and initializing level");

  SFG_currentLevel.levelPointer = level;

  SFG_currentLevel.floorColor = level->floorColor;
  SFG_currentLevel.ceilingColor = level->ceilingColor;

  for (uint8_t i = 0; i < 7; ++i)
    SFG_currentLevel.textures[i] =
      SFG_texturesWall[level->textureIndices[i]];

  SFG_currentLevel.timeStart = SFG_getTimeMs(); 
 
  SFG_recompurePLayerDirection();  
}

void SFG_init()
{
  SFG_LOG("initializing game")

  SFG_frame = 0;
  SFG_lastFrameTimeMs = 0;

  RCL_initCamera(&SFG_camera);
  RCL_initRayConstraints(&SFG_rayConstraints);

  SFG_camera.resolution.x = SFG_RESOLUTION_X / SFG_RAYCASTING_SUBSAMPLE;
  SFG_camera.resolution.y = SFG_RESOLUTION_Y;
  SFG_camera.height = RCL_UNITS_PER_SQUARE;
  SFG_camera.position.x = RCL_UNITS_PER_SQUARE * 5;
  SFG_camera.position.y = RCL_UNITS_PER_SQUARE * 5;

  SFG_rayConstraints.maxHits = SFG_RAYCASTING_MAX_HITS;
  SFG_rayConstraints.maxSteps = SFG_RAYCASTING_MAX_STEPS;

  for (uint16_t i = 0; i < SFG_RESOLUTION_Y; ++i)
    SFG_backgroundScaleMap[i] = (i * SFG_TEXTURE_SIZE) / SFG_RESOLUTION_Y;

  SFG_backgroundScroll = 0;

  SFG_setLevel(&SFG_level0);
}

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
    SFG_recompurePLayerDirection();

  if (SFG_keyPressed(SFG_KEY_UP))
  {
    SFG_camera.position.x += SFG_playerDirection.x    * 5;
    SFG_camera.position.y += SFG_playerDirection.y    * 5;
  }
  else if (SFG_keyPressed(SFG_KEY_DOWN))
  {
    SFG_camera.position.x -= SFG_playerDirection.x;
    SFG_camera.position.y -= SFG_playerDirection.y;
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
    RCL_renderComplex(SFG_camera,SFG_floorHeightAt,SFG_ceilingHeightAt,SFG_texturesAt,SFG_rayConstraints);

    SFG_lastFrameTimeMs = timeNow;
  }

  uint32_t timeNextFrame = timeNow + SFG_MS_PER_FRAME;
  timeNow = SFG_getTimeMs();

  if (timeNextFrame > timeNow)
    SFG_sleepMs((timeNextFrame - timeNow) / 2); // wait, relieve CPU
  else
    SFG_LOG("failed to reach target FPS!")
}
