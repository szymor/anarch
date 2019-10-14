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
#define SFG_KEY_MAP 10

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
#include "settings.h" // will include if not included by platform

#define RCL_PIXEL_FUNCTION SFG_pixelFunc
#define RCL_TEXTURE_VERTICAL_STRETCH 0

#define RCL_CAMERA_COLL_HEIGHT_BELOW 800
#define RCL_CAMERA_COLL_HEIGHT_ABOVE 100

#include "raycastlib.h" 

#define SFG_GAME_RESOLUTION_X \
  (SFG_SCREEN_RESOLUTION_X / SFG_RESOLUTION_SCALEDOWN)

#define SFG_GAME_RESOLUTION_Y \
  (SFG_SCREEN_RESOLUTION_Y / SFG_RESOLUTION_SCALEDOWN)

#define SFG_MS_PER_FRAME (1000 / SFG_FPS) // ms per frame with target FPS

#if SFG_MS_PER_FRAME == 0
  #define SFG_MS_PER_FRAME 1
#endif

#define SFG_WEAPON_IMAGE_SCALE \
  (SFG_GAME_RESOLUTION_X / (SFG_TEXTURE_SIZE * 5))

#if SFG_WEAPON_IMAGE_SCALE == 0
  #define SFG_WEAPON_IMAGE_SCALE 1
#endif

#define SFG_WEAPONBOB_OFFSET_PIXELS \
  (SFG_WEAPONBOB_OFFSET * SFG_WEAPON_IMAGE_SCALE)

#define SFG_WEAPON_IMAGE_POSITION_X \
  (SFG_GAME_RESOLUTION_X / 2 - (SFG_WEAPON_IMAGE_SCALE * SFG_TEXTURE_SIZE) / 2)

#define SFG_WEAPON_IMAGE_POSITION_Y \
  (SFG_GAME_RESOLUTION_Y - (SFG_WEAPON_IMAGE_SCALE * SFG_TEXTURE_SIZE))

#define SFG_PLAYER_TURN_UNITS_PER_FRAME \
  ((SFG_PLAYER_TURN_SPEED * RCL_UNITS_PER_SQUARE) / (360 * SFG_FPS))

#if SFG_PLAYER_TURN_UNITS_PER_FRAME == 0
  #define SFG_PLAYER_TURN_UNITS_PER_FRAME 1
#endif

#define SFG_PLAYER_MOVE_UNITS_PER_FRAME \
  ((SFG_PLAYER_MOVE_SPEED * RCL_UNITS_PER_SQUARE) / SFG_FPS)

#if SFG_PLAYER_MOVE_UNITS_PER_FRAME == 0
  #define SFG_PLAYER_MOVE_UNITS_PER_FRAME 1
#endif

#define SFG_GRAVITY_SPEED_INCREASE_PER_FRAME \
  ((SFG_GRAVITY_ACCELERATION * RCL_UNITS_PER_SQUARE) / (SFG_FPS * SFG_FPS))

#if SFG_GRAVITY_SPEED_INCREASE_PER_FRAME == 0
  #define SFG_GRAVITY_SPEED_INCREASE_PER_FRAME 1
#endif

#define SFG_HEADBOB_FRAME_INCREASE_PER_FRAME \
  (SFG_HEADBOB_SPEED / SFG_FPS)

#if SFG_HEADBOB_FRAME_INCREASE_PER_FRAME == 0
  #define SFG_HEADBOB_FRAME_INCREASE_PER_FRAME 1
#endif

#define SFG_HEADBOB_ENABLED (SFG_HEADBOB_SPEED > 0 && SFG_HEADBOB_OFFSET > 0)

#define SFG_CAMERA_SHEAR_STEP_PER_FRAME \
  ((SFG_GAME_RESOLUTION_Y * SFG_CAMERA_SHEAR_SPEED) / SFG_FPS)

#if SFG_CAMERA_SHEAR_STEP_PER_FRAME == 0
  #define SFG_CAMERA_SHEAR_STEP_PER_FRAME 1
#endif

#define SFG_CAMERA_MAX_SHEAR_PIXELS \
  (SFG_CAMERA_MAX_SHEAR * SFG_GAME_RESOLUTION_Y / 1024)

#define SFG_FONT_SIZE_SMALL \
 (SFG_GAME_RESOLUTION_X / (SFG_FONT_CHARACTER_SIZE * 50))

#if SFG_FONT_SIZE_SMALL == 0
  #define SFG_FONT_SIZE_SMALL 1
#endif

#define SFG_FONT_SIZE_MEDIUM \
  (SFG_GAME_RESOLUTION_X / (SFG_FONT_CHARACTER_SIZE * 30))

#if SFG_FONT_SIZE_MEDIUM == 0
  #define SFG_FONT_SIZE_MEDIUM 1
#endif

#define SFG_FONT_SIZE_BIG \
  (SFG_GAME_RESOLUTION_X / (SFG_FONT_CHARACTER_SIZE * 18))

#if SFG_FONT_SIZE_BIG == 0
  #define SFG_FONT_SIZE_BIG 1
#endif

#define SFG_Z_BUFFER_SIZE \
  (SFG_GAME_RESOLUTION_X / SFG_RAYCASTING_SUBSAMPLE + 1)

uint8_t SFG_zBuffer[SFG_Z_BUFFER_SIZE];

#define SFG_RCL_UNIT_TO_Z_BUFFER(x) (x / RCL_UNITS_PER_SQUARE)

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
  uint16_t headBobFrame;
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

  SFG_player.direction.x =
    (SFG_player.direction.x * SFG_PLAYER_MOVE_UNITS_PER_FRAME)
    / RCL_UNITS_PER_SQUARE;

  SFG_player.direction.y =
    (SFG_player.direction.y * SFG_PLAYER_MOVE_UNITS_PER_FRAME)
    / RCL_UNITS_PER_SQUARE;

  SFG_backgroundScroll =
    ((SFG_player.camera.direction * 8) * SFG_GAME_RESOLUTION_Y)
    / RCL_UNITS_PER_SQUARE; 
}

void SFG_initPlayer()
{
  RCL_initCamera(&SFG_player.camera);

  SFG_player.camera.resolution.x =
    SFG_GAME_RESOLUTION_X / SFG_RAYCASTING_SUBSAMPLE;

  SFG_player.camera.resolution.y = SFG_GAME_RESOLUTION_Y;

  SFG_player.camera.height = RCL_UNITS_PER_SQUARE * 12;
  SFG_player.camera.position.x = RCL_UNITS_PER_SQUARE * 15;
  SFG_player.camera.position.y = RCL_UNITS_PER_SQUARE * 8;

  SFG_recompurePLayerDirection();
  SFG_player.verticalSpeed = 0;
  SFG_player.previousVerticalSpeed = 0;

  SFG_player.headBobFrame = 0;
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
#define SFG_DOOR_UP_DOWN_MASK 0x20
#define SFG_DOOR_VERTICAL_POSITION_MASK 0x1f
#define SFG_DOOR_HEIGHT_STEP (RCL_UNITS_PER_SQUARE / 0x1f)
#define SFG_DOOR_INCREMENT_PER_FRAME \
  (SFG_DOOR_OPEN_SPEED / (SFG_DOOR_HEIGHT_STEP * SFG_FPS))

#if SFG_DOOR_INCREMENT_PER_FRAME == 0
  #define SFG_DOOR_INCREMENT_PER_FRAME 1
#endif

#define SFG_MAX_DOORS 32

/**
  Holds information about one instance of a level item (a type of level element,
  e.g. pickable items, decorations etc.). The format is following:

  MSB  abbbbbbb  LSB

  a:        active flag, 1 means the item is nearby to player and is active
  bbbbbbb:  index to elements array of the current level, pointing to element
            representing this item 
*/
typedef uint8_t SFG_ItemRecord;

#define SFG_ITEM_RECORD_ACTIVE_MASK 0x80

#define SFG_MAX_LEVEL_ITEMS SFG_MAX_LEVEL_ELEMENTS

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

  SFG_DoorRecord doorRecords[SFG_MAX_DOORS];
  uint8_t doorRecordCount;
  uint8_t checkedDoorIndex; ///< Says which door are currently being checked.

  SFG_ItemRecord itemRecords[SFG_MAX_LEVEL_ITEMS]; ///< Holds level items
  uint8_t itemRecordCount;
  uint8_t checkedItemIndex; ///< Same as checkedDoorIndex, but for items.

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

  if (pixel->position.y == SFG_GAME_RESOLUTION_Y / 2)
    SFG_zBuffer[pixel->position.x / SFG_RAYCASTING_SUBSAMPLE] =
      SFG_RCL_UNIT_TO_Z_BUFFER(pixel->depth);

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

#if SFG_ENABLE_FOG
    color = palette_minusValue(color,shadow);
#endif
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

/**
  Draws image on screen, with transparency. This is faster than sprite drawing.
  For performance sake drawing near screen edges is not pixel perfect.
*/
void SFG_blitImage(
  const uint8_t *image,
  int16_t posX,
  int16_t posY,
  uint8_t scale)
{
  if (scale == 0)
    return;
 
  uint16_t x0 = posX,
           x1,
           y0 = posY, 
           y1;
 
  uint8_t u0 = 0, v0 = 0;

  if (posX < 0)
  {
    x0 = 0;
    u0 = (-1 * posX) / scale;
  }

  posX += scale * SFG_TEXTURE_SIZE;

  uint16_t limitX = SFG_GAME_RESOLUTION_X - scale;
  uint16_t limitY = SFG_GAME_RESOLUTION_Y - scale;

  x1 = posX >= 0 ?
       (posX <= limitX ? posX : limitX)
       : 0;

  if (x1 >= SFG_GAME_RESOLUTION_X)
    x1 = SFG_GAME_RESOLUTION_X - 1;

  if (posY < 0)
  {
    y0 = 0;
    v0 = (-1 * posY) / scale;
  }

  posY += scale * SFG_TEXTURE_SIZE;

  y1 = posY >= 0 ?
       (posY <= limitY ? posY : limitY)
       : 0;

  if (y1 >= SFG_GAME_RESOLUTION_Y)
    y1 = SFG_GAME_RESOLUTION_Y - 1;

  uint8_t u,v;

  v = v0;

  for (uint16_t y = y0; y < y1; y += scale)
  {
    u = u0;

    for (uint16_t x = x0; x < x1; x += scale)
    {
      uint8_t color = SFG_getTexel(image,u,v);

      if (color != SFG_TRANSPARENT_COLOR)
      {
        uint16_t sY = y;

        for (uint8_t j = 0; j < scale; ++j)
        {
          uint16_t sX = x;

          for (uint8_t i = 0; i < scale; ++i)
          {
            SFG_setGamePixel(sX,sY,color);
            sX++;
          }
          
          sY++;
        }
      }

      u++;
    }

    v++;
  }
}

#define SFG_MAX_SPRITE_SIZE SFG_GAME_RESOLUTION_X

uint8_t SFG_spriteSamplingPoints[SFG_MAX_SPRITE_SIZE];

void SFG_drawScaledSprite(
  const uint8_t *image,
  int16_t centerX,
  int16_t centerY,
  int16_t size,
  uint8_t minusValue,
  RCL_Unit distance)
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

  if ((x0 > x1) || (y0 > y1) || (u0 >= size) || (v0 >= size))
    return;     // outside screen?

  int16_t u1 = u0 + (x1 - x0);
  int16_t v1 = v0 + (y1 - y0);

  // precompute sampling positions:

  int16_t uMin = RCL_min(u0,u1);
  int16_t vMin = RCL_min(v0,v1);
  int16_t uMax = RCL_max(u0,u1);
  int16_t vMax = RCL_max(v0,v1);

  int16_t precompFrom = RCL_min(uMin,vMin);
  int16_t precompTo = RCL_max(uMax,vMax);

  precompFrom = RCL_max(0,precompFrom);
  precompTo = RCL_min(SFG_MAX_SPRITE_SIZE - 1,precompTo);

  #define PRECOMP_SCALE 2048

  int16_t precompStepScaled = (SFG_TEXTURE_SIZE * PRECOMP_SCALE) / size;
  int16_t precompPosScaled = precompFrom * precompStepScaled;

  for (int16_t i = precompFrom; i <= precompTo; ++i)
  {
    SFG_spriteSamplingPoints[i] = precompPosScaled / PRECOMP_SCALE;
    precompPosScaled += precompStepScaled;
  }

  #undef PRECOMP_SCALE

  uint8_t zDistance = SFG_RCL_UNIT_TO_Z_BUFFER(distance);

  for (int16_t x = x0, u = u0; x <= x1; ++x, ++u)
  {
    if (SFG_zBuffer[x / SFG_RAYCASTING_SUBSAMPLE] >= zDistance)
    {
      int8_t columnTransparent = 1;

      for (int16_t y = y0, v = v0; y <= y1; ++y, ++v)
      {
        uint8_t color =
          SFG_getTexel(image,SFG_spriteSamplingPoints[u],
            SFG_spriteSamplingPoints[v]);

        if (color != SFG_TRANSPARENT_COLOR)
        {
#if SFG_DIMINISH_SPRITES
          color = palette_minusValue(color,minusValue);
#endif
 
          columnTransparent = 0;

          SFG_setGamePixel(x,y,color);
        }
      }

      if (!columnTransparent)
        SFG_zBuffer[x / SFG_RAYCASTING_SUBSAMPLE] = zDistance;
    }
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

uint32_t SFG_frameTime; ///< Keeps a constant time (in ms) during a frame

RCL_Unit SFG_floorHeightAt(int16_t x, int16_t y)
{
  uint8_t properties;

  SFG_TileDefinition tile =
    SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&properties);

  uint8_t doorHeight = 0;

  if (properties == SFG_TILE_PROPERTY_DOOR)
  {
    for (uint8_t i = 0; i < SFG_currentLevel.doorRecordCount; ++i)
    {
      SFG_DoorRecord *door = &(SFG_currentLevel.doorRecords[i]);

      if ((door->coords[0] == x) && (door->coords[1] == y))
      {
        doorHeight = door->state & SFG_DOOR_VERTICAL_POSITION_MASK;
        break;
      }
    }
  }
  else if (properties == SFG_TILE_PROPERTY_ELEVATOR)
  {
    return SFG_movingWallHeight(
      SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_TILE_CEILING_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_frameTime - SFG_currentLevel.timeStart);
  }
 
  return SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP -
           doorHeight * SFG_DOOR_HEIGHT_STEP;
}

RCL_Unit SFG_ceilingHeightAt(int16_t x, int16_t y)
{
  uint8_t properties;
  SFG_TileDefinition tile =
    SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&properties);

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
      SFG_frameTime - SFG_currentLevel.timeStart);
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

  SFG_currentLevel.checkedDoorIndex = 0;

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
          &(SFG_currentLevel.doorRecords[SFG_currentLevel.doorRecordCount]);

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

  SFG_LOG("initializing level elements");

  SFG_currentLevel.checkedItemIndex = 0;

  SFG_currentLevel.itemRecordCount = 0;

  for (uint8_t i = 0; i < SFG_MAX_LEVEL_ELEMENTS; ++i)
  {
    const SFG_LevelElement *e = &(SFG_currentLevel.levelPointer->elements[i]);

    if (e->elementType == SFG_LEVEL_ELEMENT_BARREL)
    {
      SFG_currentLevel.itemRecords[SFG_currentLevel.itemRecordCount] = i;

      SFG_currentLevel.itemRecordCount++;
    }
  } 

  SFG_currentLevel.timeStart = SFG_getTimeMs(); 
  SFG_currentLevel.frameStart = SFG_gameFrame;
 
  SFG_initPlayer();
}

void SFG_init()
{
  SFG_LOG("initializing game")

  SFG_gameFrame = 0;

  RCL_initRayConstraints(&SFG_rayConstraints);

  SFG_rayConstraints.maxHits = SFG_RAYCASTING_MAX_HITS;
  SFG_rayConstraints.maxSteps = SFG_RAYCASTING_MAX_STEPS;

  for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_Y; ++i)
    SFG_backgroundScaleMap[i] =
      (i * SFG_TEXTURE_SIZE) / SFG_GAME_RESOLUTION_Y;

  SFG_backgroundScroll = 0;

  SFG_setAndInitLevel(&SFG_level0);

  SFG_lastFrameTimeMs = SFG_getTimeMs();
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

#if SFG_HEADBOB_ENABLED
  int8_t bobbing = 0;
#endif

  int8_t shearing = 0;

  if (SFG_keyPressed(SFG_KEY_A))
  {
    if (SFG_keyPressed(SFG_KEY_UP))
    {
      SFG_player.camera.shear =
        RCL_min(SFG_CAMERA_MAX_SHEAR_PIXELS,
                SFG_player.camera.shear + SFG_CAMERA_SHEAR_STEP_PER_FRAME);

      shearing = 1;
    }
    else if (SFG_keyPressed(SFG_KEY_DOWN))
    {
      SFG_player.camera.shear =
        RCL_max(-1 * SFG_CAMERA_MAX_SHEAR_PIXELS,
                SFG_player.camera.shear - SFG_CAMERA_SHEAR_STEP_PER_FRAME);

      shearing = 1;
    }

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

    if (SFG_keyPressed(SFG_KEY_UP))
    {
      moveOffset.x += SFG_player.direction.x;
      moveOffset.y += SFG_player.direction.y;
#if SFG_HEADBOB_ENABLED
      bobbing = 1;
#endif
    }
    else if (SFG_keyPressed(SFG_KEY_DOWN))
    {
      moveOffset.x -= SFG_player.direction.x;
      moveOffset.y -= SFG_player.direction.y;
#if SFG_HEADBOB_ENABLED
      bobbing = 1;
#endif
    }
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

  if (!shearing && SFG_player.camera.shear != 0)
  {
    // gradually shear back to zero

    SFG_player.camera.shear =
      (SFG_player.camera.shear > 0) ?
      RCL_max(0,SFG_player.camera.shear - SFG_CAMERA_SHEAR_STEP_PER_FRAME) :
      RCL_min(0,SFG_player.camera.shear + SFG_CAMERA_SHEAR_STEP_PER_FRAME);
  }

#if SFG_HEADBOB_ENABLED
  if (bobbing)
  {
    SFG_player.headBobFrame += SFG_HEADBOB_FRAME_INCREASE_PER_FRAME; 
  }
  else if (SFG_player.headBobFrame != 0)
  {
    // smoothly stop bobbing

    uint8_t quadrant = (SFG_player.headBobFrame % RCL_UNITS_PER_SQUARE) /
      (RCL_UNITS_PER_SQUARE / 4);

    /* when in quadrant in which sin is going away from zero, switch to the
       same value of the next quadrant, so that bobbing starts to go towards
       zero immediately */

    if (quadrant % 2 == 0)
      SFG_player.headBobFrame =
        ((quadrant + 1) * RCL_UNITS_PER_SQUARE / 4) +
        (RCL_UNITS_PER_SQUARE / 4 - SFG_player.headBobFrame %
        (RCL_UNITS_PER_SQUARE / 4));

    RCL_Unit currentFrame = SFG_player.headBobFrame;
    RCL_Unit nextFrame = SFG_player.headBobFrame + 16;

    // only stop bobbing when we pass frame at which sin crosses ero
    SFG_player.headBobFrame =
      (currentFrame / (RCL_UNITS_PER_SQUARE / 2) ==
       nextFrame / (RCL_UNITS_PER_SQUARE / 2)) ?
       nextFrame : 0;
  }
#endif

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

  if (SFG_currentLevel.doorRecordCount > 0) // has to be here
  {
    /* Check one door on whether a player is standing nearby. For performance
       reasons we only check one door and move to another in the next frame. */

    SFG_DoorRecord *door =
      &(SFG_currentLevel.doorRecords[SFG_currentLevel.checkedDoorIndex]);

    door->state = (door->state & ~SFG_DOOR_UP_DOWN_MASK) |
      (
        ((door->coords[0] >= (SFG_player.squarePosition[0] - 1)) &&
         (door->coords[0] <= (SFG_player.squarePosition[0] + 1)) &&
         (door->coords[1] >= (SFG_player.squarePosition[1] - 1)) &&
         (door->coords[1] <= (SFG_player.squarePosition[1] + 1))) ?
         SFG_DOOR_UP_DOWN_MASK : 0x00 
      );

    SFG_currentLevel.checkedDoorIndex++;

    if (SFG_currentLevel.checkedDoorIndex >= SFG_currentLevel.doorRecordCount)
      SFG_currentLevel.checkedDoorIndex = 0;

    for (uint32_t i = 0; i < SFG_currentLevel.doorRecordCount; ++i)
    {
      SFG_DoorRecord *door = &(SFG_currentLevel.doorRecords[i]);

      int8_t height = door->state & SFG_DOOR_VERTICAL_POSITION_MASK;

      height = (door->state & SFG_DOOR_UP_DOWN_MASK) ?
            RCL_min(0x1f,height + SFG_DOOR_INCREMENT_PER_FRAME) :
            RCL_max(0x00,height - SFG_DOOR_INCREMENT_PER_FRAME);

      door->state = (door->state & ~SFG_DOOR_VERTICAL_POSITION_MASK) | height;
    }
  }

  // handle items, in a similar manner to door:

  if (SFG_currentLevel.itemRecordCount > 0) // has to be here
  {
    SFG_ItemRecord item =
      SFG_currentLevel.itemRecords[SFG_currentLevel.checkedItemIndex];

    item &= ~SFG_ITEM_RECORD_ACTIVE_MASK;

    SFG_LevelElement e =
      SFG_currentLevel.levelPointer->elements[item];

    if (
        (RCL_absVal(SFG_player.squarePosition[0] - e.coords[0])
        <= SFG_LEVEL_ELEMENT_ACTIVE_DISTANCE)
        &&
        (RCL_absVal(SFG_player.squarePosition[1] - e.coords[1])
        <= SFG_LEVEL_ELEMENT_ACTIVE_DISTANCE)
      )
      item |= SFG_ITEM_RECORD_ACTIVE_MASK;

    SFG_currentLevel.itemRecords[SFG_currentLevel.checkedItemIndex] = item;

    SFG_currentLevel.checkedItemIndex++;

    if (SFG_currentLevel.checkedItemIndex >= SFG_currentLevel.itemRecordCount)
      SFG_currentLevel.checkedItemIndex = 0;
  }
}

void SFG_clearScreen(uint8_t color)
{
  for (uint16_t j = 0; j < SFG_GAME_RESOLUTION_Y; ++j)
    for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_X; ++i)
      SFG_setGamePixel(i,j,color);
}

#define SFG_MAP_PIXEL_SIZE (SFG_GAME_RESOLUTION_Y / SFG_MAP_SIZE)

#if SFG_MAP_PIXEL_SIZE == 0
  #define SFG_MAP_SIZE 1
#endif

/**
  Draws fullscreen map of the current level.
*/
void SFG_drawMap()
{
  SFG_clearScreen(0);
    
  uint16_t maxJ =
    (SFG_MAP_PIXEL_SIZE * SFG_MAP_SIZE) < SFG_GAME_RESOLUTION_Y ?
    (SFG_MAP_SIZE) : (SFG_GAME_RESOLUTION_Y / SFG_MAP_PIXEL_SIZE);

  uint16_t maxI =
    (SFG_MAP_PIXEL_SIZE * SFG_MAP_SIZE) < SFG_GAME_RESOLUTION_X ?
    (SFG_MAP_SIZE) : (SFG_GAME_RESOLUTION_X / SFG_MAP_PIXEL_SIZE);

  uint16_t topLeftX =
    (SFG_GAME_RESOLUTION_X - (maxI * SFG_MAP_PIXEL_SIZE)) / 2;

  uint16_t topLeftY =
    (SFG_GAME_RESOLUTION_Y - (maxJ * SFG_MAP_PIXEL_SIZE)) / 2;

  uint16_t x;
  uint16_t y = topLeftY;

  for (int16_t j = maxJ - 1; j >= 0; --j)
  {
    x = topLeftX;

    for (uint16_t i = 0; i < maxI; ++i)
    {
      uint8_t properties;

      SFG_TileDefinition tile =
        SFG_getMapTile(SFG_currentLevel.levelPointer,i,j,&properties);

      uint8_t color = 94; // init with player color

      if (i != SFG_player.squarePosition[0] ||
          j != SFG_player.squarePosition[1])
      {
        if (properties == SFG_TILE_PROPERTY_ELEVATOR)
          color = 46;
        else if (properties == SFG_TILE_PROPERTY_SQUEEZER)
          color = 63;
        else
        {
          color = SFG_TILE_FLOOR_HEIGHT(tile) / 8 + 2;

          if (properties == SFG_TILE_PROPERTY_DOOR)
            color += 8;
        }
      }

      for (uint16_t k = 0; k < SFG_MAP_PIXEL_SIZE; ++k)
        for (uint16_t l = 0; l < SFG_MAP_PIXEL_SIZE; ++l)
          SFG_setGamePixel(x + l, y + k,color);

      x += SFG_MAP_PIXEL_SIZE;
    }

    y += SFG_MAP_PIXEL_SIZE;
  } 
}

/**
  Draws text on screen using the bitmap font stored in assets.
*/
void SFG_drawText(
  const char *text,
  uint16_t x,
  uint16_t y,
  uint8_t size,
  uint8_t color)
{
  if (size == 0)
    size = 1;

  uint16_t pos = 0;

  uint16_t currentX = x;
  uint16_t currentY = y;

  while (text[pos] != 0)
  {
    uint16_t character = SFG_font[SFG_charToFontIndex(text[pos])];

    for (uint8_t i = 0; i < 4; ++i)
    {
      currentY = y;

      for (uint8_t j = 0; j < 4; ++j)
      {
        if (character & 0x8000)
          for (uint8_t k = 0; k < size; ++k)
            for (uint8_t l = 0; l < size; ++l)
            {
              uint16_t drawX = currentX + k;
              uint16_t drawY = currentY + l;

              if (drawX < SFG_GAME_RESOLUTION_X &&
                drawY < SFG_GAME_RESOLUTION_Y)
                SFG_setGamePixel(drawX,drawY,color);
            }

        currentY += size;
        character = character << 1;
      }

      currentX += size;

      if (currentX >= SFG_GAME_RESOLUTION_X)
        break;
    }
    
    currentX += size;
      
    if (currentX >= SFG_GAME_RESOLUTION_X)
      break;

    pos++;    
  }
}

/**
  Draws a number as text on screen, returns the number of characters drawn.
*/
uint8_t SFG_drawNumber(
  int16_t number,
  uint16_t x,
  uint16_t y,
  uint8_t size,
  uint8_t color
)
{
  char text[7];

  text[6] = 0; // terminate the string

  int8_t positive = 1;

  if (number < 0)
  {
    positive = 0;
    number *= -1;
  }

  int8_t position = 5;

  while (1)
  {
    text[position] = '0' + number % 10;
    number /= 10;

    position--;

    if (number == 0 || position == 0)
      break;
  }

  if (!positive)
  {
    text[position] = '-';
    position--;
  }

  SFG_drawText(text + position + 1,x,y,size,color);

  return 5 - position;
}

void SFG_draw()
{
  if (SFG_keyPressed(SFG_KEY_MAP))
  {
    SFG_drawMap();
  } 
  else
  { 
    for (uint16_t i = 0; i < SFG_Z_BUFFER_SIZE; ++i)
      SFG_zBuffer[i] = 255;

int16_t weaponBobOffset;

#if SFG_HEADBOB_ENABLED
    RCL_Unit bobSin = RCL_sinInt(SFG_player.headBobFrame);

    RCL_Unit headBobOffset =
      (bobSin * SFG_HEADBOB_OFFSET) / RCL_UNITS_PER_SQUARE;

    weaponBobOffset =
      (bobSin * SFG_WEAPONBOB_OFFSET_PIXELS) / (RCL_UNITS_PER_SQUARE) + 
      SFG_WEAPONBOB_OFFSET_PIXELS;

    // add head bob just for the rendering
    SFG_player.camera.height += headBobOffset;
#endif

    RCL_renderComplex(
      SFG_player.camera,
      SFG_floorHeightAt,
      SFG_ceilingHeightAt,
      SFG_texturesAt,
      SFG_rayConstraints);
 
    // draw sprites:

    for (uint8_t i = 0; i < SFG_currentLevel.itemRecordCount; ++i)
      if (SFG_currentLevel.itemRecords[i] & SFG_ITEM_RECORD_ACTIVE_MASK)
      {
        RCL_Vector2D worldPosition;

        SFG_LevelElement e = 
          SFG_currentLevel.levelPointer->elements[
            SFG_currentLevel.itemRecords[i] & ~SFG_ITEM_RECORD_ACTIVE_MASK];

        worldPosition.x =
          e.coords[0] * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2;

        worldPosition.y =
          e.coords[1] * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2;

        RCL_PixelInfo p =
          RCL_mapToScreen(
            worldPosition,
            SFG_floorHeightAt(e.coords[0],e.coords[1]) + RCL_UNITS_PER_SQUARE / 2,
            SFG_player.camera);

        if (p.depth > 0)
          SFG_drawScaledSprite(SFG_sprites[0],
            p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
            RCL_perspectiveScale(SFG_GAME_RESOLUTION_Y / 2,p.depth),
            p.depth / (RCL_UNITS_PER_SQUARE * 2),p.depth);
      }

#if SFG_HEADBOB_ENABLED
    // substract head bob after rendering
    SFG_player.camera.height -= headBobOffset;
#endif



SFG_drawText("124",10,SFG_GAME_RESOLUTION_Y - 10 - SFG_FONT_CHARACTER_SIZE * SFG_FONT_SIZE_MEDIUM,SFG_FONT_SIZE_MEDIUM,7);
SFG_drawText("ammo",

SFG_GAME_RESOLUTION_X - 10 - 4 * (SFG_FONT_CHARACTER_SIZE * SFG_FONT_SIZE_MEDIUM + 1)
,SFG_GAME_RESOLUTION_Y - 10 - SFG_FONT_CHARACTER_SIZE * SFG_FONT_SIZE_MEDIUM,SFG_FONT_SIZE_MEDIUM,7);

SFG_blitImage(SFG_weaponImages[0],
SFG_WEAPON_IMAGE_POSITION_X,
SFG_WEAPON_IMAGE_POSITION_Y + weaponBobOffset,
SFG_WEAPON_IMAGE_SCALE);

  }
}

void SFG_mainLoopBody()
{
  /* standard deterministic game loop, independed on actuall achieved FPS,
     each game logic (physics) frame is performed with the SFG_MS_PER_FRAME
     delta time. */

  uint32_t timeNow = SFG_getTimeMs();
  uint32_t timeNextFrame = SFG_lastFrameTimeMs + SFG_MS_PER_FRAME;

  SFG_frameTime = timeNow;

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
    SFG_draw();

    SFG_lastFrameTimeMs = timeNow;
  }
  else
  {
    SFG_sleepMs((timeNextFrame - timeNow) / 2); // wait, relieve CPU
  }
}
