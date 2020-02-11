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
#define SFG_KEY_TOGGLE_FREELOOK 11
#define SFG_KEY_NEXT_WEAPON 12
#define SFG_KEY_PREVIOUS_WEAPON 13

#define SFG_KEY_COUNT 14 ///< Number of keys.

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

/** 
  Returns 1 (0) if given key is pressed (not pressed). At least the mandatory
  keys have to be implemented, the optional keys don't have to ever return 1.
  See the key contant definitions to see which ones are mandatory.
*/
int8_t SFG_keyPressed(uint8_t key);

/**
  Optinal function for mouse/analog controls, gets mouse x and y offset in
  pixels from the game screen center (to achieve classic FPS mouse controls the
  platform should center the mouse at the end). If the platform isn't using a
  mouse, this function should simply return [0,0] offets at each call.
*/
void SFG_getMouseOffset(int16_t *x, int16_t *y);

/**
  Returns time in ms sice program start.
*/
uint32_t SFG_getTimeMs();

/** 
  Sleep (yield CPU) for specified amount of ms. This is used to relieve CPU
  usage. If your platform doesn't need this or handles it in other way, this
  function can do nothing.
*/
void SFG_sleepMs(uint16_t timeMs);

/**
  Set specified screen pixel. The function doesn't have to check whether
  the coordinates are within screen.
*/
static inline void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex);

/**
  Play given sound effect (SFX). This function may or may not use the sound
  samples provided in sounds.h, and it may or may not ignore the volume
  parameter (which is 0 to 255). Depending on the platform the function can play
  completely different samples or even e.g. just beeps. If the platform can't
  play sounds, this function implementation can simply be left empty. This
  function doesn't have to implement safety measures, the back end takes cares
  of them.
*/
void SFG_playSound(uint8_t soundIndex, uint8_t volume);

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

#define RCL_PIXEL_FUNCTION SFG_pixelFunc
#define RCL_TEXTURE_VERTICAL_STRETCH 0

#define RCL_CAMERA_COLL_HEIGHT_BELOW 800
#define RCL_CAMERA_COLL_HEIGHT_ABOVE 200

#include "raycastlib.h" 

#include "images.h"
#include "levels.h"
#include "palette.h"
#include "settings.h" // will include if not included by platform
#include "constants.h"

typedef struct
{
  uint8_t coords[2];
  uint8_t state;     /**< door state in format:
                          
                          MSB  ccbaaaaa  LSB

                          aaaaa: current door height (how much they're open)
                          b:     whether currently going up (0) or down (1)
                          cc:    by which card (key) the door is unlocked, 00
                                 means no card (unlocked), 1 means card 0 etc.
                     */
} SFG_DoorRecord;

#define SFG_SPRITE_SIZE(size0to3) \
  (((size0to3 + 3) * SFG_BASE_SPRITE_SIZE) / 4)

#define SFG_SPRITE_SIZE_TO_HEIGH_ABOVE_GROUND(size0to3) \
  ((((size0to3) + 3) * (RCL_UNITS_PER_SQUARE / 2)) / 4)

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

#define SFG_ITEM_RECORD_LEVEL_ELEMENT(itemRecord) \
  (SFG_currentLevel.levelPointer->elements[itemRecord & \
  ~SFG_ITEM_RECORD_ACTIVE_MASK])

typedef struct 
{
  uint8_t stateType;     /**< Holds state (lower 4 bits) and type of monster
                              (upper 4 bits). */
  uint8_t coords[2];     /**< Monster position, in 1/4s of a square */
  uint8_t health;
} SFG_MonsterRecord;

#define SFG_MR_STATE(mr) ((mr).stateType & SFG_MONSTER_MASK_STATE)
#define SFG_MR_TYPE(mr) ((mr).stateType & SFG_MONSTER_MASK_TYPE)

#define SFG_MONSTER_COORD_TO_RCL_UNITS(c) (c * 256)
#define SFG_MONSTER_COORD_TO_SQUARES(c) (c / 4)

#define SFG_ELEMENT_COORD_TO_RCL_UNITS(c) \
  (c * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2)

#define SFG_MONSTER_MASK_STATE 0x0f
#define SFG_MONSTER_MASK_TYPE  0xf0

#define SFG_MONSTER_STATE_INACTIVE  0  ///< Not nearby, not actively updated.
#define SFG_MONSTER_STATE_IDLE      1
#define SFG_MONSTER_STATE_ATTACKING 2
#define SFG_MONSTER_STATE_HURTING   3
#define SFG_MONSTER_STATE_DYING     4
#define SFG_MONSTER_STATE_GOING_N   5
#define SFG_MONSTER_STATE_GOING_NE  6
#define SFG_MONSTER_STATE_GOING_E   7
#define SFG_MONSTER_STATE_GOING_SE  8
#define SFG_MONSTER_STATE_GOING_S   9
#define SFG_MONSTER_STATE_GOING_SW  10
#define SFG_MONSTER_STATE_GOING_W   11
#define SFG_MONSTER_STATE_GOING_NW  12

typedef struct
{
  uint8_t  type;
  uint8_t  doubleFramesToLive; /**< This number times two (because 256 could be
                                    too little at high FPS) says after how many
                                    frames the projectile is destroyed. */
  uint16_t position[3]; /**< Current position, stored as u16 to save space, as
                             that is exactly enough to store position on 64x64
                             map. */
  int16_t direction[3]; /**< Added to position each game step. */
} SFG_ProjectileRecord;

/*
  GLOBAL VARIABLES
===============================================================================
*/

/**
  Groups global variables related to the game as such in a single struct. There
  are still other global structs for player, level etc.
*/
struct
{
  uint8_t currentRandom;         ///< for RNG
  uint8_t spriteAnimationFrame;
  uint8_t explosionSoundPlayed;  /**< Prevents playing too many explosion
                                     sounds at once */
  RCL_RayConstraints rayConstraints;
  uint8_t keyStates[SFG_KEY_COUNT]; /**< Pressed states of keys, LSB of each
                                     value means current pressed states, other
                                     bits store states in previous frames. */
  uint8_t zBuffer[SFG_Z_BUFFER_SIZE];
  int8_t backgroundScaleMap[SFG_GAME_RESOLUTION_Y];
  uint16_t backgroundScroll;
  uint8_t spriteSamplingPoints[SFG_MAX_SPRITE_SIZE]; /**< Helper for
                                                     precomputing sprite
                                                     sampling positions for
                                                     drawing. */
  uint32_t frameTime; ///< Keeps a constant time (in ms) during a frame
  uint32_t frame;
  uint32_t lastFrameTimeMs;
} SFG_game;

/**
  Stores player state.
*/
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
  uint8_t  weapon;                 ///< currently selected weapon

  uint8_t  health;

  uint32_t weaponCooldownStartFrame;   /**< frame from which weapon cooldown is
                                            counted */
  uint32_t lastHurtFrame;
  uint32_t lastItemTakenFrame;

  uint8_t  ammo[SFG_AMMO_TOTAL];

  uint8_t  freeLook;             /**< If on, the vertical looking (shear) does
                                 not automatically shear back. This is mainly 
                                 for mouse control. */
  uint8_t  cards;                /**< Lowest bits say which access cards have
                                 been taken. */
  uint8_t  justTeleported;
} SFG_player;

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

  SFG_ItemRecord itemRecords[SFG_MAX_ITEMS]; ///< Holds level items
  uint8_t itemRecordCount;
  uint8_t checkedItemIndex; ///< Same as checkedDoorIndex, but for items.

  SFG_MonsterRecord monsterRecords[SFG_MAX_MONSTERS];
  uint8_t monsterRecordCount;
  uint8_t checkedMonsterIndex; 

  SFG_ProjectileRecord projectileRecords[SFG_MAX_PROJECTILES];
  uint8_t projectileRecordCount;

  uint8_t backgroundImage;

  uint8_t teleportCount;
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

/*
  FUNCTIONS
===============================================================================
*/

/**
  Returns a pseudorandom byte. This is a congrent generator, its parameters
  have been chosen so that each number (0-255) is included in the output
  exactly once!
*/
uint8_t SFG_random()
{
  SFG_game.currentRandom *= 13;
  SFG_game.currentRandom += 7;
  
  return SFG_game.currentRandom;
}

void SFG_playSoundSafe(uint8_t soundIndex, uint8_t volume)
{
  if (soundIndex == 2) // explosion?
  {
    if (!SFG_game.explosionSoundPlayed)
      SFG_playSound(soundIndex,volume);
    
    SFG_game.explosionSoundPlayed = 1;
  }
  else
    SFG_playSound(soundIndex,volume);
}

/**
  Returns a damage value for specific attack type (SFG_WEAPON_FIRE_TYPE_...),
  with added randomness (so the values will differe). For explosion pass
  SFG_WEAPON_FIRE_TYPE_FIREBALL.
*/
uint8_t SFG_getDamageValue(uint8_t attackType)
{
  if (attackType >= SFG_WEAPON_FIRE_TYPES_TOTAL)
    return 0;

  int32_t value = SFG_attackDamageTable[attackType];   // has to be signed
  int32_t maxAdd = (value * SFG_DAMAGE_RANDOMNESS) / 256;

  value = value + (maxAdd / 2) - (SFG_random() * maxAdd / 256);

  if (value < 0)
    value = 0;

  return value;
}

/**
  Returns ammo type for given weapon.
*/
uint8_t SFG_weaponAmmo(uint8_t weapon)
{
  if (weapon == SFG_WEAPON_KNIFE)
    return SFG_AMMO_NONE;
  if (weapon == SFG_WEAPON_MACHINE_GUN ||
      weapon == SFG_WEAPON_SHOTGUN)
    return SFG_AMMO_BULLETS;
  else if (weapon == SFG_WEAPON_ROCKET_LAUNCHER)
    return SFG_AMMO_ROCKETS;
  else
    return SFG_AMMO_PLASMA;
}

RCL_Unit SFG_taxicabDistance(
  RCL_Unit x0, RCL_Unit y0, RCL_Unit z0, RCL_Unit x1, RCL_Unit y1, RCL_Unit z1)
{
  return (RCL_absVal(x0 - x1) + RCL_absVal(y0 - y1) + RCL_absVal(z0 - z1));
}

uint8_t SFG_isInActiveDistanceFromPlayer(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  return SFG_taxicabDistance(
    x,y,z,SFG_player.camera.position.x,SFG_player.camera.position.y,
    SFG_player.camera.height) <= SFG_LEVEL_ELEMENT_ACTIVE_DISTANCE;
}

static inline uint8_t SFG_RCLUnitToZBuffer(RCL_Unit x)
{
  x /= RCL_UNITS_PER_SQUARE;

  uint8_t okay = x < 255;

  return okay * (x + 1) - 1;
}

const uint8_t *SFG_getMonsterSprite(
  uint8_t monsterType, uint8_t state, uint8_t frame)
{
  uint8_t index = 17; // makes the code smaller compared to returning pointers

  if (state != SFG_MONSTER_STATE_DYING)
    switch (monsterType)
    {
      case SFG_LEVEL_ELEMENT_MONSTER_SPIDER:
        switch (state)
        {
          case SFG_MONSTER_STATE_ATTACKING: index = 1; break;
          case SFG_MONSTER_STATE_IDLE: index = 0; break;
          default: index = frame ? 0 : 2; break;
        }
        break;

      case SFG_LEVEL_ELEMENT_MONSTER_WARRIOR:
        index = state != SFG_MONSTER_STATE_ATTACKING ? 6 : 7;
        break;

      case SFG_LEVEL_ELEMENT_MONSTER_DESTROYER:
        switch (state)
        {
          case SFG_MONSTER_STATE_ATTACKING: index = 4; break;
          case SFG_MONSTER_STATE_IDLE: index = 3; break;
          default: index = frame ? 3 : 5; break;
        }
        break;

      case SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT:
        index = state != SFG_MONSTER_STATE_ATTACKING ? 8 : 9;
        break;

      case SFG_LEVEL_ELEMENT_MONSTER_ENDER:
        switch (state)
        {
          case SFG_MONSTER_STATE_ATTACKING: index = 12; break;
          case SFG_MONSTER_STATE_IDLE: index = 10; break;
          default: index = frame ? 10 : 11; break;
        }
        break;

      case SFG_LEVEL_ELEMENT_MONSTER_TURRET:
        switch (state)
        {
          case SFG_MONSTER_STATE_ATTACKING: index = 15; break;
          case SFG_MONSTER_STATE_IDLE: index = 13; break;
          default: index = frame ? 13 : 14; break;
        }
        break;

      case SFG_LEVEL_ELEMENT_MONSTER_EXPLODER:
      default:
        index = 16; 
        break;
    }

  return SFG_monsterSprites[index];
}

/**
  Says whether given key is currently pressed (down). This should be preferred
  to SFG_keyPressed().
*/
uint8_t SFG_keyIsDown(uint8_t key)
{
  return SFG_game.keyStates[key] & 0x01;
}

/**
  Says whether given key has been pressed in the current frame.
*/
uint8_t SFG_keyJustPressed(uint8_t key)
{
  return (SFG_game.keyStates[key] & 0x03) == 1;
}

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

  SFG_game.backgroundScroll =
    ((SFG_player.camera.direction * 8) * SFG_GAME_RESOLUTION_Y)
    / RCL_UNITS_PER_SQUARE; 
}

#if SFG_BACKGROUND_BLUR != 0
uint8_t SFG_backgroundBlurIndex = 0;

SFG_PROGRAM_MEMORY int8_t SFG_backgroundBlurOffsets[9] =
  {0  * SFG_BACKGROUND_BLUR,
   16 * SFG_BACKGROUND_BLUR,
   7  * SFG_BACKGROUND_BLUR,
   17 * SFG_BACKGROUND_BLUR,
   1  * SFG_BACKGROUND_BLUR,
   4  * SFG_BACKGROUND_BLUR,
   15 * SFG_BACKGROUND_BLUR,
   9  * SFG_BACKGROUND_BLUR,
   7  * SFG_BACKGROUND_BLUR
  };
#endif

uint8_t static inline SFG_fogValueDiminish(RCL_Unit depth)
{
  return depth / SFG_FOG_DIMINISH_STEP;
}

void SFG_pixelFunc(RCL_PixelInfo *pixel)
{
  uint8_t color;
  uint8_t shadow = 0;

  if (pixel->position.y == SFG_GAME_RESOLUTION_Y / 2)
  {
    uint8_t zValue = pixel->isWall ? SFG_RCLUnitToZBuffer(pixel->depth) : 255;

    for (uint8_t i = 0; i < SFG_RAYCASTING_SUBSAMPLE; ++i)
      SFG_game.zBuffer[pixel->position.x * SFG_RAYCASTING_SUBSAMPLE + i] = zValue;
  }

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
          SFG_wallTextures[SFG_currentLevel.levelPointer->doorTextureIndex],
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
    uint8_t fogShadow = (pixel->depth * 8) / SFG_FOG_DIMINISH_STEP;

    uint8_t fogShadowPart = fogShadow & 0x07;

    fogShadow /= 8;

    uint8_t xMod4 = pixel->position.x & 0x03;
    uint8_t yMod2 = pixel->position.y & 0x01;

    shadow +=
      fogShadow + SFG_ditheringPatterns[fogShadowPart * 8 + yMod2 * 4 + xMod4];
#else
    shadow += SFG_fogValueDiminish(pixel->depth);
#endif

#if SFG_ENABLE_FOG
    color = palette_minusValue(color,shadow);
#endif
  }
  else
  {
    color = SFG_getTexel(
      SFG_backgroundImages[SFG_currentLevel.backgroundImage],
      SFG_game.backgroundScaleMap[((pixel->position.x 
#if SFG_BACKGROUND_BLUR != 0
        + SFG_backgroundBlurOffsets[SFG_backgroundBlurIndex]
#endif
        ) * SFG_RAYCASTING_SUBSAMPLE + SFG_game.backgroundScroll) % SFG_GAME_RESOLUTION_Y], 
      (SFG_game.backgroundScaleMap[(pixel->position.y          // ^ TODO: get rid of mod?
#if SFG_BACKGROUND_BLUR != 0
        + SFG_backgroundBlurOffsets[SFG_backgroundBlurIndex + 1]
#endif
        ) % SFG_GAME_RESOLUTION_Y ])                                               
      );

#if SFG_BACKGROUND_BLUR != 0
    SFG_backgroundBlurIndex = (SFG_backgroundBlurIndex + 1) % 0x07;
#endif
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

  if ((x0 > x1) || (y0 > y1) || (u0 >= size) || (v0 >= size)) // outside screen?
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

  precompFrom = RCL_max(0,precompFrom);
  precompTo = RCL_min(SFG_MAX_SPRITE_SIZE - 1,precompTo);

  #define PRECOMP_SCALE 2048

  int16_t precompStepScaled = ((SFG_TEXTURE_SIZE - 1) * PRECOMP_SCALE) / size;
  int16_t precompPosScaled = precompFrom * precompStepScaled;

  for (int16_t i = precompFrom; i <= precompTo; ++i)
  {
    SFG_game.spriteSamplingPoints[i] = precompPosScaled / PRECOMP_SCALE;
    precompPosScaled += precompStepScaled;
  }

  #undef PRECOMP_SCALE

  uint8_t zDistance = SFG_RCLUnitToZBuffer(distance);

  for (int16_t x = x0, u = u0; x <= x1; ++x, ++u)
  {
    if (SFG_game.zBuffer[x] >= zDistance)
    {
      int8_t columnTransparent = 1;

      for (int16_t y = y0, v = v0; y <= y1; ++y, ++v)
      {
        uint8_t color =
          SFG_getTexel(image,SFG_game.spriteSamplingPoints[u],
            SFG_game.spriteSamplingPoints[v]);

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
        SFG_game.zBuffer[x] = zDistance;
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
      SFG_game.frameTime - SFG_currentLevel.timeStart);
  }
 
  return SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP -
           doorHeight * SFG_DOOR_HEIGHT_STEP;
}

void SFG_initPlayer()
{
  RCL_initCamera(&SFG_player.camera);

  SFG_player.camera.resolution.x =
    SFG_GAME_RESOLUTION_X / SFG_RAYCASTING_SUBSAMPLE;

  SFG_player.camera.resolution.y = SFG_GAME_RESOLUTION_Y - SFG_HUD_BAR_HEIGHT;

  SFG_player.camera.position.x = RCL_UNITS_PER_SQUARE / 2 +
    SFG_currentLevel.levelPointer->playerStart[0] *  RCL_UNITS_PER_SQUARE;

  SFG_player.camera.position.y = RCL_UNITS_PER_SQUARE / 2 +
    SFG_currentLevel.levelPointer->playerStart[1] *  RCL_UNITS_PER_SQUARE;

  SFG_player.camera.height = 
    SFG_floorHeightAt( 
      SFG_currentLevel.levelPointer->playerStart[0],
      SFG_currentLevel.levelPointer->playerStart[1]);

  SFG_player.camera.direction = 
    SFG_currentLevel.levelPointer->playerStart[2] * 4;

  SFG_recompurePLayerDirection(); 
 
  SFG_player.previousVerticalSpeed = 0;

  SFG_player.headBobFrame = 0;

  SFG_player.weapon = 2;

  SFG_player.weaponCooldownStartFrame = SFG_game.frame;
  SFG_player.lastHurtFrame = SFG_game.frame;
  SFG_player.lastItemTakenFrame = SFG_game.frame;

  SFG_player.health = SFG_PLAYER_MAX_HEALTH;

  SFG_player.cards = 0;

  SFG_player.justTeleported = 0;

  for (uint8_t i = 0; i < SFG_AMMO_TOTAL; ++i)
    SFG_player.ammo[i] = 0;
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
      SFG_game.frameTime - SFG_currentLevel.timeStart);
}

void SFG_setAndInitLevel(const SFG_Level *level)
{
  SFG_LOG("setting and initializing level");

  SFG_game.currentRandom = 0;

  SFG_currentLevel.backgroundImage = level->backgroundImage;

  SFG_currentLevel.levelPointer = level;

  SFG_currentLevel.floorColor = level->floorColor;
  SFG_currentLevel.ceilingColor = level->ceilingColor;

  for (uint8_t i = 0; i < 7; ++i)
    SFG_currentLevel.textures[i] =
      SFG_wallTextures[level->textureIndices[i]];

  SFG_LOG("initializing doors");

  SFG_currentLevel.checkedDoorIndex = 0;

  SFG_currentLevel.doorRecordCount = 0;

  SFG_currentLevel.projectileRecordCount = 0;

  SFG_currentLevel.teleportCount = 0;

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
      {
        SFG_LOG("warning: too many doors!");
        break;
      }
    }

    if (SFG_currentLevel.doorRecordCount >= SFG_MAX_DOORS)
      break;
  }

  SFG_LOG("initializing level elements");

  SFG_currentLevel.itemRecordCount = 0;
  SFG_currentLevel.checkedItemIndex = 0;

  SFG_currentLevel.monsterRecordCount = 0;
  SFG_currentLevel.checkedMonsterIndex = 0;

  SFG_MonsterRecord *monster;

  for (uint8_t i = 0; i < SFG_MAX_LEVEL_ELEMENTS; ++i)
  {
    const SFG_LevelElement *e = &(SFG_currentLevel.levelPointer->elements[i]);

    if (e->type != SFG_LEVEL_ELEMENT_NONE)
    {
      if (SFG_LEVEL_ELEMENT_TYPE_IS_MOSTER(e->type))
      {
        SFG_LOG("adding monster");

        monster =
        &(SFG_currentLevel.monsterRecords[SFG_currentLevel.monsterRecordCount]);

        monster->stateType = e->type | SFG_MONSTER_STATE_INACTIVE;
        monster->health = SFG_GET_MONSTER_MAX_HEALTH(SFG_MONSTER_TYPE_TO_INDEX(e->type));

        monster->coords[0] = e->coords[0] * 4 + 2;
        monster->coords[1] = e->coords[1] * 4 + 2;

        SFG_currentLevel.monsterRecordCount++;
      }
      else if ((e->type < SFG_LEVEL_ELEMENT_LOCK0) ||
        (e->type > SFG_LEVEL_ELEMENT_LOCK2))
      {
        SFG_LOG("adding item");
        SFG_currentLevel.itemRecords[SFG_currentLevel.itemRecordCount] = i;
        SFG_currentLevel.itemRecordCount++;

        if (e->type == SFG_LEVEL_ELEMENT_TELEPORT)
          SFG_currentLevel.teleportCount++;
      }
      else
      {
        SFG_LOG("adding door lock");
      
        uint8_t properties;
     
        SFG_getMapTile(level,e->coords[0],e->coords[1],&properties);

        if ((properties & SFG_TILE_PROPERTY_MASK) == SFG_TILE_PROPERTY_DOOR)
        {
          // find the door record and lock the door:
          for (uint16_t j = 0; j < SFG_currentLevel.doorRecordCount; ++j)
          {
            SFG_DoorRecord *d = &(SFG_currentLevel.doorRecords[j]);

            if (d->coords[0] == e->coords[0] && d->coords[1] == e->coords[1])
            {
              d->state |= (e->type - SFG_LEVEL_ELEMENT_LOCK0 + 1) << 6;
              break;
            }
          }
        }
        else
        {
          SFG_LOG("warning: lock not put on door tile!");
        }
      }
    }
  } 

  SFG_currentLevel.timeStart = SFG_getTimeMs(); 
  SFG_currentLevel.frameStart = SFG_game.frame;

  SFG_game.spriteAnimationFrame = 0;
 
  SFG_initPlayer();
}

void SFG_init()
{
  SFG_LOG("initializing game")

  SFG_game.frame = 0;

  SFG_game.currentRandom = 0;

  RCL_initRayConstraints(&SFG_game.rayConstraints);

  SFG_game.rayConstraints.maxHits = SFG_RAYCASTING_MAX_HITS;
  SFG_game.rayConstraints.maxSteps = SFG_RAYCASTING_MAX_STEPS;

  for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_Y; ++i)
    SFG_game.backgroundScaleMap[i] =
      (i * SFG_TEXTURE_SIZE) / SFG_GAME_RESOLUTION_Y;

  for (uint8_t i = 0; i < SFG_KEY_COUNT; ++i)
    SFG_game.keyStates[i] = 0;

  SFG_game.backgroundScroll = 0;

  SFG_setAndInitLevel(&SFG_level0);

  SFG_game.lastFrameTimeMs = SFG_getTimeMs();

  SFG_player.freeLook = 0;
}

void SFG_getPlayerWeaponInfo(
  uint8_t *ammoType, uint8_t *projectileCount, uint8_t *canShoot)
{
  *ammoType = SFG_weaponAmmo(SFG_player.weapon);

  *projectileCount = SFG_GET_WEAPON_PROJECTILE_COUNT(SFG_player.weapon);

#if SFG_INFINITE_AMMO
  *canShoot = 1;
#else
  *canShoot = 
    (*ammoType == SFG_AMMO_NONE || 
     SFG_player.ammo[*ammoType] >= *projectileCount);
#endif
}

void SFG_playerRotateWeapon(uint8_t next)
{
  RCL_Unit initialWeapon = SFG_player.weapon;
  RCL_Unit increment = next ? 1 : -1;

  while (1)
  {
    SFG_player.weapon = 
      RCL_wrap(SFG_player.weapon + increment,SFG_WEAPONS_TOTAL);

    if (SFG_player.weapon == initialWeapon)
      break;

    uint8_t ammo, projectileCount, canShoot;

    SFG_getPlayerWeaponInfo(&ammo,&projectileCount,&canShoot);
 
    if (canShoot)
      break;
  }
}

/**
  Adds new projectile to the current level, return 1 if added, 0 if not (max
  count reached).
*/
uint8_t SFG_createProjectile(SFG_ProjectileRecord projectile)
{
  if (SFG_currentLevel.projectileRecordCount >= SFG_MAX_PROJECTILES)
    return 0; 

  SFG_currentLevel.projectileRecords[SFG_currentLevel.projectileRecordCount] =
    projectile;
  
  SFG_currentLevel.projectileRecordCount++;

  return 1;
}

/**
  Launches projectile of given type from given position in given direction
  (has to be normalized), with given offset (so as to not collide with the
  shooting entity). Returns the same value as SFG_createProjectile.
*/
uint8_t SFG_launchProjectile(
  uint8_t type,   
  RCL_Vector2D shootFrom,
  RCL_Unit shootFromHeight,
  RCL_Vector2D direction,
  RCL_Unit verticalSpeed,
  RCL_Unit offsetDistance
  )
{
  if (type == SFG_PROJECTILE_NONE)
    return 0;

  SFG_ProjectileRecord p;

  p.type = type;
  p.doubleFramesToLive = 
    RCL_nonZero(SFG_GET_PROJECTILE_FRAMES_TO_LIVE(type) / 2);
  
  p.position[0] =
    shootFrom.x + (direction.x * offsetDistance) / RCL_UNITS_PER_SQUARE;

  p.position[1] = 
    shootFrom.y + (direction.y * offsetDistance) / RCL_UNITS_PER_SQUARE; 

  p.position[2] = shootFromHeight;

  p.direction[0] = 
    (direction.x * SFG_GET_PROJECTILE_SPEED_UPS(type)) / RCL_UNITS_PER_SQUARE;

  p.direction[1] =
    (direction.y * SFG_GET_PROJECTILE_SPEED_UPS(type)) / RCL_UNITS_PER_SQUARE;

  p.direction[2] = verticalSpeed;

  return SFG_createProjectile(p);
}

/**
  Pushes a given position away from a center by given distance, with collisions.
  Returns 1 if push away happened, otherwise 0.
*/
uint8_t SFG_pushAway(
  RCL_Unit pos[3],
  RCL_Unit centerX,
  RCL_Unit centerY,
  RCL_Unit preferredDirection,
  RCL_Unit distance)
{
  RCL_Vector2D fromCenter;

  fromCenter.x = pos[0] - centerX;
  fromCenter.y = pos[1] - centerY;

  RCL_Unit l = RCL_len(fromCenter);

  if (l < 128)
  {
    fromCenter = RCL_angleToDirection(preferredDirection);
    l = RCL_UNITS_PER_SQUARE;
  }
  else if (l >= distance)
  {
    return 0;
  }

  RCL_Vector2D offset;

  offset.x = (fromCenter.x * distance) / l; 
  offset.y = (fromCenter.y * distance) / l; 

  RCL_Camera c;

  RCL_initCamera(&c);

  c.position.x = pos[0];
  c.position.y = pos[1];
  c.height = pos[2];

  RCL_moveCameraWithCollision(&c,offset,0,SFG_floorHeightAt,
    SFG_ceilingHeightAt,1,1);

  pos[0] = c.position.x;
  pos[1] = c.position.y;
  pos[2] = c.height;

  return 1;
}

uint8_t SFG_pushPlayerAway(RCL_Unit centerX, RCL_Unit centerY, RCL_Unit distance)
{
  RCL_Unit p[3];

  p[0] = SFG_player.camera.position.x; 
  p[1] = SFG_player.camera.position.y; 
  p[2] = SFG_player.camera.height; 

  uint8_t result = SFG_pushAway(p,centerX,centerY,
    SFG_player.camera.direction - RCL_UNITS_PER_SQUARE / 2,
    distance);

  SFG_player.camera.position.x = p[0]; 
  SFG_player.camera.position.y = p[1]; 
  SFG_player.camera.height = p[2];

  return result;
}

/**
  Helper function to resolve collision with level element. The function supposes
  the collision already does happen and only resolves it. Returns adjusted move
  offset.
*/
RCL_Vector2D SFG_resolveCollisionWithElement(
  RCL_Vector2D position, RCL_Vector2D moveOffset, RCL_Vector2D elementPos)
{
  RCL_Unit dx = RCL_absVal(elementPos.x - position.x);
  RCL_Unit dy = RCL_absVal(elementPos.y - position.y);

  if (dx > dy)
  {
    // colliding from left/right

    if ((moveOffset.x > 0) == (position.x < elementPos.x))
      moveOffset.x = 0;
      // ^ only stop if heading towards element, to avoid getting stuck
  }
  else
  {
    // colliding from up/down

    if ((moveOffset.y > 0) == (position.y < elementPos.y))
      moveOffset.y = 0;
  }

  return moveOffset;  
}

/**
  Adds or substracts player's health, which either hurts him (negative value)
  or heals him (positive value).
*/
void SFG_playerChangeHealth(int8_t healthAdd)
{
  int16_t health = SFG_player.health;
  health += healthAdd;
  health = RCL_clamp(health,0,SFG_PLAYER_MAX_HEALTH);

  SFG_player.health = health;

  if (healthAdd < 0)
    SFG_player.lastHurtFrame = SFG_game.frame;
}

/**
  Same as SFG_playerChangeHealth but for monsters.
*/
void SFG_monsterChangeHealth(SFG_MonsterRecord *monster, int8_t healthAdd)
{
  int16_t health = monster->health;
  health += healthAdd;
  health = RCL_clamp(health,0,255);
  monster->health = health;
}

void SFG_removeItem(uint8_t index)
{
  SFG_LOG("removing item");

  for (uint16_t j = index; j < SFG_currentLevel.itemRecordCount - 1; ++j)
    SFG_currentLevel.itemRecords[j] =
      SFG_currentLevel.itemRecords[j + 1];

  SFG_currentLevel.itemRecordCount--; 
}
  
void SFG_createExplosion(RCL_Unit, RCL_Unit, RCL_Unit); // forward decl

void SFG_explodeBarrel(uint8_t itemIndex, RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  SFG_removeItem(itemIndex);
  SFG_createExplosion(x,y,z);
}

uint8_t SFG_distantSoundVolume(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  RCL_Unit distance = SFG_taxicabDistance(x,y,z,
                        SFG_player.camera.position.x,
                        SFG_player.camera.position.y,
                        SFG_player.camera.height);

  if (distance >= SFG_SFX_MAX_DISTANCE)
    return 0;

  return 255 - (distance * 255) / SFG_SFX_MAX_DISTANCE;
}

void SFG_createExplosion(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  SFG_ProjectileRecord explosion;

  SFG_playSound(2,SFG_distantSoundVolume(x,y,z));

  explosion.type = SFG_PROJECTILE_EXPLOSION;

  explosion.position[0] = x;
  explosion.position[1] = y;
  explosion.position[2] = z;

  explosion.direction[0] = 0;
  explosion.direction[1] = 0;
  explosion.direction[2] = 0;

  explosion.doubleFramesToLive = RCL_nonZero(
    SFG_GET_PROJECTILE_FRAMES_TO_LIVE(SFG_PROJECTILE_EXPLOSION) / 2);

  SFG_createProjectile(explosion);

  uint8_t damage = SFG_getDamageValue(SFG_WEAPON_FIRE_TYPE_FIREBALL);

  if (SFG_pushPlayerAway(x,y,SFG_EXPLOSION_DISTANCE))
    SFG_playerChangeHealth(-1 * damage);

  for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
  {
    SFG_MonsterRecord *monster = &(SFG_currentLevel.monsterRecords[i]);

    if (SFG_MR_STATE(*monster) == SFG_MONSTER_STATE_INACTIVE)
      continue; 

    RCL_Unit monsterHeight =
      SFG_floorHeightAt(
        SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
        SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1]))
        + RCL_UNITS_PER_SQUARE / 2;

    if (SFG_taxicabDistance(
      SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
      SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),monsterHeight,
      x,y,z) <= SFG_EXPLOSION_DISTANCE)
    {
      SFG_monsterChangeHealth(monster,
        -1 * SFG_getDamageValue(SFG_WEAPON_FIRE_TYPE_FIREBALL));
    }
  }

  // explode nearby barrels

  if (damage >= SFG_BARREL_EXPLOSION_DAMAGE_THRESHOLD)
    for (uint16_t i = 0; i < SFG_currentLevel.itemRecordCount; ++i)
    {
      SFG_ItemRecord item = SFG_currentLevel.itemRecords[i];

      /* We DON'T check just active barrels but all, otherwise it looks weird
         that out of sight barrels in a line didn't explode.*/

      SFG_LevelElement element = SFG_ITEM_RECORD_LEVEL_ELEMENT(item);

      if (element.type != SFG_LEVEL_ELEMENT_BARREL)
        continue;

      RCL_Unit elementX =
        element.coords[0] * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2;

      RCL_Unit elementY =
        element.coords[1] * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2;

      RCL_Unit elementHeight =
        SFG_floorHeightAt(element.coords[0],element.coords[1]);

      if (SFG_taxicabDistance(
        x,y,z,elementX,elementY,elementHeight) <= SFG_EXPLOSION_DISTANCE)
      {
        SFG_explodeBarrel(i,elementX,elementY,elementHeight);
        i--;
      }
    }
}

void SFG_createDust(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  SFG_ProjectileRecord dust;

  dust.type = SFG_PROJECTILE_DUST;

  dust.position[0] = x;
  dust.position[1] = y;
  dust.position[2] = z;

  dust.direction[0] = 0;
  dust.direction[1] = 0;
  dust.direction[2] = 0;

  dust.doubleFramesToLive =
    RCL_nonZero(SFG_GET_PROJECTILE_FRAMES_TO_LIVE(SFG_PROJECTILE_DUST) / 2);

  SFG_createProjectile(dust);
}

void SFG_getMonsterWorldPosition(SFG_MonsterRecord *monster, RCL_Unit *x,
  RCL_Unit *y, RCL_Unit *z)
{
  *x = SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]);
  *y = SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]);
  *z = SFG_floorHeightAt(
         SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
         SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1]))
       + RCL_UNITS_PER_SQUARE / 2;
}

void SFG_monsterPerformAI(SFG_MonsterRecord *monster)
{
  uint8_t state = SFG_MR_STATE(*monster);
  uint8_t type = SFG_MR_TYPE(*monster);
  uint8_t monsterNumber = SFG_MONSTER_TYPE_TO_INDEX(type);
  uint8_t attackType = SFG_GET_MONSTER_ATTACK_TYPE(monsterNumber);

  int8_t coordAdd[2];

  coordAdd[0] = 0;
  coordAdd[1] = 0;

  uint8_t notRanged =
    (attackType == SFG_MONSTER_ATTACK_MELEE) || 
    (attackType == SFG_MONSTER_ATTACK_EXPLODE); 

  if ( // sometimes randomly attack
       !notRanged &&
       (SFG_random() < 
       SFG_GET_MONSTER_AGGRESSIVITY(SFG_MONSTER_TYPE_TO_INDEX(type)))
     )
  { 
    if (!notRanged && (SFG_random() % 4 != 0))
    {
      // attack
 
      state = SFG_MONSTER_STATE_ATTACKING;

      if (type != SFG_LEVEL_ELEMENT_MONSTER_WARRIOR)
      {
        RCL_Vector2D pos;
        RCL_Vector2D dir;

        pos.x = SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]);
        pos.y = SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]);

        dir.x = SFG_player.camera.position.x - pos.x;
        dir.y = SFG_player.camera.position.y - pos.y;

        dir = RCL_normalize(dir);

        uint8_t projectile;  

        switch (SFG_GET_MONSTER_ATTACK_TYPE(monsterNumber))
        {
          case SFG_MONSTER_ATTACK_FIREBALL:
            projectile = SFG_PROJECTILE_FIREBALL; 
            break;

          case SFG_MONSTER_ATTACK_BULLET:
            projectile = SFG_PROJECTILE_BULLET; 
            break;

          case SFG_MONSTER_ATTACK_PLASMA:
            projectile = SFG_PROJECTILE_PLASMA;
            break;

          case SFG_MONSTER_ATTACK_FIREBALL_BULLET:
            projectile = (SFG_random() < 128) ?
              SFG_PROJECTILE_FIREBALL : 
              SFG_PROJECTILE_BULLET;
            break;

          default:
            projectile = SFG_PROJECTILE_NONE; 
            break;
        }

        if (projectile == SFG_PROJECTILE_BULLET)
          SFG_playSoundSafe(0,
            SFG_distantSoundVolume( 
              SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
              SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),
              SFG_floorHeightAt(
                SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
                SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1])
                )
              )
            );

        SFG_launchProjectile(
          projectile,
          pos,
          SFG_floorHeightAt(
             SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
             SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1])
             ) + RCL_UNITS_PER_SQUARE / 2,
          dir,
          0,
          SFG_PROJECTILE_SPAWN_OFFSET
        );
      }
    }
    else
      state = SFG_MONSTER_STATE_IDLE;
  }
  else if (state == SFG_MONSTER_STATE_IDLE)
  {
    if (notRanged)
    {
      // non-ranged monsters walk towards player

      uint8_t mX = SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]);
      uint8_t mY = SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1]);

      RCL_Unit pX, pY, pZ;
      SFG_getMonsterWorldPosition(monster,&pX,&pY,&pZ);

      uint8_t isClose = // close to player?
        SFG_taxicabDistance(pX,pY,pZ,
          SFG_player.camera.position.x,
          SFG_player.camera.position.y,
          SFG_player.camera.height) <= SFG_MELEE_RANGE;

      if (!isClose)
      {
        // walk towards player

        if (mX > SFG_player.squarePosition[0])
        {
          if (mY > SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_NW;
          else if (mY < SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_SW;
          else
            state = SFG_MONSTER_STATE_GOING_W;
        }
        else if (mX < SFG_player.squarePosition[0])
        {
          if (mY > SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_NE;
          else if (mY < SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_SE;
          else
            state = SFG_MONSTER_STATE_GOING_E;
        }
        else
        {
          if (mY > SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_N;
          else if (mY < SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_S;
        }
      }
      else // is close
      {
        // melee, close-up attack

        if (attackType == SFG_MONSTER_ATTACK_MELEE)
        {
          // melee attack

          state = SFG_MONSTER_STATE_ATTACKING;

          SFG_playerChangeHealth(
            -1 * SFG_getDamageValue(SFG_WEAPON_FIRE_TYPE_MELEE)); 
              
          SFG_playSoundSafe(3,255);
        }
        else // SFG_MONSTER_ATTACK_EXPLODE
        {
          // explode

          uint8_t properties;

          SFG_TileDefinition tile =
            SFG_getMapTile(SFG_currentLevel.levelPointer,mX,mY,&properties);
         
          SFG_createExplosion(mX * RCL_UNITS_PER_SQUARE ,mY * RCL_UNITS_PER_SQUARE,
          SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP + SFG_WALL_HEIGHT_STEP);

          monster->health = 0;
        }
      }
    }
    else // ranged monsters
    {
      // choose walk direction randomly

      switch (SFG_random() % 8)
      {
        case 0: state = SFG_MONSTER_STATE_GOING_E; break;
        case 1: state = SFG_MONSTER_STATE_GOING_W; break;
        case 2: state = SFG_MONSTER_STATE_GOING_N; break;
        case 3: state = SFG_MONSTER_STATE_GOING_S; break;
        case 4: state = SFG_MONSTER_STATE_GOING_NE; break;
        case 5: state = SFG_MONSTER_STATE_GOING_NW; break;
        case 6: state = SFG_MONSTER_STATE_GOING_SE; break;
        case 7: state = SFG_MONSTER_STATE_GOING_SW; break;
        default: break;
      }
    }
  }
  else if (state == SFG_MONSTER_STATE_ATTACKING)
  {
    state = SFG_MONSTER_STATE_IDLE;
  }
  else
  {
    int8_t add = 1;

    if (attackType == SFG_MONSTER_ATTACK_MELEE)
      add = 2;
    else if (attackType == SFG_MONSTER_ATTACK_EXPLODE)
      add = 3;

    if (state == SFG_MONSTER_STATE_GOING_E ||
        state == SFG_MONSTER_STATE_GOING_NE ||
        state == SFG_MONSTER_STATE_GOING_SE)
      coordAdd[0] = add;
    else if (state == SFG_MONSTER_STATE_GOING_W ||
        state == SFG_MONSTER_STATE_GOING_SW ||
        state == SFG_MONSTER_STATE_GOING_NW)
      coordAdd[0] = -1 * add;

    if (state == SFG_MONSTER_STATE_GOING_N ||
        state == SFG_MONSTER_STATE_GOING_NE ||
        state == SFG_MONSTER_STATE_GOING_NW)
      coordAdd[1] = -1 * add;
    else if (state == SFG_MONSTER_STATE_GOING_S ||
        state == SFG_MONSTER_STATE_GOING_SE ||
        state == SFG_MONSTER_STATE_GOING_SW)
      coordAdd[1] = add;

    if (add)
      state = SFG_MONSTER_STATE_IDLE;
  }

  int16_t newPos[2];

  newPos[0] = monster->coords[0] + coordAdd[0];
  newPos[1] = monster->coords[1] + coordAdd[1];

  int8_t collision = 0;

  if (newPos[0] < 0 || newPos[0] >= 256 || newPos[1] < 0 || newPos[1] >= 256)
  {
    collision = 1;
  }
  else
  {
    RCL_Unit currentHeight =
      SFG_floorHeightAt(monster->coords[0] / 4,monster->coords[1] / 4);

    RCL_Unit newHeight =
      SFG_floorHeightAt(newPos[0] / 4,newPos[1] / 4);

    collision =
      RCL_absVal(currentHeight - newHeight) > RCL_CAMERA_COLL_STEP_HEIGHT;
  }

  if (collision)
  {
    state = SFG_MONSTER_STATE_IDLE;
    // ^ will force the monster to choose random direction in next update
 
    newPos[0] = monster->coords[0];
    newPos[1] = monster->coords[1];
  }

  monster->stateType = state | type;
  monster->coords[0] = newPos[0];
  monster->coords[1] = newPos[1];;
}

/**
  Helper function, returns a pointer to level element representing item with
  given index, but only if the item is active (otherwise 0 is returned).
*/
static inline const SFG_LevelElement *SFG_getActiveItemElement(uint8_t index)
{
  SFG_ItemRecord item = SFG_currentLevel.itemRecords[index];

  if ((item & SFG_ITEM_RECORD_ACTIVE_MASK) == 0)
    return 0;

  return &(SFG_currentLevel.levelPointer->elements[item &
           ~SFG_ITEM_RECORD_ACTIVE_MASK]);
}

static inline uint8_t SFG_elementCollides(
  RCL_Unit pointX,
  RCL_Unit pointY,
  RCL_Unit pointZ,
  RCL_Unit elementX,
  RCL_Unit elementY,
  RCL_Unit elementHeight
)
{
  return
    SFG_taxicabDistance(pointX,pointY,pointZ,elementX,elementY,elementHeight)
    <= SFG_ELEMENT_COLLISION_DISTANCE;
}

/**
  Checks collision of a projectile with level element at given position.
*/
uint8_t SFG_projectileCollides(SFG_ProjectileRecord *projectile,
  RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  if (!SFG_elementCollides(x,y,z,
    projectile->position[0],projectile->position[1],projectile->position[2]))
    return 0;

  if ((projectile->type == SFG_PROJECTILE_EXPLOSION) ||
      (projectile->type == SFG_PROJECTILE_DUST))
    return 0;

  /* For directional projectiles we only register a collision if its direction
     is "towards" the element so that the shooter doesn't get shot by his own
     projectile. */

  RCL_Vector2D projDir, toElement;

  projDir.x = projectile->direction[0]; 
  projDir.y = projectile->direction[1];

  toElement.x = x - projectile->position[0];
  toElement.y = y - projectile->position[1];
   
  return RCL_vectorsAngleCos(projDir,toElement) >= 0;
}

void SFG_getLevelElementSprite(
  uint8_t elementType, uint8_t *spriteIndex, uint8_t *spriteSize)
{
  *spriteSize = 0;
  *spriteIndex = elementType - 1;

  switch (elementType)
  {
    case SFG_LEVEL_ELEMENT_TREE:
      *spriteSize = 2;
      break;

    case SFG_LEVEL_ELEMENT_TELEPORT:
    case SFG_LEVEL_ELEMENT_FINISH:
      *spriteSize = 3;
      break;

    case SFG_LEVEL_ELEMENT_CARD0:
    case SFG_LEVEL_ELEMENT_CARD1:
    case SFG_LEVEL_ELEMENT_CARD2:
      *spriteIndex = SFG_LEVEL_ELEMENT_CARD0 - 1;
      break;

    default:
      break;
  }
}

/**
  Performs one game step (logic, physics), happening SFG_MS_PER_FRAME after
  previous frame. 
*/
void SFG_gameStep()
{
  SFG_game.explosionSoundPlayed = 0;

  for (uint8_t i = 0; i < SFG_KEY_COUNT; ++i)
    SFG_game.keyStates[i] = (SFG_game.keyStates[i] << 1) | SFG_keyPressed(i);

  if ((SFG_currentLevel.frameStart - SFG_game.frame) %
      SFG_SPRITE_ANIMATION_FRAME_DURATION == 0)
    SFG_game.spriteAnimationFrame++;

  int8_t recomputeDirection = 0;

  RCL_Vector2D moveOffset;

  moveOffset.x = 0;
  moveOffset.y = 0;

  int8_t strafe = 0;

#if SFG_HEADBOB_ENABLED
  int8_t bobbing = 0;
#endif

  int8_t shearing = 0;

  if (SFG_keyJustPressed(SFG_KEY_TOGGLE_FREELOOK))
    SFG_player.freeLook = !SFG_player.freeLook;

#if SFG_PREVIEW_MODE == 0
  if (
    SFG_keyIsDown(SFG_KEY_B) &&
    !SFG_keyIsDown(SFG_KEY_C) &&
    (SFG_game.frame - SFG_player.weaponCooldownStartFrame >
    SFG_GET_WEAPON_FIRE_COOLDOWN_FRAMES(SFG_player.weapon)))
  {
    // player: attack, shoot, fire

    uint8_t ammo, projectileCount, canShoot;

    SFG_getPlayerWeaponInfo(&ammo,&projectileCount,&canShoot);

    if (canShoot)
    {
      uint8_t sound;

      switch (SFG_player.weapon)
      {
        case SFG_WEAPON_KNIFE:           sound = 255; break;
        case SFG_WEAPON_ROCKET_LAUNCHER: 
        case SFG_WEAPON_SHOTGUN:         sound = 2; break;
        case SFG_WEAPON_PLASMAGUN:
        case SFG_WEAPON_SOLUTION:        sound = 4; break;
        default:                         sound = 0; break;
      }

      if (sound != 255)
        SFG_playSoundSafe(sound,255);

      if (SFG_player.weapon != SFG_WEAPON_KNIFE)
        SFG_playSoundSafe(
          (SFG_player.weapon == SFG_WEAPON_SHOTGUN ||
           SFG_player.weapon == SFG_WEAPON_MACHINE_GUN) ? 0 : 4,255);

      if (ammo != SFG_AMMO_NONE)
        SFG_player.ammo[ammo] -= projectileCount;

      uint8_t projectile;

      switch (SFG_GET_WEAPON_FIRE_TYPE(SFG_player.weapon))
      {
        case SFG_WEAPON_FIRE_TYPE_PLASMA:
          projectile = SFG_PROJECTILE_PLASMA;
          break;

        case SFG_WEAPON_FIRE_TYPE_FIREBALL:
          projectile = SFG_PROJECTILE_FIREBALL;
          break;

        case SFG_WEAPON_FIRE_TYPE_BULLET:
          projectile = SFG_PROJECTILE_BULLET;
          break;

        case SFG_WEAPON_FIRE_TYPE_MELEE:
          projectile = SFG_PROJECTILE_NONE;
          break;

        default:
          projectile = 255;
          break;
      }
          
      if (projectile != SFG_PROJECTILE_NONE)
      {
        uint16_t angleAdd = SFG_PROJECTILE_SPREAD_ANGLE / (projectileCount + 1);

        RCL_Unit direction =
          (SFG_player.camera.direction - SFG_PROJECTILE_SPREAD_ANGLE / 2) 
          + angleAdd;

        for (uint8_t i = 0; i < projectileCount; ++i)
        {
          SFG_launchProjectile(
            projectile,
            SFG_player.camera.position,
            SFG_player.camera.height,
            RCL_angleToDirection(direction),
            (SFG_player.camera.shear *
              SFG_GET_PROJECTILE_SPEED_UPS(projectile)) / 
              SFG_CAMERA_MAX_SHEAR_PIXELS,
              SFG_PROJECTILE_SPAWN_OFFSET
            );

          direction += angleAdd;
        }
      }
      else
      {
        // player's melee attack

        for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
        {
          SFG_MonsterRecord *m = &(SFG_currentLevel.monsterRecords[i]);

          if (SFG_MR_STATE(*m) == SFG_MONSTER_STATE_INACTIVE)
            continue;

          RCL_Unit pX, pY, pZ;
          SFG_getMonsterWorldPosition(m,&pX,&pY,&pZ);

          if (SFG_taxicabDistance(pX,pY,pZ,
              SFG_player.camera.position.x,
              SFG_player.camera.position.y,
              SFG_player.camera.height) > SFG_MELEE_RANGE)
            continue;
   
            RCL_Vector2D toMonster;

            toMonster.x = pX - SFG_player.camera.position.x;
            toMonster.y = pY - SFG_player.camera.position.y;

            if (RCL_vectorsAngleCos(SFG_player.direction,toMonster) >=
                (RCL_UNITS_PER_SQUARE - SFG_PLAYER_MELEE_ANGLE))
            {
              SFG_monsterChangeHealth(m,
                -1 * SFG_getDamageValue(SFG_WEAPON_FIRE_TYPE_MELEE));

              SFG_createDust(pX,pY,pZ);

              SFG_playSoundSafe(3,255);

              break;
            }
        }
      }

      SFG_player.weaponCooldownStartFrame = SFG_game.frame;

      if (ammo != SFG_AMMO_NONE && SFG_player.ammo[ammo] == 0)
        SFG_playerRotateWeapon(1);
    } // endif: has enough ammo?
  } // attack
#endif // SFG_PREVIEW_MODE == 0

  if (SFG_keyJustPressed(SFG_KEY_NEXT_WEAPON))
    SFG_playerRotateWeapon(1);
  else if (SFG_keyJustPressed(SFG_KEY_PREVIOUS_WEAPON))
    SFG_playerRotateWeapon(0);

  if (SFG_keyIsDown(SFG_KEY_A))
  {
    if (SFG_keyIsDown(SFG_KEY_UP))
    {
      SFG_player.camera.shear =
        RCL_min(SFG_CAMERA_MAX_SHEAR_PIXELS,
                SFG_player.camera.shear + SFG_CAMERA_SHEAR_STEP_PER_FRAME);

      shearing = 1;
    }
    else if (SFG_keyIsDown(SFG_KEY_DOWN))
    {
      SFG_player.camera.shear =
        RCL_max(-1 * SFG_CAMERA_MAX_SHEAR_PIXELS,
                SFG_player.camera.shear - SFG_CAMERA_SHEAR_STEP_PER_FRAME);

      shearing = 1;
    }

    if (!SFG_keyIsDown(SFG_KEY_C))
    {
      if (SFG_keyIsDown(SFG_KEY_LEFT))
        strafe = -1;
      else if (SFG_keyIsDown(SFG_KEY_RIGHT))
        strafe = 1;
    }
    else
    {
      if (SFG_keyJustPressed(SFG_KEY_LEFT) || SFG_keyJustPressed(SFG_KEY_A))
        SFG_playerRotateWeapon(0);
      else if (SFG_keyJustPressed(SFG_KEY_RIGHT) || SFG_keyJustPressed(SFG_KEY_B))
        SFG_playerRotateWeapon(1);
    }
  }
  else
  {
    if (!SFG_keyIsDown(SFG_KEY_C))
    {
      if (SFG_keyIsDown(SFG_KEY_LEFT))
      {
        SFG_player.camera.direction -= SFG_PLAYER_TURN_UNITS_PER_FRAME;
        recomputeDirection = 1;
      }
      else if (SFG_keyIsDown(SFG_KEY_RIGHT))
      {
        SFG_player.camera.direction += SFG_PLAYER_TURN_UNITS_PER_FRAME;
        recomputeDirection = 1;
      }
    }
    else
    {
      if (SFG_keyJustPressed(SFG_KEY_LEFT) | SFG_keyJustPressed(SFG_KEY_A))
        SFG_playerRotateWeapon(0);
      else if (SFG_keyJustPressed(SFG_KEY_RIGHT) | SFG_keyJustPressed(SFG_KEY_B))
        SFG_playerRotateWeapon(1);
    }

    int16_t mouseX, mouseY;

    SFG_getMouseOffset(&mouseX,&mouseY);

    if (mouseX != 0 || mouseY != 0)
    {
      SFG_player.camera.direction += 
        (mouseX * SFG_MOUSE_SENSITIVITY_HORIZONTAL) / 128;

      if (SFG_player.freeLook)
        SFG_player.camera.shear =
          RCL_max(RCL_min(
            SFG_player.camera.shear - (mouseY * SFG_MOUSE_SENSITIVITY_VERTICAL)
            / 128,
            SFG_CAMERA_MAX_SHEAR_PIXELS),
            -1 * SFG_CAMERA_MAX_SHEAR_PIXELS);
 
      recomputeDirection = 1;
    }

    if (recomputeDirection)
      SFG_recompurePLayerDirection();

    if (SFG_keyIsDown(SFG_KEY_UP))
    {
      moveOffset.x += SFG_player.direction.x;
      moveOffset.y += SFG_player.direction.y;
#if SFG_HEADBOB_ENABLED
      bobbing = 1;
#endif
    }
    else if (SFG_keyIsDown(SFG_KEY_DOWN))
    {
      moveOffset.x -= SFG_player.direction.x;
      moveOffset.y -= SFG_player.direction.y;
#if SFG_HEADBOB_ENABLED
      bobbing = 1;
#endif
    }
  }

  if (SFG_keyIsDown(SFG_KEY_STRAFE_LEFT))
    strafe = -1;
  else if (SFG_keyIsDown(SFG_KEY_STRAFE_RIGHT))
    strafe = 1;

  if (strafe != 0)
  {
    moveOffset.x += strafe * SFG_player.direction.y;
    moveOffset.y -= strafe * SFG_player.direction.x;
  }

#if SFG_PREVIEW_MODE
  if (SFG_keyIsDown(SFG_KEY_B))
    SFG_player.verticalSpeed = SFG_PLAYER_MOVE_UNITS_PER_FRAME;
  else if (SFG_keyIsDown(SFG_KEY_C))
    SFG_player.verticalSpeed = -1 * SFG_PLAYER_MOVE_UNITS_PER_FRAME;
  else
    SFG_player.verticalSpeed = 0;
#else
  RCL_Unit verticalOffset = 
    (  
      (
        SFG_keyIsDown(SFG_KEY_JUMP) ||
        (SFG_keyIsDown(SFG_KEY_UP) && SFG_keyIsDown(SFG_KEY_C))
      ) &&
      (SFG_player.verticalSpeed == 0) &&
      (SFG_player.previousVerticalSpeed == 0)) ?
    SFG_PLAYER_JUMP_SPEED :
    (SFG_player.verticalSpeed - SFG_GRAVITY_SPEED_INCREASE_PER_FRAME);
#endif

  if (!shearing && SFG_player.camera.shear != 0 && !SFG_player.freeLook)
  {
    // gradually shear back to zero

    SFG_player.camera.shear =
      (SFG_player.camera.shear > 0) ?
      RCL_max(0,SFG_player.camera.shear - SFG_CAMERA_SHEAR_STEP_PER_FRAME) :
      RCL_min(0,SFG_player.camera.shear + SFG_CAMERA_SHEAR_STEP_PER_FRAME);
  }

#if SFG_HEADBOB_ENABLED && !SFG_PREVIEW_MODE
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

  // handle player collision with level elements:

  // monsters:
  for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
  {
    SFG_MonsterRecord *m = &(SFG_currentLevel.monsterRecords[i]);

    if (SFG_MR_STATE(*m) == SFG_MONSTER_STATE_INACTIVE)
      continue;

    RCL_Vector2D mPos;

    mPos.x = SFG_MONSTER_COORD_TO_RCL_UNITS(m->coords[0]);
    mPos.y = SFG_MONSTER_COORD_TO_RCL_UNITS(m->coords[1]);

    if (
         SFG_elementCollides(
           SFG_player.camera.position.x,
           SFG_player.camera.position.y,
           SFG_player.camera.height,
           mPos.x,
           mPos.y,
           SFG_floorHeightAt(
               SFG_MONSTER_COORD_TO_SQUARES(m->coords[0]),
               SFG_MONSTER_COORD_TO_SQUARES(m->coords[1]))
         )
       )
    {
      moveOffset = SFG_resolveCollisionWithElement(
        SFG_player.camera.position,moveOffset,mPos);
    }
  }

  uint8_t collidesWithTeleport = 0;

  // items:
  for (int16_t i = 0; i < SFG_currentLevel.itemRecordCount; ++i)
    // ^ has to be int16_t (signed)
  {
    if (!(SFG_currentLevel.itemRecords[i] & SFG_ITEM_RECORD_ACTIVE_MASK))
      continue;

    const SFG_LevelElement *e = SFG_getActiveItemElement(i);

    if (e != 0)
    {
      RCL_Vector2D ePos;

      ePos.x = SFG_ELEMENT_COORD_TO_RCL_UNITS(e->coords[0]);
      ePos.y = SFG_ELEMENT_COORD_TO_RCL_UNITS(e->coords[1]);

      if (
           SFG_elementCollides(
             SFG_player.camera.position.x,
             SFG_player.camera.position.y,
             SFG_player.camera.height,
             ePos.x,
             ePos.y,
             SFG_floorHeightAt(e->coords[0],e->coords[1])
           )
         )
      {
        uint8_t eliminate = 1;

        switch (e->type)
        {
          case SFG_LEVEL_ELEMENT_HEALTH:
            SFG_playerChangeHealth(SFG_HEALTH_KIT_VALUE);
            break;
          
          case SFG_LEVEL_ELEMENT_BULLETS:
            SFG_player.ammo[SFG_AMMO_BULLETS] += SFG_AMMO_INCREASE_BULLETS;
            break;

          case SFG_LEVEL_ELEMENT_ROCKETS:
            SFG_player.ammo[SFG_AMMO_ROCKETS] += SFG_AMMO_INCREASE_ROCKETS;
            break;

          case SFG_LEVEL_ELEMENT_PLASMA:
            SFG_player.ammo[SFG_AMMO_PLASMA] += SFG_AMMO_INCREASE_PLASMA;
            break;

          case SFG_LEVEL_ELEMENT_CARD0:
          case SFG_LEVEL_ELEMENT_CARD1:
          case SFG_LEVEL_ELEMENT_CARD2:
            SFG_player.cards |= 1 << (e->type - SFG_LEVEL_ELEMENT_CARD0);
            break;

          case SFG_LEVEL_ELEMENT_TELEPORT:
            collidesWithTeleport = 1;
            eliminate = 0;
            break;

          default:
            eliminate = 0;
            break;
        }

        if (eliminate) // take the item
        {
#if !SFG_PREVIEW_MODE
          SFG_removeItem(i);
          SFG_player.lastItemTakenFrame = SFG_game.frame;
          i--;
          SFG_playSoundSafe(3,255);
#endif
        }
        else 
        {
          if (e->type != SFG_LEVEL_ELEMENT_TELEPORT)
          {
            // collide
            moveOffset = SFG_resolveCollisionWithElement(
              SFG_player.camera.position,moveOffset,ePos);
          }
          else if ((SFG_currentLevel.teleportCount > 1) &&
            !SFG_player.justTeleported)
          {
            // teleport to random destination teleport

            uint8_t teleportNumber =
              SFG_random() % (SFG_currentLevel.teleportCount - 1) + 1;

            for (uint16_t j = 0; j < SFG_currentLevel.itemRecordCount; ++j)
            {
              SFG_LevelElement e2 = 
                SFG_currentLevel.levelPointer->elements
                  [SFG_currentLevel.itemRecords[j] &
                  ~SFG_ITEM_RECORD_ACTIVE_MASK];

              if ((e2.type == SFG_LEVEL_ELEMENT_TELEPORT) && (j != i))
                teleportNumber--;

              if (teleportNumber == 0)
              {
                SFG_player.camera.position.x =
                  SFG_ELEMENT_COORD_TO_RCL_UNITS(e2.coords[0]);

                SFG_player.camera.position.y =
                  SFG_ELEMENT_COORD_TO_RCL_UNITS(e2.coords[1]);

                SFG_player.camera.height =
                  SFG_floorHeightAt(e2.coords[0],e2.coords[1]) +
                  RCL_CAMERA_COLL_HEIGHT_BELOW;

                SFG_currentLevel.itemRecords[j] |= SFG_ITEM_RECORD_ACTIVE_MASK;
                /* ^ we have to make the new teleport immediately active so
                   that it will immediately collide */

                SFG_player.justTeleported = 1;

                SFG_playSoundSafe(4,255);

                break;
              }
            }
          }
        }
      }
    } 
  } // item collision check

  if (!collidesWithTeleport)
    SFG_player.justTeleported = 0;

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

  // update projectiles:

  uint8_t substractFrames =
    (SFG_game.frame - SFG_currentLevel.frameStart) & 0x01 ? 1 : 0;
    // ^ only substract frames to live every other frame

  for (int8_t i = 0; i < SFG_currentLevel.projectileRecordCount; ++i)
  { // ^ has to be signed
    SFG_ProjectileRecord *p = &(SFG_currentLevel.projectileRecords[i]);

    uint8_t attackType = 255;

    if (p->type == SFG_PROJECTILE_BULLET)
      attackType = SFG_WEAPON_FIRE_TYPE_BULLET;
    else if (p->type == SFG_PROJECTILE_PLASMA)
      attackType = SFG_WEAPON_FIRE_TYPE_PLASMA;

    RCL_Unit pos[3]; // we have to convert from uint16_t because under/overflows

    uint8_t eliminate = 0;

    for (uint8_t j = 0; j < 3; ++j) 
    {
      pos[j] = p->position[j];
      pos[j] += p->direction[j];

      if ( // projectile outside map?
        (pos[j] < 0) ||
        (pos[j] >= (SFG_MAP_SIZE * RCL_UNITS_PER_SQUARE)))
      {
        eliminate = 1;
        break;
      }
    }

    if (p->doubleFramesToLive == 0) // no more time to live?
    {
      eliminate = 1;
    }
    else if (
      (p->type != SFG_PROJECTILE_EXPLOSION) &&
      (p->type != SFG_PROJECTILE_DUST))
    {
      if (SFG_projectileCollides( // collides with player?
            p,
            SFG_player.camera.position.x,
            SFG_player.camera.position.y,
            SFG_player.camera.height))
        {
          eliminate = 1;
          SFG_playerChangeHealth(-1 * SFG_getDamageValue(attackType));
        }

      // check collision with the map

      if (!eliminate &&
          ((SFG_floorHeightAt(pos[0] / RCL_UNITS_PER_SQUARE,pos[1] / 
            RCL_UNITS_PER_SQUARE) >= pos[2])
          ||
          (SFG_ceilingHeightAt(pos[0] / RCL_UNITS_PER_SQUARE,pos[1] /
            RCL_UNITS_PER_SQUARE) <= pos[2]))
        )
        eliminate = 1;

      // check collision with active level elements

      if (!eliminate) // monsters 
        for (uint16_t j = 0; j < SFG_currentLevel.monsterRecordCount; ++j)
        {
          SFG_MonsterRecord *m = &(SFG_currentLevel.monsterRecords[j]);

          if (SFG_MR_STATE(*m) != SFG_MONSTER_STATE_INACTIVE)
          {
            if (SFG_projectileCollides(p,
                  SFG_MONSTER_COORD_TO_RCL_UNITS(m->coords[0]),
                  SFG_MONSTER_COORD_TO_RCL_UNITS(m->coords[1]),
                  SFG_floorHeightAt(
                    SFG_MONSTER_COORD_TO_SQUARES(m->coords[0]),
                    SFG_MONSTER_COORD_TO_SQUARES(m->coords[1]))
                   ))
            {
              eliminate = 1;
              SFG_monsterChangeHealth(m,-1 * SFG_getDamageValue(attackType));
              break;
            }
          }
        }

      if (!eliminate) // items
        for (uint16_t j = 0; j < SFG_currentLevel.itemRecordCount; ++j)
        {
          const SFG_LevelElement *e = SFG_getActiveItemElement(j);

          if (e != 0)
          {
            RCL_Unit x = SFG_ELEMENT_COORD_TO_RCL_UNITS(e->coords[0]);
            RCL_Unit y = SFG_ELEMENT_COORD_TO_RCL_UNITS(e->coords[1]);
            RCL_Unit z = SFG_floorHeightAt(e->coords[0],e->coords[1]);

            if (SFG_projectileCollides(p,x,y,z))
            {
              if (
                   (e->type == SFG_LEVEL_ELEMENT_BARREL) &&
                   (SFG_getDamageValue(attackType) >= 
                     SFG_BARREL_EXPLOSION_DAMAGE_THRESHOLD)
                 )
              {
                SFG_explodeBarrel(j,x,y,z);
              }

              eliminate = 1;
              break;
            }
          }
        }
    }

    if (eliminate)
    {
      if (p->type == SFG_PROJECTILE_FIREBALL)
        SFG_createExplosion(p->position[0],p->position[1],p->position[2]);
      else if (p->type == SFG_PROJECTILE_BULLET)
        SFG_createDust(p->position[0],p->position[1],p->position[2]);
      else if (p->type == SFG_PROJECTILE_PLASMA)
        SFG_playSoundSafe(4,SFG_distantSoundVolume(pos[0],pos[1],pos[2]));

      // remove the projectile

      for (uint8_t j = i; j < SFG_currentLevel.projectileRecordCount - 1; ++j)
        SFG_currentLevel.projectileRecords[j] =
          SFG_currentLevel.projectileRecords[j + 1];

      SFG_currentLevel.projectileRecordCount--;

      i--;
    }
    else
    {
      p->position[0] = pos[0];
      p->position[1] = pos[1];
      p->position[2] = pos[2];
    }

    p->doubleFramesToLive -= substractFrames;
  }

  // handle door:
  if (SFG_currentLevel.doorRecordCount > 0) // has to be here
  {
    /* Check one door on whether a player is standing nearby. For performance
       reasons we only check a few doors and move to others in the next
       frame. */

    for (uint16_t i = 0;
         i < RCL_min(SFG_ELEMENT_DISTANCES_CHECKED_PER_FRAME,
           SFG_currentLevel.doorRecordCount);
         ++i) 
    {
      SFG_DoorRecord *door =
        &(SFG_currentLevel.doorRecords[SFG_currentLevel.checkedDoorIndex]);

      uint8_t upDownState = door->state & SFG_DOOR_UP_DOWN_MASK;

      uint8_t lock = SFG_DOOR_LOCK(door->state);

      uint8_t newUpDownState = 
                (
                  ((lock == 0) || (SFG_player.cards & (1 << (lock - 1)))) &&
                  (door->coords[0] >= (SFG_player.squarePosition[0] - 1)) &&
                  (door->coords[0] <= (SFG_player.squarePosition[0] + 1)) &&
                  (door->coords[1] >= (SFG_player.squarePosition[1] - 1)) &&
                  (door->coords[1] <= (SFG_player.squarePosition[1] + 1))
                ) ? SFG_DOOR_UP_DOWN_MASK : 0x00; 

      if (upDownState != newUpDownState)
        SFG_playSoundSafe(1,255);

      door->state = (door->state & ~SFG_DOOR_UP_DOWN_MASK) | newUpDownState;

      SFG_currentLevel.checkedDoorIndex++;

      if (SFG_currentLevel.checkedDoorIndex >= SFG_currentLevel.doorRecordCount)
        SFG_currentLevel.checkedDoorIndex = 0;
    }

    // move door up/down:
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
    // check item distances:

    for (uint16_t i = 0;
         i < RCL_min(SFG_ELEMENT_DISTANCES_CHECKED_PER_FRAME,
           SFG_currentLevel.itemRecordCount);
         ++i) 
    {
      SFG_ItemRecord item =
        SFG_currentLevel.itemRecords[SFG_currentLevel.checkedItemIndex];

      item &= ~SFG_ITEM_RECORD_ACTIVE_MASK;

      SFG_LevelElement e =
        SFG_currentLevel.levelPointer->elements[item];

      if (
        SFG_isInActiveDistanceFromPlayer(
          e.coords[0] * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2,
          e.coords[1] * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2,
          SFG_floorHeightAt(e.coords[0],e.coords[1]) + RCL_UNITS_PER_SQUARE / 2)
        )
        item |= SFG_ITEM_RECORD_ACTIVE_MASK;

      SFG_currentLevel.itemRecords[SFG_currentLevel.checkedItemIndex] = item;

      SFG_currentLevel.checkedItemIndex++;

      if (SFG_currentLevel.checkedItemIndex >= SFG_currentLevel.itemRecordCount)
        SFG_currentLevel.checkedItemIndex = 0;
    }
  }

  // similarly handle monsters:
  if (SFG_currentLevel.monsterRecordCount > 0) // has to be here
  {
    // check monster distances:

    for (uint16_t i = 0;
         i < RCL_min(SFG_ELEMENT_DISTANCES_CHECKED_PER_FRAME,
           SFG_currentLevel.monsterRecordCount);
         ++i) 
    {
      SFG_MonsterRecord *monster =
      &(SFG_currentLevel.monsterRecords[SFG_currentLevel.checkedMonsterIndex]);

      if ( // far away from the player?
        !SFG_isInActiveDistanceFromPlayer(
          SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
          SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),
          SFG_floorHeightAt(
            SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
            SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1]))
            + RCL_UNITS_PER_SQUARE / 2
          )
        )
      {
        monster->stateType = 
           (monster->stateType & SFG_MONSTER_MASK_TYPE) |
           SFG_MONSTER_STATE_INACTIVE;
      }
      else if (SFG_MR_STATE(*monster) == SFG_MONSTER_STATE_INACTIVE)
      {
        monster->stateType = 
          (monster->stateType & SFG_MONSTER_MASK_TYPE) |
          SFG_MONSTER_STATE_IDLE;
      }

      SFG_currentLevel.checkedMonsterIndex++;

      if (SFG_currentLevel.checkedMonsterIndex >=
        SFG_currentLevel.monsterRecordCount)
        SFG_currentLevel.checkedMonsterIndex = 0;
    }
  }

  // update AI and handle dead monsters:
  if ((SFG_game.frame - SFG_currentLevel.frameStart) %
      SFG_AI_UPDATE_FRAME_INTERVAL == 0)
  {
    for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
    {
      SFG_MonsterRecord *monster = &(SFG_currentLevel.monsterRecords[i]);
      uint8_t state = SFG_MR_STATE(*monster);

      if (state == SFG_MONSTER_STATE_DYING)
      {
        // remove dead

        for (uint16_t j = i; j < SFG_currentLevel.monsterRecordCount - 1; ++j)
          SFG_currentLevel.monsterRecords[j] =
            SFG_currentLevel.monsterRecords[j + 1];        

        SFG_currentLevel.monsterRecordCount -= 1;

        i--;
      }
      else if (monster->health == 0)
      {
        monster->stateType = SFG_MR_TYPE(*monster) | SFG_MONSTER_STATE_DYING;
        SFG_playSoundSafe(2,255);
      }
      else if (state != SFG_MONSTER_STATE_INACTIVE)
      {
#if SFG_PREVIEW_MODE == 0
        SFG_monsterPerformAI(monster);
#endif
      }
    }
  }
}

void SFG_clearScreen(uint8_t color)
{
  for (uint16_t j = 0; j < SFG_GAME_RESOLUTION_Y; ++j)
    for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_X; ++i)
      SFG_setGamePixel(i,j,color);
}

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

/**
  Draws a border that indicates something is happening, e.g. being hurt or
  taking an item.
*/
void SFG_drawIndicationBorder(uint16_t width, uint8_t color)
{
  for (uint16_t j = 0; j < width; ++j)
  {
    uint16_t j2 = SFG_GAME_RESOLUTION_Y - 1 - j;

    for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_X; ++i)
    {
      if ((i & 0x01) == (j & 0x01))
      {
        SFG_setGamePixel(i,j,color);
        SFG_setGamePixel(i,j2,color);
      }
    }
  }

  for (uint16_t i = 0; i < width; ++i)
  {
    uint16_t i2 = SFG_GAME_RESOLUTION_X - 1 - i;

    for (uint16_t j = width; j < SFG_GAME_RESOLUTION_Y - width; ++j)
    {
      if ((i & 0x01) == (j & 0x01))
      {
        SFG_setGamePixel(i,j,color);
        SFG_setGamePixel(i2,j,color);
      }
    }
  }
}

/**
  Draws the player weapon, handling the shooting animation.
*/
void SFG_drawWeapon(int16_t bobOffset)
{
  uint32_t shotAnimationFrame =
    SFG_game.frame - SFG_player.weaponCooldownStartFrame;

  uint32_t animationLength = SFG_GET_WEAPON_FIRE_COOLDOWN_FRAMES(SFG_player.weapon);

  bobOffset -= SFG_HUD_BAR_HEIGHT;
     
  uint8_t fireType = SFG_GET_WEAPON_FIRE_TYPE(SFG_player.weapon);

  if (shotAnimationFrame < animationLength)
  {
    if (fireType == SFG_WEAPON_FIRE_TYPE_MELEE)
    {
      bobOffset = shotAnimationFrame < animationLength / 2 ? 0 :
        2 * SFG_WEAPONBOB_OFFSET_PIXELS     ;
    }
    else
    {
      bobOffset +=  
          ((animationLength - shotAnimationFrame) * SFG_WEAPON_IMAGE_SCALE * 20)
          / animationLength;
   
      if (
        ((fireType == SFG_WEAPON_FIRE_TYPE_FIREBALL) ||
         (fireType == SFG_WEAPON_FIRE_TYPE_BULLET)) &&
        shotAnimationFrame < animationLength / 2)
        SFG_blitImage(SFG_effectSprites[0],
          SFG_WEAPON_IMAGE_POSITION_X,
          SFG_WEAPON_IMAGE_POSITION_Y - (SFG_TEXTURE_SIZE / 3) * SFG_WEAPON_IMAGE_SCALE + bobOffset,
          SFG_WEAPON_IMAGE_SCALE);
    }
  }

  SFG_blitImage(SFG_weaponImages[SFG_player.weapon],
  SFG_WEAPON_IMAGE_POSITION_X,
  SFG_WEAPON_IMAGE_POSITION_Y + bobOffset - 1,
  SFG_WEAPON_IMAGE_SCALE);
}

void SFG_drawMenu()
{
  #define BACKGROUND_SCALE (SFG_GAME_RESOLUTION_X / (4 * SFG_TEXTURE_SIZE ) )

  #if BACKGROUND_SCALE == 0
    #define BACKGROUND_SCALE 1
  #endif

  #define SCROLL_PIXELS_PER_FRAME ((64 * SFG_GAME_RESOLUTION_X) / (8 * SFG_FPS))

  #if SCROLL_PIXELS_PER_FRAME == 0
    #define SCROLL_PIXELS_PER_FRAME 1
  #endif

  #define MAX_ITEMS 8

  #define CHAR_SIZE (SFG_FONT_SIZE_MEDIUM * (SFG_FONT_CHARACTER_SIZE + 1))

  uint16_t scroll = (SFG_game.frame * SCROLL_PIXELS_PER_FRAME) / 64;

  for (uint16_t y = 0; y < SFG_GAME_RESOLUTION_Y; ++y)
    for (uint16_t x = 0; x < SFG_GAME_RESOLUTION_X; ++x)
      SFG_setGamePixel(x,y,
        (y >= (SFG_TEXTURE_SIZE * BACKGROUND_SCALE)) ? 0 :
        SFG_getTexel(SFG_backgroundImages[0],((x + scroll) / BACKGROUND_SCALE)
          % SFG_TEXTURE_SIZE,y / BACKGROUND_SCALE));

  char *itemTexts[MAX_ITEMS];

  for (uint8_t i = 0; i < MAX_ITEMS; ++i)
    itemTexts[i] = 0;

  itemTexts[0] = "continue";
  itemTexts[1] = "map";
  itemTexts[2] = "play level 1";
  itemTexts[3] = "load";
  itemTexts[4] = "sound on";
  itemTexts[5] = "exit";


uint16_t y = CHAR_SIZE;


SFG_blitImage(SFG_logoImage,   SFG_GAME_RESOLUTION_X / 2 - 16 * SFG_FONT_SIZE_MEDIUM,
  y,SFG_FONT_SIZE_MEDIUM   );

  y += 32 * SFG_FONT_SIZE_MEDIUM + CHAR_SIZE * 2;

  for (uint8_t i = 0; i < MAX_ITEMS; ++i)
  {

    if (itemTexts[i] == 0)
      break;

    uint8_t textLen = 0;

    while (itemTexts[i][textLen] != 0)
      textLen++;


    uint16_t drawX = (SFG_GAME_RESOLUTION_X - textLen * CHAR_SIZE) / 2;



    //SFG_drawText(itemTexts[i],drawX - 1,y - 1,SFG_FONT_SIZE_MEDIUM,63);
    SFG_drawText(itemTexts[i],drawX,y,SFG_FONT_SIZE_MEDIUM,23);


    y += CHAR_SIZE + SFG_FONT_SIZE_MEDIUM;
  }

  #undef CHAR_SIZE
  #undef MAX_ITEMS
  #undef BACKGROUND_SCALE
  #undef SCROLL_PIXELS_PER_FRAME
}

void SFG_draw()
{
#if SFG_BACKGROUND_BLUR != 0
  SFG_backgroundBlurIndex = 0;
#endif

SFG_drawMenu();
return;

  if (SFG_keyPressed(SFG_KEY_MAP))
  {
    SFG_drawMap();
  } 
  else
  { 
    for (uint16_t i = 0; i < SFG_Z_BUFFER_SIZE; ++i)
      SFG_game.zBuffer[i] = 255;

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
      SFG_game.rayConstraints);
 
    // draw sprites:

    // monster sprites:
    for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
    {
      SFG_MonsterRecord m = SFG_currentLevel.monsterRecords[i];

      uint8_t state = SFG_MR_STATE(m);

      if (state != SFG_MONSTER_STATE_INACTIVE)
      {
        RCL_Vector2D worldPosition;

        worldPosition.x = SFG_MONSTER_COORD_TO_RCL_UNITS(m.coords[0]);
        worldPosition.y = SFG_MONSTER_COORD_TO_RCL_UNITS(m.coords[1]);

        uint8_t spriteSize = SFG_GET_MONSTER_SPRITE_SIZE(
          SFG_MONSTER_TYPE_TO_INDEX(SFG_MR_TYPE(m)));

        RCL_PixelInfo p =
          RCL_mapToScreen(
            worldPosition,
            SFG_floorHeightAt(
              SFG_MONSTER_COORD_TO_SQUARES(m.coords[0]),
              SFG_MONSTER_COORD_TO_SQUARES(m.coords[1]))
              + 
              SFG_SPRITE_SIZE_TO_HEIGH_ABOVE_GROUND(spriteSize),
              SFG_player.camera);

        if (p.depth > 0)
        {
          const uint8_t *s =
            SFG_getMonsterSprite(
              SFG_MR_TYPE(m),
              state,
              SFG_game.spriteAnimationFrame & 0x01);

          SFG_drawScaledSprite(s,
            p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
            RCL_perspectiveScale(
            SFG_SPRITE_SIZE(spriteSize),
            p.depth),
            p.depth / (RCL_UNITS_PER_SQUARE * 2),p.depth);
        }
      }
    }

    // item sprites:
    for (uint16_t i = 0; i < SFG_currentLevel.itemRecordCount; ++i)
      if (SFG_currentLevel.itemRecords[i] & SFG_ITEM_RECORD_ACTIVE_MASK)
      {
        RCL_Vector2D worldPosition;

        SFG_LevelElement e = 
          SFG_currentLevel.levelPointer->elements[
            SFG_currentLevel.itemRecords[i] & ~SFG_ITEM_RECORD_ACTIVE_MASK];

        worldPosition.x =
          SFG_ELEMENT_COORD_TO_RCL_UNITS(e.coords[0]);

        worldPosition.y =
          SFG_ELEMENT_COORD_TO_RCL_UNITS(e.coords[1]);

        uint8_t spriteIndex;
        uint8_t spriteSize;

        SFG_getLevelElementSprite(e.type,&spriteIndex,&spriteSize);

        RCL_PixelInfo p =
          RCL_mapToScreen(
            worldPosition,
            SFG_floorHeightAt(e.coords[0],e.coords[1])
            + SFG_SPRITE_SIZE_TO_HEIGH_ABOVE_GROUND(spriteSize),
            SFG_player.camera);

        if (p.depth > 0)
        {
          SFG_drawScaledSprite(
            SFG_itemSprites[spriteIndex],
            p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
            RCL_perspectiveScale(SFG_SPRITE_SIZE(spriteSize),p.depth),
            p.depth / (RCL_UNITS_PER_SQUARE * 2),p.depth - 1000);
        }
      }

    // projectile sprites:
    for (uint8_t i = 0; i < SFG_currentLevel.projectileRecordCount; ++i)
    {
      SFG_ProjectileRecord *proj = &(SFG_currentLevel.projectileRecords[i]);

      if (proj->type == SFG_PROJECTILE_BULLET)
        continue; // bullets aren't drawn

      RCL_Vector2D worldPosition;

      worldPosition.x = proj->position[0];
      worldPosition.y = proj->position[1];

      RCL_PixelInfo p =
        RCL_mapToScreen(worldPosition,proj->position[2],SFG_player.camera);
       
      const uint8_t *s = SFG_effectSprites[proj->type];

      int16_t spriteSize = SFG_SPRITE_SIZE(0);

      if (proj->type == SFG_PROJECTILE_EXPLOSION ||
          proj->type == SFG_PROJECTILE_DUST)
      {
        int16_t doubleFramesToLive =
          RCL_nonZero(SFG_GET_PROJECTILE_FRAMES_TO_LIVE(proj->type) / 2);

        // grow the explosion sprite as an animation
        spriteSize =
          (
            SFG_BASE_SPRITE_SIZE *
            RCL_sinInt(          
              ((doubleFramesToLive -
               proj->doubleFramesToLive) * RCL_UNITS_PER_SQUARE / 4)
               / doubleFramesToLive) 
          ) / RCL_UNITS_PER_SQUARE;
      }

      if (p.depth > 0)
        SFG_drawScaledSprite(s,
            p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
            RCL_perspectiveScale(spriteSize,p.depth),
            SFG_fogValueDiminish(p.depth),
            p.depth);  
    }

#if SFG_HEADBOB_ENABLED
    // substract head bob after rendering
    SFG_player.camera.height -= headBobOffset;
#endif

#if SFG_PREVIEW_MODE == 0
    SFG_drawWeapon(weaponBobOffset);
#endif

    // draw the HUD:

    // bar

    uint8_t color = 5;

    for (uint16_t j = SFG_GAME_RESOLUTION_Y - SFG_HUD_BAR_HEIGHT;
      j < SFG_GAME_RESOLUTION_Y; ++j)
    {
      for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_X; ++i)
        SFG_setGamePixel(i,j,color);

      color = 2;
    }

    #define TEXT_Y (SFG_GAME_RESOLUTION_Y - SFG_HUD_MARGIN - \
      SFG_FONT_CHARACTER_SIZE * SFG_FONT_SIZE_MEDIUM)

    SFG_drawNumber( // health
      SFG_player.health,
      SFG_HUD_MARGIN,
      TEXT_Y,
      SFG_FONT_SIZE_MEDIUM,
      SFG_player.health > SFG_PLAYER_HEALTH_WARNING_LEVEL ? 4 : 175);

    SFG_drawNumber( // ammo
      SFG_player.ammo[SFG_weaponAmmo(SFG_player.weapon)],
      SFG_GAME_RESOLUTION_X - SFG_HUD_MARGIN -
        (SFG_FONT_CHARACTER_SIZE + 1) * SFG_FONT_SIZE_MEDIUM * 3,
      TEXT_Y,
      SFG_FONT_SIZE_MEDIUM,
      4); 

    for (uint8_t i = 0; i < 3; ++i) // access cards
      if (SFG_player.cards & (1 << i))
        SFG_drawText(",",SFG_HUD_MARGIN + (SFG_FONT_CHARACTER_SIZE + 1) *
        SFG_FONT_SIZE_MEDIUM * (6 + i),
        TEXT_Y,SFG_FONT_SIZE_MEDIUM,i == 0 ? 93 : (i == 1 ? 124 : 60));

    #undef TEXT_Y

    // border indicator

    if (SFG_game.frame - SFG_player.lastHurtFrame
        <= SFG_HUD_BORDER_INDICATOR_DURATION_FRAMES)
      SFG_drawIndicationBorder(SFG_HUD_BORDER_INDICATOR_WIDTH_PIXELS,
      SFG_HUD_HURT_INDICATION_COLOR);
    else if (SFG_game.frame - SFG_player.lastItemTakenFrame
        <= SFG_HUD_BORDER_INDICATOR_DURATION_FRAMES)
      SFG_drawIndicationBorder(SFG_HUD_BORDER_INDICATOR_WIDTH_PIXELS,
      SFG_HUD_ITEM_TAKEN_INDICATION_COLOR);
  }
}

void SFG_mainLoopBody()
{
  /* standard deterministic game loop, independed on actuall achieved FPS,
     each game logic (physics) frame is performed with the SFG_MS_PER_FRAME
     delta time. */
  uint32_t timeNow = SFG_getTimeMs();
  uint32_t timeNextFrame = SFG_game.lastFrameTimeMs + SFG_MS_PER_FRAME;

  SFG_game.frameTime = timeNow;

  if (timeNow >= timeNextFrame)
  {
    uint32_t timeSinceLastFrame = timeNow - SFG_game.lastFrameTimeMs;

    uint8_t steps = 0;

    // perform game logic (physics), for each frame
    while (timeSinceLastFrame >= SFG_MS_PER_FRAME)
    {
      SFG_gameStep();

      timeSinceLastFrame -= SFG_MS_PER_FRAME;

      SFG_game.frame++;
      steps++;
    }

    if (steps > 1)
      SFG_LOG("Failed to reach target FPS! Consider setting a lower value.")

    // render noly once
    SFG_draw();

    SFG_game.lastFrameTimeMs = timeNow;
  }
  else
  {
    SFG_sleepMs((timeNextFrame - timeNow) / 2); // wait, relieve CPU
  }
}
