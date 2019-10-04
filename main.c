/**
  @file main.c
 
  Main source file of the game that puts together all the pieces. main game
  logic is implemented here.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#include <stdint.h>

/*
  The following keys are mandatory to be implemented on any platform in order
  for the game to be playable.
*/
#define SFG_KEY_UP 0
#define SFG_KEY_RIGHT 1
#define SFG_KEY_DOWN 2
#define SFG_KEY_LEFT 3
#define SFG_KEY_A 4
#define SFG_KEY_B 5
#define SFG_KEY_C 6
/*
  The following keys are optional for a platform to implement. They just make
  the controls more comfortable.
*/
#define SFG_KEY_JUMP 7
#define SFG_KEY_STRAFE_LEFT 8
#define SFG_KEY_STRAFE_RIGHT 9

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

#define SFG_PROGRAM_MEMORY static const /**< Can be redefined to platform's
                                         specifier of program meory. */

/** Return 1 (0) if given key is pressed (not pressed). At least the mandatory
  keys have to be implemented, the optional keys don't have to ever return 1.
  See the key contant definitions to see which ones are mandatory. */
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

#include "constants.h"
#include "levels.h"
#include "assets.h"
#include "palette.h"

#define SFG_GAME_RESOLUTION_X \
  (SFG_SCREEN_RESOLUTION_X / SFG_RESOLUTION_SCALEDOWN)

#define SFG_GAME_RESOLUTION_Y \
  (SFG_SCREEN_RESOLUTION_Y / SFG_RESOLUTION_SCALEDOWN)

#define SFG_MS_PER_FRAME (1000 / SFG_FPS) // ms per frame with target FPS

#define SFG_PLAYER_TURN_UNITS_PER_FRAME\
  ((SFG_PLAYER_TURN_SPEED * RCL_UNITS_PER_SQUARE) / (360 * SFG_FPS))
// TODO: ^ if zero, set to 1?

#define SFG_PLAYER_MOVE_UNITS_PER_FRAME\
  ((SFG_PLAYER_MOVE_SPEED * RCL_UNITS_PER_SQUARE) / SFG_FPS)

#define SFG_GRAVITY_SPEED_INCREASE_PER_FRAME\
  ((SFG_GRAVITY_ACCELERATION * RCL_UNITS_PER_SQUARE) / (SFG_FPS * SFG_FPS))

#define RCL_PIXEL_FUNCTION SFG_pixelFunc
#define RCL_TEXTURE_VERTICAL_STRETCH 0

#define RCL_CAMERA_COLL_HEIGHT_BELOW 800
#define RCL_CAMERA_COLL_HEIGHT_ABOVE 100

#include "raycastlib.h" 

/**
  Step in which walls get higher, in raycastlib units.
*/
#define SFG_WALL_HEIGHT_STEP (RCL_UNITS_PER_SQUARE / 4)

#define SFG_CEILING_MAX_HEIGHT\
  (16 * RCL_UNITS_PER_SQUARE - RCL_UNITS_PER_SQUARE / 2 )

int8_t SFG_backgroundScaleMap[SFG_GAME_RESOLUTION_Y];
uint16_t SFG_backgroundScroll;

struct
{
  RCL_Camera camera;
  int8_t squarePosition[2];
  RCL_Vector2D direction;
  RCL_Unit verticalSpeed;
  RCL_Unit previousVerticalSpeed; /**< Vertical speed in previous frame, needed
                                  for determining whether player is in the
                                  air. */
} SFG_player;


#if SFG_RESOLUTION_SCALEDOWN == 1
  #define SFG_setGamePixel SFG_setPixel
#else
/**
  Sets the game pixel (a pixel that can potentially be bigger than the screen
  pixel).
*/
static inline void SFG_setGamePixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  uint16_t screenY = y * SFG_RESOLUTION_SCALEDOWN;
  uint16_t screenX = x * SFG_RESOLUTION_SCALEDOWN;

  for (uint16_t j = screenY; j < screenY + SFG_RESOLUTION_SCALEDOWN; ++j)
    for (uint16_t i = screenX; i < screenX + SFG_RESOLUTION_SCALEDOWN; ++i)
      SFG_setPixel(i,j,colorIndex);
}
#endif

void SFG_recompurePLayerDirection()
{
  SFG_player.camera.direction = RCL_wrap(SFG_player.camera.direction,RCL_UNITS_PER_SQUARE);

  SFG_player.direction = RCL_angleToDirection(SFG_player.camera.direction);

  SFG_player.direction.x = (SFG_player.direction.x * SFG_PLAYER_MOVE_UNITS_PER_FRAME) / RCL_UNITS_PER_SQUARE;
  SFG_player.direction.y = (SFG_player.direction.y * SFG_PLAYER_MOVE_UNITS_PER_FRAME) / RCL_UNITS_PER_SQUARE;

  SFG_backgroundScroll =
    ((SFG_player.camera.direction * 8) * SFG_GAME_RESOLUTION_Y) / RCL_UNITS_PER_SQUARE; 
}

void SFG_initPlayer()
{
  RCL_initCamera(&SFG_player.camera);

  SFG_player.camera.resolution.x = SFG_GAME_RESOLUTION_X / SFG_RAYCASTING_SUBSAMPLE;
  SFG_player.camera.resolution.y = SFG_GAME_RESOLUTION_Y;
  SFG_player.camera.height = RCL_UNITS_PER_SQUARE * 12;
  SFG_player.camera.position.x = RCL_UNITS_PER_SQUARE * 15;
  SFG_player.camera.position.y = RCL_UNITS_PER_SQUARE * 8;

  SFG_recompurePLayerDirection();
  SFG_player.verticalSpeed = 0;
  SFG_player.previousVerticalSpeed = 0;
}

RCL_RayConstraints SFG_rayConstraints;

typedef struct
{
  uint8_t coords[2];
  uint8_t state;     /**< door state in format:
                          
                          MSB  ccbaaaaa  LSB

                          aaaaa: current door height (how much they're open)
                          b:     whether currently going up (0) or down (1)
                          cc:    by which keys the door is unlocked
                     */
} SFG_DoorRecord;

#define SFG_DOOR_DEFAULT_STATE 0x1f

#define SFG_MAX_DOORS 32

/**
  Stores the current level and helper precomputed vaues for better performance.
*/
struct
{
  const SFG_Level *levelPointer;
  const uint8_t* textures[7];
  uint32_t timeStart;
  uint32_t frameStart;
  uint8_t floorColor;
  uint8_t ceilingColor;
  SFG_DoorRecord doors[SFG_MAX_DOORS];
  uint8_t doorRecordCount;
  uint8_t checkedDoorIndex; ///< Says which door are currently being checked.
} SFG_currentLevel;

#if SFG_DITHERED_SHADOW
SFG_PROGRAM_MEMORY uint8_t SFG_ditheringPatterns[] =
{
  0,0,0,0,
  0,0,0,0,

  0,0,0,0,
  0,1,0,0,

  0,0,0,0,
  0,1,0,1,

  1,0,1,0,
  0,1,0,0,

  1,0,1,0,
  0,1,0,1,

  1,0,1,0,
  0,1,1,1,

  1,1,1,1,
  0,1,0,1,

  1,1,1,1,
  0,1,1,1,
 
  1,1,1,1,
  1,1,1,1
};
#endif

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
    uint8_t fogShadow = (pixel->depth * 4) / (RCL_UNITS_PER_SQUARE);
    
    uint8_t fogShadowPart = fogShadow & 0x07;

    fogShadow /= 8;

    uint8_t xMod4 = pixel->position.x & 0x03;
    uint8_t yMod2 = pixel->position.y & 0x01;

    shadow +=
      fogShadow + SFG_ditheringPatterns[fogShadowPart * 8 + yMod2 * 4 + xMod4];
#else
    shadow += pixel->depth / (RCL_UNITS_PER_SQUARE * 2);
#endif

    color = palette_minusValue(color,shadow);
  }
  else
  {
    color = SFG_getTexel(SFG_backgrounds[0],
    SFG_backgroundScaleMap[(pixel->position.x * SFG_RAYCASTING_SUBSAMPLE + SFG_backgroundScroll) % SFG_GAME_RESOLUTION_Y],
                                                                   // ^ TODO: get rid of mod?
    SFG_backgroundScaleMap[pixel->position.y]);
  }

  RCL_Unit screenX = pixel->position.x * SFG_RAYCASTING_SUBSAMPLE;

  for (uint8_t i = 0; i < SFG_RAYCASTING_SUBSAMPLE; ++i)
  {
    SFG_setGamePixel(screenX,pixel->position.y,color);
    screenX++;
  }
}

#define SFG_MAX_SPRITE_SIZE SFG_GAME_RESOLUTION_X

uint8_t SFG_spriteSamplingPoints[SFG_MAX_SPRITE_SIZE];

void SFG_drawScaledImage(
  const uint8_t *image,
  int16_t centerX,
  int16_t centerY,
  int16_t size)
{
  if ((size > SFG_MAX_SPRITE_SIZE) || (size == 0))
    return;

  uint16_t halfSize = size / 2;

  int16_t topLeftX = centerX - halfSize;
  int16_t topLeftY = centerY - halfSize; 

  int16_t x0, u0;

  if (topLeftX < 0)
  {
    u0 = -1 * topLeftX;
    x0 = 0;
  }
  else
  {
    u0 = 0;
    x0 = topLeftX;
  }

  int16_t x1 = topLeftX + size - 1;

  if (x1 >= SFG_GAME_RESOLUTION_X)
    x1 = SFG_GAME_RESOLUTION_X - 1;

  int16_t y0, v0;

  if (topLeftY < 0)
  {
    v0 = -1 * topLeftY;
    y0 = 0;
  }
  else
  {
    v0 = 0;
    y0 = topLeftY;
  }

  int16_t y1 = topLeftY + size - 1;

  if (y1 >= SFG_GAME_RESOLUTION_Y)
    y1 = SFG_GAME_RESOLUTION_Y - 1;

  if ((x0 > x1) || (y0 > y1)) // completely outside screen?
    return;

  int16_t u1 = u0 + (x1 - x0);
  int16_t v1 = v0 + (y1 - y0);

  // precompute sampling positions:

  int16_t uMin = RCL_min(u0,u1);
  int16_t vMin = RCL_min(v0,v1);
  int16_t uMax = RCL_max(u0,u1);
  int16_t vMax = RCL_max(v0,v1);

  int16_t precompFrom = RCL_min(uMin,vMin);
  int16_t precompTo = RCL_max(uMax,vMax);

  #define PRECOMP_SCALE 2048

  int16_t precompStepScaled = (SFG_TEXTURE_SIZE * PRECOMP_SCALE) / size;
  int16_t precompPosScaled = precompFrom * precompStepScaled;

  for (int16_t i = precompFrom; i <= precompTo; ++i)
  {
    SFG_spriteSamplingPoints[i] = precompPosScaled / PRECOMP_SCALE;
    precompPosScaled += precompStepScaled;
  }

  #undef PRECOMP_SCALE

  for (int16_t x = x0, u = u0; x <= x1; ++x, ++u)
    for (int16_t y = y0, v = v0; y <= y1; ++y, ++v)
      SFG_setGamePixel(x,y,SFG_getTexel(image,
                                    SFG_spriteSamplingPoints[u],
                                    SFG_spriteSamplingPoints[v]));
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
      (SFG_TILE_CEILING_HEIGHT(tile) + SFG_TILE_FLOOR_HEIGHT(tile))
         * SFG_WALL_HEIGHT_STEP,
      SFG_getTimeMs() - SFG_currentLevel.timeStart);
}

uint32_t SFG_gameFrame;
uint32_t SFG_lastFrameTimeMs;

void SFG_setAndInitLevel(const SFG_Level *level)
{
  SFG_LOG("setting and initializing level");

  SFG_currentLevel.levelPointer = level;

  SFG_currentLevel.floorColor = level->floorColor;
  SFG_currentLevel.ceilingColor = level->ceilingColor;

  for (uint8_t i = 0; i < 7; ++i)
    SFG_currentLevel.textures[i] =
      SFG_texturesWall[level->textureIndices[i]];

  SFG_LOG("initializing doors");

  SFG_currentLevel.doorRecordCount = 0;

  for (uint8_t j = 0; j < SFG_MAP_SIZE; ++j)
  {
    for (uint8_t i = 0; i < SFG_MAP_SIZE; ++i)
    {
      uint8_t properties;
     
      SFG_getMapTile(level,i,j,&properties);

      if ((properties & SFG_TILE_PROPERTY_MASK) == SFG_TILE_PROPERTY_DOOR)
      {
        SFG_DoorRecord *d =
          &(SFG_currentLevel.doors[SFG_currentLevel.doorRecordCount]);

        d->coords[0] = i;
        d->coords[1] = j;
        d->state = SFG_DOOR_DEFAULT_STATE;

        SFG_currentLevel.doorRecordCount++;
      }

      if (SFG_currentLevel.doorRecordCount >= SFG_MAX_DOORS)
        break;
    }

    if (SFG_currentLevel.doorRecordCount >= SFG_MAX_DOORS)
      break;
  }

  SFG_currentLevel.timeStart = SFG_getTimeMs(); 
  SFG_currentLevel.frameStart = SFG_gameFrame;
 
  SFG_initPlayer();
}

void SFG_init()
{
  SFG_LOG("initializing game")

  SFG_gameFrame = 0;
  SFG_lastFrameTimeMs = 0;

  RCL_initRayConstraints(&SFG_rayConstraints);

  SFG_rayConstraints.maxHits = SFG_RAYCASTING_MAX_HITS;
  SFG_rayConstraints.maxSteps = SFG_RAYCASTING_MAX_STEPS;

  for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_Y; ++i)
    SFG_backgroundScaleMap[i] =
      (i * SFG_TEXTURE_SIZE) / SFG_GAME_RESOLUTION_Y;

  SFG_backgroundScroll = 0;

  SFG_setAndInitLevel(&SFG_level0);
}

/**
  Performs one game step (logic, physics), happening SFG_MS_PER_FRAME after
  previous frame. 
*/
void SFG_gameStep()
{
  int8_t recomputeDirection = 0;

  RCL_Vector2D moveOffset;

  moveOffset.x = 0;
  moveOffset.y = 0;

  int8_t strafe = 0;

  if (SFG_keyPressed(SFG_KEY_A))
  {
    if (SFG_keyPressed(SFG_KEY_LEFT))
      strafe = -1;
    else if (SFG_keyPressed(SFG_KEY_RIGHT))
      strafe = 1;
  }
  else
  {
    if (SFG_keyPressed(SFG_KEY_LEFT))
    {
      SFG_player.camera.direction -= SFG_PLAYER_TURN_UNITS_PER_FRAME;
      recomputeDirection = 1;
    }
    else if (SFG_keyPressed(SFG_KEY_RIGHT))
    {
      SFG_player.camera.direction += SFG_PLAYER_TURN_UNITS_PER_FRAME;
      recomputeDirection = 1;
    }

    if (recomputeDirection)
      SFG_recompurePLayerDirection();
  }

  if (SFG_keyPressed(SFG_KEY_STRAFE_LEFT))
    strafe = -1;
  else if (SFG_keyPressed(SFG_KEY_STRAFE_RIGHT))
    strafe = 1;

  if (strafe != 0)
  {
    moveOffset.x = strafe * SFG_player.direction.y;
    moveOffset.y = -1 * strafe * SFG_player.direction.x;
  }

#if SFG_PREVIEW_MODE
  if (SFG_keyPressed(SFG_KEY_B))
    SFG_player.verticalSpeed = SFG_PLAYER_MOVE_UNITS_PER_FRAME;
  else if (SFG_keyPressed(SFG_KEY_C))
    SFG_player.verticalSpeed = -1 * SFG_PLAYER_MOVE_UNITS_PER_FRAME;
  else
    SFG_player.verticalSpeed = 0;
#else
  RCL_Unit verticalOffset = 
    (  
      (
        SFG_keyPressed(SFG_KEY_JUMP) ||
        (SFG_keyPressed(SFG_KEY_UP) && SFG_keyPressed(SFG_KEY_C))
      ) &&
      (SFG_player.verticalSpeed == 0) &&
      (SFG_player.previousVerticalSpeed == 0)) ?
    SFG_PLAYER_JUMP_SPEED :
    (SFG_player.verticalSpeed - SFG_GRAVITY_SPEED_INCREASE_PER_FRAME);
#endif

  if (SFG_keyPressed(SFG_KEY_UP))
  {
    moveOffset.x += SFG_player.direction.x;
    moveOffset.y += SFG_player.direction.y;
  }
  else if (SFG_keyPressed(SFG_KEY_DOWN))
  {
    moveOffset.x -= SFG_player.direction.x;
    moveOffset.y -= SFG_player.direction.y;
  }

  RCL_Unit previousHeight = SFG_player.camera.height;

#if SFG_PREVIEW_MODE
  SFG_player.camera.position.x +=
    SFG_PREVIEW_MODE_SPEED_MULTIPLIER * moveOffset.x;

  SFG_player.camera.position.y +=
    SFG_PREVIEW_MODE_SPEED_MULTIPLIER * moveOffset.y;

  SFG_player.camera.height += 
    SFG_PREVIEW_MODE_SPEED_MULTIPLIER * SFG_player.verticalSpeed;
#else
  RCL_moveCameraWithCollision(&(SFG_player.camera),moveOffset,
    verticalOffset,SFG_floorHeightAt,SFG_ceilingHeightAt,1,1);

  SFG_player.previousVerticalSpeed = SFG_player.verticalSpeed;

  RCL_Unit limit = RCL_max(RCL_max(0,verticalOffset),SFG_player.verticalSpeed);
  
  SFG_player.verticalSpeed =
    RCL_min(limit,SFG_player.camera.height - previousHeight);
  /* ^ By "limit" we assure height increase caused by climbing a step doesn't
     add vertical velocity. */
#endif

  SFG_player.squarePosition[0] =
    SFG_player.camera.position.x / RCL_UNITS_PER_SQUARE;

  SFG_player.squarePosition[1] =
    SFG_player.camera.position.y / RCL_UNITS_PER_SQUARE;

  // handle door:

  /* Check one door on whether a player is standing nearby. For performance
     reasons we only check one door and move to another in the next frame. */

  SFG_DoorRecord door =
    SFG_currentLevel.doors[SFG_currentLevel.checkedDoorIndex];

  if (
    (door.coords[0] >= (SFG_player.squarePosition[0] - 1)) &&
    (door.coords[0] <= (SFG_player.squarePosition[0] + 1)) &&
    (door.coords[1] >= (SFG_player.squarePosition[1] - 1)) &&
    (door.coords[1] <= (SFG_player.squarePosition[1] + 1)))
    printf("%d %d\n",SFG_player.squarePosition[0],SFG_player.squarePosition[1]);

  SFG_currentLevel.checkedDoorIndex++;

  if (SFG_currentLevel.checkedDoorIndex >= SFG_currentLevel.doorRecordCount)
    SFG_currentLevel.checkedDoorIndex = 0;
}

void SFG_mainLoopBody()
{
  /* standard deterministic game loop, independed on actuall achieved FPS,
     each game logic (physics) frame is performed with the SFG_MS_PER_FRAME
     delta time. */

  uint32_t timeNow = SFG_getTimeMs();
  uint32_t timeNextFrame = SFG_lastFrameTimeMs + SFG_MS_PER_FRAME;

  if (timeNow >= timeNextFrame)
  {
    uint32_t timeSinceLastFrame = timeNow - SFG_lastFrameTimeMs;

    uint8_t steps = 0;

    // perform game logic (physics), for each frame
    while (timeSinceLastFrame >= SFG_MS_PER_FRAME)
    {
      SFG_gameStep();

      timeSinceLastFrame -= SFG_MS_PER_FRAME;

      SFG_gameFrame++;
      steps++;
    }

    if (steps > 1)
      SFG_LOG("Failed to reach target FPS! Consider setting a lower value.")

    // render noly once
    RCL_renderComplex(SFG_player.camera,SFG_floorHeightAt,SFG_ceilingHeightAt,SFG_texturesAt,SFG_rayConstraints);

    SFG_lastFrameTimeMs = timeNow;
  }
  else
  {
    SFG_sleepMs((timeNextFrame - timeNow) / 2); // wait, relieve CPU
  }
}
