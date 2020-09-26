/**
  @file game.h
 
  Main source file of the game that puts together all the pieces. main game
  logic is implemented here.

  Physics notes (you can break this when messing with constants):

  - Lowest ceiling under which player can fit is 4 height steps.
  - Widest hole over which player can run without jumping is 1 square.
  - Widest hole over which the player can jump is 3 squares.
  - Highest step a player can walk onto without jumping is 2 height steps.
  - Highest step a player can jump onto is 3 height steps.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_GAME_H
#define _SFG_GAME_H

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
#define SFG_KEY_MENU 14

#define SFG_KEY_COUNT 15 ///< Number of keys.

/* ============================= PORTING =================================== */

/* When porting, do the following:
   - Include this file (and possibly other optionaly files) in your main_*
     frontend source.
   - Implement the following functions in your frontend source.
   - Call SFG_init() from your frontend initialization code.
   - Call SFG_mainLoopBody() from within your frontend main loop.
*/

#ifndef SFG_LOG
  #define SFG_LOG(str) ; ///< Can be redefined to log messages for better debug.
#endif

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
  mouse, this function can simply return [0,0] offets at each call, or even
  do nothing (leave the variables as are).
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
  samples provided in sounds.h, and it may or may not ignore the (logarithmic)
  volume parameter (0 to 255). Depending on the platform the function can play
  completely different samples or even e.g. just beeps. If the platform can't
  play sounds, this function implementation can simply be left empty. This
  function doesn't have to implement safety measures, the back end takes cares
  of them.
*/
void SFG_playSound(uint8_t soundIndex, uint8_t volume);

/**
  Informs the frontend whether music should get enabled/disabled. Playing music
  is optional and the frontend can ignore it. If a frontend wants to implement
  music, it can use the one provided in sounds.h or use its own.
*/
void SFG_enableMusic(uint8_t enable);

#define SFG_SAVE_SIZE 12

/**
  Optional function for permanently saving game state. Platform that don't have
  permanent storage may let this function do nothing. If implemented, the
  function should save the passed data into its permanent storage, e.g. a file,
  a cookie etc.
*/
void SFG_save(uint8_t data[SFG_SAVE_SIZE]);

/**
  Optional function for retrieving game data that were saved to permanent
  storage. Platforms without permanent storage may let this function do nothing.
  If implemented, the function should fill the passed array with data from
  permanent storage, e.g. a file, a cookie etc.

  If this function is called before SFG_save was ever called and no data is
  present in permanent memory, this function should do nothing (leave the data
  array as is).

  This function should return 1 if saving/loading is possible and 0 if not (this
  will be used by the game to detect this capability).
*/
uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE]);

/* ========================================================================= */

/**
  Game main loop body, call this inside the platform's specific main loop.
  Returns 1 if the game continues, 0 if the game was exited.
*/
uint8_t SFG_mainLoopBody();

/**
  Initializes the whole program, call this in the platform initialization.
*/
void SFG_init();

#include "settings.h"

#if SFG_ARDUINO
  #include <avr/pgmspace.h>

  #define SFG_PROGRAM_MEMORY const PROGMEM
  #define SFG_PROGRAM_MEMORY_U8(addr) pgm_read_byte(addr)
  // TODO
#else
  #define SFG_PROGRAM_MEMORY static const
  #define SFG_PROGRAM_MEMORY_U8(addr) ((uint8_t) (*(addr)))
#endif

#include "images.h"
#include "levels.h"
#include "texts.h"
#include "palette.h"

#if SFG_TEXTURE_DISTANCE == 0
  #define RCL_COMPUTE_WALL_TEXCOORDS 0
#endif

#define RCL_PIXEL_FUNCTION SFG_pixelFunc
#define RCL_TEXTURE_VERTICAL_STRETCH 0

#define RCL_CAMERA_COLL_HEIGHT_BELOW 800
#define RCL_CAMERA_COLL_HEIGHT_ABOVE 200

#include "raycastlib.h" 

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

#define SFG_SPRITE_SIZE_PIXELS(size0to3) \
  ((SFG_SPRITE_SIZE(size0to3) * SFG_GAME_RESOLUTION_Y) / RCL_UNITS_PER_SQUARE)

#define SFG_SPRITE_SIZE_TO_HEIGHT_ABOVE_GROUND(size0to3) \
  (SFG_SPRITE_SIZE(size0to3) / 2)

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
  uint8_t coords[2];     /**< monster position, in 1/4s of a square */
  uint8_t health;
} SFG_MonsterRecord;

#define SFG_MR_STATE(mr) ((mr).stateType & SFG_MONSTER_MASK_STATE)
#define SFG_MR_TYPE(mr) \
  (SFG_MONSTER_INDEX_TO_TYPE(((mr).stateType & SFG_MONSTER_MASK_TYPE) >> 4))

#define SFG_MONSTER_COORD_TO_RCL_UNITS(c) ((RCL_UNITS_PER_SQUARE / 8) + c * 256)
#define SFG_MONSTER_COORD_TO_SQUARES(c) (c / 4)

#define SFG_ELEMENT_COORD_TO_RCL_UNITS(c) \
  (c * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2)

#define SFG_MONSTER_MASK_STATE 0x0f
#define SFG_MONSTER_MASK_TYPE  0xf0

#define SFG_MONSTER_STATE_INACTIVE  0 ///< Not nearby, not actively updated.
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
#define SFG_MONSTER_STATE_DEAD      13

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

#define SFG_GAME_STATE_MENU 0
#define SFG_GAME_STATE_PLAYING 1
#define SFG_GAME_STATE_WIN 2
#define SFG_GAME_STATE_LOSE 3
#define SFG_GAME_STATE_INTRO 4
#define SFG_GAME_STATE_OUTRO 5
#define SFG_GAME_STATE_MAP 6
#define SFG_GAME_STATE_LEVEL_START 7

#define SFG_MENU_ITEM_CONTINUE 0
#define SFG_MENU_ITEM_MAP 1
#define SFG_MENU_ITEM_PLAY 2
#define SFG_MENU_ITEM_LOAD 3
#define SFG_MENU_ITEM_SOUND 4
#define SFG_MENU_ITEM_SHEAR 5
#define SFG_MENU_ITEM_EXIT 6

#define SFG_MENU_ITEM_NONE 255

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
  uint8_t state;
  uint32_t stateChangeTime;      ///< Time in ms at which the state was changed.

  uint8_t currentRandom;         ///< for RNG
  uint8_t spriteAnimationFrame;

  uint8_t soundsPlayedThisFrame; /**< Each bit says whether given sound was
                                    played this frame, prevents playing too many
                                    sounds at once. */

  RCL_RayConstraints rayConstraints;
  uint8_t keyStates[SFG_KEY_COUNT]; /**< Pressed states of keys, each value
                                    stores the number of frames for which the
                                    key has been held. */
  uint8_t zBuffer[SFG_Z_BUFFER_SIZE];

  uint8_t textureAverageColors[SFG_WALL_TEXTURE_COUNT]; /**< Contains average
                                    color for each wall texture. */

  int8_t backgroundScaleMap[SFG_GAME_RESOLUTION_Y];
  uint16_t backgroundScroll;
  uint8_t spriteSamplingPoints[SFG_MAX_SPRITE_SIZE]; /**< Helper for
                                                     precomputing sprite
                                                     sampling positions for
                                                     drawing. */
  uint32_t frameTime; ///< Keeps a constant time (in ms) during a frame
  uint32_t frame;
  uint32_t lastFrameTimeMs;
  uint8_t selectedMenuItem;
  uint8_t selectedLevel;   ///< Level to play selected in the main menu.
  uint8_t antiSpam;   ///< Prevents log message spamming.
  uint8_t settings;   /**< Dynamic game settings (can be changed at runtime),
                           bit meaning:

                           MSB -------- LSB
                                   ||||
                                   |||\_ sound (SFX)
                                   ||\__ music
                                   |\___ shearing
                                   \____ freelook (shearing not sliding back) */

  uint8_t blink;      ///< Says whether blinkg is currently on or off.
  uint8_t saved;      /**< Helper variable to know if game was saved. Can be
                           0 (not saved), 1 (just saved) or 255 (can't save).*/
  uint8_t save[SFG_SAVE_SIZE];  /**< Stores the game save state that's kept in
                           the persistent memory.

                           The save format is binary and platform independent.
                           The save contains game settings, game progress and a
                           saved position. The format is as follows:

                         0  4b  highest level that has been reached
                         0  4b  level number of the saved position (15: no save)
                         1  8b  game settings (SFG_game.settings)
                         2  8b  health at saved position
                         3  8b  bullet ammo at saved position
                         4  8b  rocket ammo at saved position
                         5  8b  plasma ammo at saved position
                         6  32b little endian total play time, in 10ths of sec
                         10 16b little endian total enemies killed from start */
  uint8_t continues;  ///< Whether the game continues or was exited.
} SFG_game;

#define SFG_SAVE_TOTAL_TIME (SFG_game.save[6] + SFG_game.save[7] * 256 + \
  SFG_game.save[8] * 65536 + SFG_game.save[9] * 4294967296)

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

  uint32_t weaponCooldownFrames; ///< frames left for weapon cooldow
  uint32_t lastHurtFrame;
  uint32_t lastItemTakenFrame;

  uint8_t  ammo[SFG_AMMO_TOTAL];

  uint8_t  cards;                /**< Lowest 3 bits say which access cards have
                                 been taken., the next 3 bits say which cards
                                 should be blinking in the HUD, the last
                                 2 bits are a blink reset counter. */
  uint8_t  justTeleported;
} SFG_player;

/**
  Stores the current level and helper precomputed vaues for better performance.
*/
struct
{
  const SFG_Level *levelPointer;
  uint8_t levelNumber;
  const uint8_t* textures[7];

  uint32_t timeStart;
  uint32_t frameStart;
  uint32_t completionTime10sOfS; ///< Completion time in 10th of second.

  uint8_t floorColor;
  uint8_t ceilingColor;

  SFG_DoorRecord doorRecords[SFG_MAX_DOORS];
  uint8_t doorRecordCount;
  uint8_t checkedDoorIndex; ///< Says which door are currently being checked.

  SFG_ItemRecord itemRecords[SFG_MAX_ITEMS]; ///< Holds level items.
  uint8_t itemRecordCount;
  uint8_t checkedItemIndex; ///< Same as checkedDoorIndex, but for items.

  SFG_MonsterRecord monsterRecords[SFG_MAX_MONSTERS];
  uint8_t monsterRecordCount;
  uint8_t checkedMonsterIndex; 

  SFG_ProjectileRecord projectileRecords[SFG_MAX_PROJECTILES];
  uint8_t projectileRecordCount;
  uint8_t bossCount;
  uint8_t monstersDead;
  uint8_t backgroundImage;
  uint8_t teleportCount;
  uint16_t mapRevealMask; /**< Bits say which parts of the map have been
                               revealed. */

  uint8_t itemCollisionMap[(SFG_MAP_SIZE * SFG_MAP_SIZE) / 8];
                          /**< Bit array, for each map square says whether there
                               is a colliding item or not. */
} SFG_currentLevel;

#if SFG_ARDUINO
/**
  Copy of the current level that is stored in RAM. This is only done on Arduino
  because accessing it in program memory directly would be difficult.
*/
SFG_Level SFG_ramLevel;
#endif

/**
  Helper function for accessing the itemCollisionMap bits.
*/
void SFG_getItemCollisionMapIndex(
  uint8_t x, uint8_t y, uint16_t *byte, uint8_t *bit)
{
  uint16_t index = y * SFG_MAP_SIZE + x;

  *byte = index / 8;
  *bit = index % 8;
}

void SFG_setItemCollisionMapBit(uint8_t x, uint8_t y, uint8_t value)
{
  uint16_t byte;
  uint8_t bit;

  SFG_getItemCollisionMapIndex(x,y,&byte,&bit);

  SFG_currentLevel.itemCollisionMap[byte] &= ~(0x01 << bit);
  SFG_currentLevel.itemCollisionMap[byte] |= (value & 0x01) << bit;
}

uint8_t SFG_getItemCollisionMapBit(uint8_t x, uint8_t y)
{
  uint16_t byte;
  uint8_t bit;

  SFG_getItemCollisionMapIndex(x,y,&byte,&bit);
  return (SFG_currentLevel.itemCollisionMap[byte] >> bit) & 0x01;
}

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
  Returns a pseudorandom byte. This is a very simple congrent generator, its
  parameters have been chosen so that each number (0-255) is included in the
  output exactly once!
*/
uint8_t SFG_random()
{
  SFG_game.currentRandom *= 13;
  SFG_game.currentRandom += 7;
  
  return SFG_game.currentRandom;
}

void SFG_playGameSound(uint8_t soundIndex, uint8_t volume)
{
  if (!(SFG_game.settings & 0x01))
    return;

  uint8_t mask = 0x01 << soundIndex;

  if (!(SFG_game.soundsPlayedThisFrame & mask))
  {
    SFG_playSound(soundIndex,volume);
    SFG_game.soundsPlayedThisFrame |= mask;
  }
}

/**
  Returns a damage value for specific attack type (SFG_WEAPON_FIRE_TYPE_...),
  with added randomness (so the values will differ). For explosion pass
  SFG_WEAPON_FIRE_TYPE_FIREBALL.
*/
uint8_t SFG_getDamageValue(uint8_t attackType)
{
  if (attackType >= SFG_WEAPON_FIRE_TYPES_TOTAL)
    return 0;

  int32_t value = SFG_attackDamageTable[attackType]; // has to be signed
  int32_t maxAdd = (value * SFG_DAMAGE_RANDOMNESS) / 256;

  value = value + (maxAdd / 2) - (SFG_random() * maxAdd / 256);

  if (value < 0)
    value = 0;

  return value;
}

/**
  Saves game data to persistent storage.
*/
void SFG_gameSave()
{
  if (SFG_game.saved == SFG_CANT_SAVE)
    return;

  SFG_LOG("saving game data");

  SFG_save(SFG_game.save);
}

/**
  Loads game data from persistent storage.
*/
void SFG_gameLoad()
{
  if (SFG_game.saved == SFG_CANT_SAVE)
    return;

  SFG_LOG("loading game data");

  uint8_t result = SFG_load(SFG_game.save);

  if (result == 0)
    SFG_game.saved = SFG_CANT_SAVE;
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
  return (RCL_abs(x0 - x1) + RCL_abs(y0 - y1) + RCL_abs(z0 - z1));
}

uint8_t SFG_isInActiveDistanceFromPlayer(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  return SFG_taxicabDistance(
    x,y,z,SFG_player.camera.position.x,SFG_player.camera.position.y,
    SFG_player.camera.height) <= SFG_LEVEL_ELEMENT_ACTIVE_DISTANCE;
}

/**
  Function called when a level end to compute the stats etc.
*/
void SFG_levelEnds()
{
  SFG_currentLevel.completionTime10sOfS = (SFG_MS_PER_FRAME *
    (SFG_game.frame - SFG_currentLevel.frameStart)) / 100; 

  SFG_currentLevel.monstersDead = 0;
  
  for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
    if (SFG_currentLevel.monsterRecords[i].health == 0)
      SFG_currentLevel.monstersDead++;

  uint32_t totalTime = SFG_SAVE_TOTAL_TIME;

  if ((SFG_currentLevel.levelNumber == 0) || (totalTime != 0))
  {
    SFG_LOG("Updating save totals.");
  
    totalTime += SFG_currentLevel.completionTime10sOfS;

    for (uint8_t i = 0; i < 4; ++i)
    {
      SFG_game.save[6 + i] = totalTime % 256;
      totalTime /= 256;
    }

    SFG_game.save[10] += SFG_currentLevel.monstersDead % 256;
    SFG_game.save[11] += SFG_currentLevel.monstersDead / 256;
  }

  SFG_game.save[0] = 
    (SFG_game.save[0] & 0x0f) | ((SFG_currentLevel.levelNumber + 1) << 4);

  SFG_game.save[2] = SFG_player.health;
  SFG_game.save[3] = SFG_player.ammo[0];
  SFG_game.save[4] = SFG_player.ammo[1];
  SFG_game.save[5] = SFG_player.ammo[2];
}

static inline uint8_t SFG_RCLUnitToZBuffer(RCL_Unit x)
{
  x /= (RCL_UNITS_PER_SQUARE / 8);

  uint8_t okay = x < 256;

  return okay * (x + 1) - 1;
}

const uint8_t *SFG_getMonsterSprite(
  uint8_t monsterType, uint8_t state, uint8_t frame)
{
  uint8_t index = 
    state == SFG_MONSTER_STATE_DEAD ? 18 : 17;
  // ^ makes the code smaller compared to returning pointers

  if ((state != SFG_MONSTER_STATE_DYING) && (state != SFG_MONSTER_STATE_DEAD))
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
  
  return SFG_monsterSprites + index * SFG_TEXTURE_STORE_SIZE;
}

/**
  Says whether given key is currently pressed (down). This should be preferred
  to SFG_keyPressed().
*/
uint8_t SFG_keyIsDown(uint8_t key)
{
  return SFG_game.keyStates[key] != 0;
}

/**
  Says whether given key has been pressed in the current frame.
*/
uint8_t SFG_keyJustPressed(uint8_t key)
{
  return (SFG_game.keyStates[key]) == 1;
}

/**
  Says whether a key is being repeated after being held for certain time.
*/
uint8_t SFG_keyRepeated(uint8_t key)
{
  return
    ((SFG_game.keyStates[key] >= SFG_KEY_REPEAT_DELAY_FRAMES) ||
    (SFG_game.keyStates[key] == 255)) &&
    (SFG_game.frame % SFG_KEY_REPEAT_PERIOD_FRAMES == 0);
}

uint16_t SFG_keyRegisters(uint8_t key)
{
  return SFG_keyJustPressed(key) || SFG_keyRepeated(key);
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

void SFG_recomputePLayerDirection()
{
  SFG_player.camera.direction =
    RCL_wrap(SFG_player.camera.direction,RCL_UNITS_PER_SQUARE);

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
  {
    0  * SFG_BACKGROUND_BLUR,
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

static inline uint8_t SFG_fogValueDiminish(RCL_Unit depth)
{
  return depth / SFG_FOG_DIMINISH_STEP;
}

static inline uint8_t
  SFG_getTexelFull(uint8_t textureIndex,RCL_Unit u, RCL_Unit v)
{
  return
    SFG_getTexel(
      textureIndex != 255 ?
        SFG_currentLevel.textures[textureIndex] :
          (SFG_wallTextures + SFG_currentLevel.levelPointer->doorTextureIndex
          * SFG_TEXTURE_STORE_SIZE),
      u / 32,
      v / 32); 
}

static inline uint8_t SFG_getTexelAverage(uint8_t textureIndex)
{
  return
    textureIndex != 255 ?
      SFG_game.textureAverageColors[
        SFG_currentLevel.levelPointer->textureIndices[textureIndex]]
      :
      (
        SFG_game.textureAverageColors[
          SFG_currentLevel.levelPointer->doorTextureIndex]
        + 1 // to distinguish from normal walls
      );
}

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

#if SFG_TEXTURE_DISTANCE != 0
    RCL_Unit textureV = pixel->texCoords.y;

    if ((pixel->hit.type & SFG_TILE_PROPERTY_MASK) ==
      SFG_TILE_PROPERTY_SQUEEZER)
      textureV += pixel->wallHeight;
#endif

    color =
      textureIndex != SFG_TILE_TEXTURE_TRANSPARENT ?
      (
#if SFG_TEXTURE_DISTANCE >= 65535
      SFG_getTexelFull(textureIndex,pixel->texCoords.x,textureV)
#elif SFG_TEXTURE_DISTANCE == 0 
      SFG_getTexelAverage(textureIndex)
#else
      pixel->depth <= SFG_TEXTURE_DISTANCE ?
        SFG_getTexelFull(textureIndex,pixel->texCoords.x,textureV) :
        SFG_getTexelAverage(textureIndex)
#endif
      )
      :
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
#if SFG_DRAW_LEVEL_BACKGROUND
    color = SFG_getTexel(SFG_backgroundImages + 
        SFG_currentLevel.backgroundImage * SFG_TEXTURE_STORE_SIZE,
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
#else
    color = 1;
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

  #define PRECOMP_SCALE 512

  int16_t precompStepScaled = ((SFG_TEXTURE_SIZE) * PRECOMP_SCALE) / size;
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

  SFG_TileDefinition tile =
    SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&p);

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
    low + halfHeight + (RCL_sin(sinArg) * halfHeight) / RCL_UNITS_PER_SQUARE;
}

RCL_Unit SFG_floorHeightAt(int16_t x, int16_t y)
{
  uint8_t properties;

  SFG_TileDefinition tile =
    SFG_getMapTile(SFG_currentLevel.levelPointer,x,y,&properties);

  RCL_Unit doorHeight = 0;

  if (properties == SFG_TILE_PROPERTY_DOOR)
  {
    for (uint8_t i = 0; i < SFG_currentLevel.doorRecordCount; ++i)
    {
      SFG_DoorRecord *door = &(SFG_currentLevel.doorRecords[i]);

      if ((door->coords[0] == x) && (door->coords[1] == y))
      {
        doorHeight = door->state & SFG_DOOR_VERTICAL_POSITION_MASK;

        doorHeight = doorHeight != (0xff & SFG_DOOR_VERTICAL_POSITION_MASK)    ? 
          doorHeight * SFG_DOOR_HEIGHT_STEP : RCL_UNITS_PER_SQUARE;


        break;
      }
    }
  }
  else if (properties == SFG_TILE_PROPERTY_ELEVATOR)
  {
    RCL_Unit height =
      SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP;

    return SFG_movingWallHeight(
      height,
      height + SFG_TILE_CEILING_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP,
      SFG_game.frameTime - SFG_currentLevel.timeStart);
  }
 
  return SFG_TILE_FLOOR_HEIGHT(tile) * SFG_WALL_HEIGHT_STEP - doorHeight;
}

/**
  Like SFG_floorCollisionHeightAt, but takes into account colliding items on
  the map, so the squares that have these items are higher. The former function
  is for rendering, this one is for collision checking.
*/
RCL_Unit SFG_floorCollisionHeightAt(int16_t x, int16_t y)
{
  return SFG_floorHeightAt(x,y) +
    SFG_getItemCollisionMapBit(x,y) * RCL_UNITS_PER_SQUARE; 
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
  uint8_t initialWeapon = SFG_player.weapon;
  int8_t increment = next ? 1 : -1;

  while (1)
  {
    SFG_player.weapon =
      (SFG_WEAPONS_TOTAL + SFG_player.weapon + increment) % SFG_WEAPONS_TOTAL;

    if (SFG_player.weapon == initialWeapon)
      break;

    uint8_t ammo, projectileCount, canShoot;

    SFG_getPlayerWeaponInfo(&ammo,&projectileCount,&canShoot);
 
    if (canShoot)
      break;
  }
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
      SFG_currentLevel.levelPointer->playerStart[1]) + 
      RCL_CAMERA_COLL_HEIGHT_BELOW;

  SFG_player.camera.direction = 
    SFG_currentLevel.levelPointer->playerStart[2] *
    (RCL_UNITS_PER_SQUARE / 256);

  SFG_recomputePLayerDirection(); 

  SFG_player.previousVerticalSpeed = 0;

  SFG_player.headBobFrame = 0;

  SFG_player.weapon = SFG_WEAPON_KNIFE;

  SFG_player.weaponCooldownFrames = 0;
  SFG_player.lastHurtFrame = SFG_game.frame;
  SFG_player.lastItemTakenFrame = SFG_game.frame;

  SFG_player.health = SFG_PLAYER_START_HEALTH;

  SFG_player.cards = 
#if SFG_UNLOCK_DOOR
  0x07;
#else
  0;
#endif

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

/**
  Gets sprite (image and sprite size) for given item.
*/
void SFG_getItemSprite(
  uint8_t elementType, const uint8_t **sprite, uint8_t *spriteSize)
{
  *spriteSize = 0;
  *sprite = SFG_itemSprites + (elementType - 1) * SFG_TEXTURE_STORE_SIZE;

  switch (elementType)
  {
    case SFG_LEVEL_ELEMENT_TREE:
    case SFG_LEVEL_ELEMENT_RUIN:
    case SFG_LEVEL_ELEMENT_LAMP:
    case SFG_LEVEL_ELEMENT_TELEPORT:
      *spriteSize = 2;
      break;

    case SFG_LEVEL_ELEMENT_TERMINAL:
      *spriteSize = 1;
      break;

    case SFG_LEVEL_ELEMENT_FINISH:
    case SFG_LEVEL_ELEMENT_COLUMN:
      *spriteSize = 3;
      break;

    case SFG_LEVEL_ELEMENT_CARD0:
    case SFG_LEVEL_ELEMENT_CARD1:
    case SFG_LEVEL_ELEMENT_CARD2:
      *sprite = SFG_itemSprites + 
        (SFG_LEVEL_ELEMENT_CARD0 - 1) * SFG_TEXTURE_STORE_SIZE;
      break;

    case SFG_LEVEL_ELEMENT_BLOCKER:
      *sprite = 0;
      break;

    default:
      break;
  }
}

/**
  Says whether given item type collides, i.e. stops player from moving.
*/
uint8_t SFG_itemCollides(uint8_t elementType)
{
  return 
    elementType == SFG_LEVEL_ELEMENT_BARREL ||
    elementType == SFG_LEVEL_ELEMENT_TREE ||
    elementType == SFG_LEVEL_ELEMENT_TERMINAL ||
    elementType == SFG_LEVEL_ELEMENT_COLUMN ||
    elementType == SFG_LEVEL_ELEMENT_RUIN ||
    elementType == SFG_LEVEL_ELEMENT_BLOCKER ||
    elementType == SFG_LEVEL_ELEMENT_LAMP;
}

void SFG_setGameState(uint8_t state)
{
  SFG_game.state = state;
  SFG_game.stateChangeTime = SFG_game.frameTime;
}

void SFG_setAndInitLevel(uint8_t levelNumber)
{
  SFG_LOG("setting and initializing level");

  const SFG_Level *level;

#if SFG_ARDUINO
  memcpy_P(&SFG_ramLevel,SFG_levels[levelNumber],sizeof(SFG_Level));
  level = &SFG_ramLevel;
#else
  level = SFG_levels[levelNumber];
#endif

  SFG_game.currentRandom = 0;

  if (SFG_game.saved != SFG_CANT_SAVE)
    SFG_game.saved = 0;

  SFG_currentLevel.levelNumber = levelNumber;
  SFG_currentLevel.monstersDead = 0;
  SFG_currentLevel.backgroundImage = level->backgroundImage;
  SFG_currentLevel.levelPointer = level;
  SFG_currentLevel.bossCount = 0;
  SFG_currentLevel.floorColor = level->floorColor;
  SFG_currentLevel.ceilingColor = level->ceilingColor;
  SFG_currentLevel.completionTime10sOfS = 0;

  for (uint8_t i = 0; i < 7; ++i)
    SFG_currentLevel.textures[i] =
      SFG_wallTextures + level->textureIndices[i] * SFG_TEXTURE_STORE_SIZE;

  SFG_LOG("initializing doors");

  SFG_currentLevel.checkedDoorIndex = 0;
  SFG_currentLevel.doorRecordCount = 0;
  SFG_currentLevel.projectileRecordCount = 0;
  SFG_currentLevel.teleportCount = 0;
  SFG_currentLevel.mapRevealMask = 
#if SFG_REVEAL_MAP
    0xffff;
#else
    0;
#endif

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

  for (uint16_t i = 0; i < ((SFG_MAP_SIZE * SFG_MAP_SIZE) / 8); ++i)
    SFG_currentLevel.itemCollisionMap[i] = 0;

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

        monster->stateType = (SFG_MONSTER_TYPE_TO_INDEX(e->type) << 4)
          | SFG_MONSTER_STATE_INACTIVE;
 
        monster->health =
          SFG_GET_MONSTER_MAX_HEALTH(SFG_MONSTER_TYPE_TO_INDEX(e->type));

        monster->coords[0] = e->coords[0] * 4 + 2;
        monster->coords[1] = e->coords[1] * 4 + 2;

        SFG_currentLevel.monsterRecordCount++;

        if (e->type == SFG_LEVEL_ELEMENT_MONSTER_ENDER)
          SFG_currentLevel.bossCount++;
      }
      else if ((e->type < SFG_LEVEL_ELEMENT_LOCK0) ||
        (e->type > SFG_LEVEL_ELEMENT_LOCK2))
      {
        SFG_LOG("adding item");
        SFG_currentLevel.itemRecords[SFG_currentLevel.itemRecordCount] = i;
        SFG_currentLevel.itemRecordCount++;

        if (e->type == SFG_LEVEL_ELEMENT_TELEPORT)
          SFG_currentLevel.teleportCount++;

        if (SFG_itemCollides(e->type))
          SFG_setItemCollisionMapBit(e->coords[0],e->coords[1],1);
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
 
  SFG_setGameState(SFG_GAME_STATE_LEVEL_START);
}

void SFG_init()
{
  SFG_LOG("initializing game")

  SFG_game.frame = 0;
  SFG_game.currentRandom = 0;
  SFG_game.continues = 1;

  RCL_initRayConstraints(&SFG_game.rayConstraints);
  SFG_game.rayConstraints.maxHits = SFG_RAYCASTING_MAX_HITS;
  SFG_game.rayConstraints.maxSteps = SFG_RAYCASTING_MAX_STEPS;

  SFG_game.antiSpam = 0;

  SFG_LOG("computing average texture colors")

  for (uint8_t i = 0; i < SFG_WALL_TEXTURE_COUNT; ++i)
  {
    /** For simplicity, we round colors so that there is only 64 of them, and
      we count them up to 256. */

    uint8_t colorHistogram[64];

    for (uint8_t j = 0; j < 64; ++j)
      colorHistogram[j] = 0;

    for (uint8_t y = 0; y < SFG_TEXTURE_SIZE; ++y)
      for (uint8_t x = 0; x < SFG_TEXTURE_SIZE; ++x)
      {
        uint8_t color =
          SFG_getTexel(SFG_wallTextures + i * SFG_TEXTURE_STORE_SIZE,x,y) / 4;

        colorHistogram[color] += 1;

        if (colorHistogram[color] == 255)
          break;
      }

    uint8_t maxIndex = 0;

    for (uint8_t j = 0; j < 64; ++j)
    {
      if (colorHistogram[j] == 255)
      {
        maxIndex = j;
        break;
      }

      if (colorHistogram[j] > colorHistogram[maxIndex])
        maxIndex = j;
    }

    SFG_game.textureAverageColors[i] = maxIndex * 4;
  }

  for (uint16_t i = 0; i < SFG_GAME_RESOLUTION_Y; ++i)
    SFG_game.backgroundScaleMap[i] =
      (i * SFG_TEXTURE_SIZE) / SFG_GAME_RESOLUTION_Y;

  for (uint8_t i = 0; i < SFG_KEY_COUNT; ++i)
    SFG_game.keyStates[i] = 0;

  SFG_game.backgroundScroll = 0;
  SFG_currentLevel.levelPointer = 0;
  SFG_game.lastFrameTimeMs = SFG_getTimeMs();
  SFG_game.selectedMenuItem = 0;
  SFG_game.selectedLevel = 0;
  SFG_game.settings = 0x03;
  SFG_game.saved = 0;

  // create a default save data:

  for (uint16_t i = 0; i < SFG_SAVE_SIZE; ++i)
    SFG_game.save[i] = 0;
    
  SFG_game.save[0] = 0;
  SFG_game.save[1] = SFG_game.settings;

  SFG_gameLoad(); // attempt to load settings

  if (SFG_game.saved != SFG_CANT_SAVE)
  {
    SFG_LOG("settings loaded");
    SFG_game.settings = SFG_game.save[1]; 
  }
  else
  {
    SFG_LOG("saving/loading not possible");
    SFG_game.save[0] = SFG_NUMBER_OF_LEVELS | 0xf0; // revealed all levels
  }

  SFG_enableMusic(SFG_game.settings & 0x02);

#if SFG_START_LEVEL == 0
  SFG_setGameState(SFG_GAME_STATE_MENU);
#else
  SFG_setAndInitLevel(SFG_START_LEVEL - 1);
#endif
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

  RCL_moveCameraWithCollision(&c,offset,0,SFG_floorCollisionHeightAt,
    SFG_ceilingHeightAt,1,1);

  pos[0] = c.position.x;
  pos[1] = c.position.y;
  pos[2] = c.height;

  return 1;
}

uint8_t SFG_pushPlayerAway(
  RCL_Unit centerX, RCL_Unit centerY, RCL_Unit distance)
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
  RCL_Unit dx = RCL_abs(elementPos.x - position.x);
  RCL_Unit dy = RCL_abs(elementPos.y - position.y);

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

void SFG_playerChangeHealthWithMiltiplier(int8_t healthAdd)
{
  if (healthAdd < 0)
    healthAdd =
      RCL_min(-1,
      (((RCL_Unit) healthAdd) * SFG_PLAYER_DAMAGE_MULTIPLIER) /
      RCL_UNITS_PER_SQUARE);

  SFG_playerChangeHealth(healthAdd);
}

uint8_t SFG_distantSoundVolume(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  RCL_Unit distance = SFG_taxicabDistance(x,y,z,
                        SFG_player.camera.position.x,
                        SFG_player.camera.position.y,
                        SFG_player.camera.height);

  if (distance >= SFG_SFX_MAX_DISTANCE)
    return 0;

  uint32_t result = 255 - (distance * 255) / SFG_SFX_MAX_DISTANCE;

  return (result * result) / 256;
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

  if (healthAdd < 0)
  {
    // play hurt sound
    
    uint8_t volume = SFG_distantSoundVolume( 
      SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
      SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),
      SFG_floorHeightAt(
        SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
        SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1])));
    
      SFG_playGameSound(5,volume);
    
    if (monster->health == 0)
      SFG_playGameSound(2,volume);
  }
}

void SFG_removeItem(uint8_t index)
{
  SFG_LOG("removing item");

  for (uint16_t j = index; j < SFG_currentLevel.itemRecordCount - 1; ++j)
    SFG_currentLevel.itemRecords[j] =
      SFG_currentLevel.itemRecords[j + 1];

  SFG_currentLevel.itemRecordCount--; 
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

static inline const SFG_LevelElement *SFG_getLevelElement(uint8_t index)
{
  SFG_ItemRecord item = SFG_currentLevel.itemRecords[index];

  return &(SFG_currentLevel.levelPointer->elements[item &
           ~SFG_ITEM_RECORD_ACTIVE_MASK]);
}
  
void SFG_createExplosion(RCL_Unit, RCL_Unit, RCL_Unit); // forward decl

void SFG_explodeBarrel(uint8_t itemIndex, RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  const SFG_LevelElement *e = SFG_getLevelElement(itemIndex);
  SFG_setItemCollisionMapBit(e->coords[0],e->coords[1],0);
  SFG_removeItem(itemIndex);
  SFG_createExplosion(x,y,z);
}

void SFG_createExplosion(RCL_Unit x, RCL_Unit y, RCL_Unit z)
{
  SFG_ProjectileRecord explosion;

  SFG_playGameSound(2,SFG_distantSoundVolume(x,y,z));

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

  if (SFG_pushPlayerAway(x,y,SFG_EXPLOSION_PUSH_AWAY_DISTANCE))
    SFG_playerChangeHealthWithMiltiplier(-1 * damage);

  for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
  {
    SFG_MonsterRecord *monster = &(SFG_currentLevel.monsterRecords[i]);

    uint16_t state = SFG_MR_STATE(*monster); 

    if ((state == SFG_MONSTER_STATE_INACTIVE) ||
        (state == SFG_MONSTER_STATE_DEAD))
      continue; 

    RCL_Unit monsterHeight =
      SFG_floorHeightAt(
        SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
        SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1]))
        + RCL_UNITS_PER_SQUARE / 2;

    if (SFG_taxicabDistance(
      SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
      SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),monsterHeight,
      x,y,z) <= SFG_EXPLOSION_RADIUS)
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
        x,y,z,elementX,elementY,elementHeight) <= SFG_EXPLOSION_RADIUS)
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

  uint8_t monsterSquare[2] =
    {
      SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
      SFG_MONSTER_COORD_TO_SQUARES(monster->coords[1])
    };

  RCL_Unit currentHeight =
    SFG_floorCollisionHeightAt(monsterSquare[0],monsterSquare[1]);

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

        dir.x = SFG_player.camera.position.x - pos.x
          - 128 * SFG_MONSTER_AIM_RANDOMNESS + 
          SFG_random() * SFG_MONSTER_AIM_RANDOMNESS;

        dir.y = SFG_player.camera.position.y - pos.y
          - 128 * SFG_MONSTER_AIM_RANDOMNESS + 
          SFG_random() * SFG_MONSTER_AIM_RANDOMNESS;

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
          SFG_playGameSound(0,
            SFG_distantSoundVolume( 
              SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
              SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),
              currentHeight)
            );

        SFG_launchProjectile(
          projectile,
          pos,
          currentHeight + RCL_UNITS_PER_SQUARE / 2,
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

        if (monsterSquare[0] > SFG_player.squarePosition[0])
        {
          if (monsterSquare[1] > SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_NW;
          else if (monsterSquare[1] < SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_SW;
          else
            state = SFG_MONSTER_STATE_GOING_W;
        }
        else if (monsterSquare[0] < SFG_player.squarePosition[0])
        {
          if (monsterSquare[1] > SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_NE;
          else if (monsterSquare[1] < SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_SE;
          else
            state = SFG_MONSTER_STATE_GOING_E;
        }
        else
        {
          if (monsterSquare[1] > SFG_player.squarePosition[1])
            state = SFG_MONSTER_STATE_GOING_N;
          else if (monsterSquare[1] < SFG_player.squarePosition[1])
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

          SFG_playerChangeHealthWithMiltiplier(
            -1 * SFG_getDamageValue(SFG_WEAPON_FIRE_TYPE_MELEE)); 
              
          SFG_playGameSound(3,255);
        }
        else // SFG_MONSTER_ATTACK_EXPLODE
        {
          // explode

          SFG_createExplosion(pX,pY,pZ);
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

    if ((coordAdd[0] != 0 || coordAdd[1] != 0) && SFG_random() <
        SFG_MONSTER_SOUND_PROBABILITY)
      SFG_playGameSound(5,
          SFG_distantSoundVolume( 
          SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
          SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),
          currentHeight) / 2);

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
    RCL_Unit newHeight =
      SFG_floorCollisionHeightAt(newPos[0] / 4,newPos[1] / 4);

    collision =
      RCL_abs(currentHeight - newHeight) > RCL_CAMERA_COLL_STEP_HEIGHT;

    if (!collision)
      collision = 
        (SFG_ceilingHeightAt(newPos[0] / 4,newPos[1] / 4) - newHeight) <
        SFG_MONSTER_COLLISION_HEIGHT;
  }

  if (collision)
  {
    state = SFG_MONSTER_STATE_IDLE;
    // ^ will force the monster to choose random direction in the next update
 
    newPos[0] = monster->coords[0];
    newPos[1] = monster->coords[1];
  }

  monster->stateType = state | (monsterNumber << 4);
  monster->coords[0] = newPos[0];
  monster->coords[1] = newPos[1];;
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
    <= SFG_ELEMENT_COLLISION_RADIUS;
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

/**
  Updates a frame of the currently loaded level, i.e. enemies, projectiles,
  aimations etc., with the exception of player.
*/
void SFG_updateLevel()
{
  // update projectiles:

  uint8_t substractFrames =
    (SFG_game.frame - SFG_currentLevel.frameStart) & 0x01 ? 1 : 0;
    /* ^ only substract frames to live every other frame because a maximum of
       256 frames would be too few */

  for (int8_t i = 0; i < SFG_currentLevel.projectileRecordCount; ++i)
  { // ^ has to be signed
    SFG_ProjectileRecord *p = &(SFG_currentLevel.projectileRecords[i]);

    uint8_t attackType = 255;

    if (p->type == SFG_PROJECTILE_BULLET)
      attackType = SFG_WEAPON_FIRE_TYPE_BULLET;
    else if (p->type == SFG_PROJECTILE_PLASMA)
      attackType = SFG_WEAPON_FIRE_TYPE_PLASMA;

    RCL_Unit pos[3] = {0,0,0}; /* we have to convert from uint16_t because of
                                  under/overflows */
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
          SFG_playerChangeHealthWithMiltiplier(-1 * SFG_getDamageValue(attackType));
        }

      /* Check collision with the map (we don't use SFG_floorCollisionHeightAt
         because collisions with items have to be done differently for
         projectiles). */

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

          uint8_t state = SFG_MR_STATE(*m);

          if ((state != SFG_MONSTER_STATE_INACTIVE) &&
              (state != SFG_MONSTER_STATE_DEAD))
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

      if (!eliminate) // items (can't check itemCollisionMap because of barrels)
        for (uint16_t j = 0; j < SFG_currentLevel.itemRecordCount; ++j)
        {
          const SFG_LevelElement *e = SFG_getActiveItemElement(j);

          if (e != 0 && SFG_itemCollides(e->type))
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
        SFG_playGameSound(4,SFG_distantSoundVolume(pos[0],pos[1],pos[2]));

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
    /* Check door on whether a player is standing nearby. For performance
       reasons we only check a few doors and move to others in the next
       frame. */
   
    if (SFG_currentLevel.checkedDoorIndex == 0)
    {
      uint8_t count = SFG_player.cards >> 6;

      SFG_player.cards = (count <= 1) ?
        (SFG_player.cards & 0x07) :
        ((SFG_player.cards & 0x7f) | ((count - 1) << 6));
    }
 
    for (uint16_t i = 0;
         i < RCL_min(SFG_ELEMENT_DISTANCES_CHECKED_PER_FRAME,
           SFG_currentLevel.doorRecordCount);
         ++i) 
    {
      SFG_DoorRecord *door =
        &(SFG_currentLevel.doorRecords[SFG_currentLevel.checkedDoorIndex]);

      uint8_t upDownState = door->state & SFG_DOOR_UP_DOWN_MASK;

      uint8_t newUpDownState = 0;
      
      uint8_t lock = SFG_DOOR_LOCK(door->state);

      if ( // player near door?
        (door->coords[0] >= (SFG_player.squarePosition[0] - 1)) &&
        (door->coords[0] <= (SFG_player.squarePosition[0] + 1)) &&
        (door->coords[1] >= (SFG_player.squarePosition[1] - 1)) &&
        (door->coords[1] <= (SFG_player.squarePosition[1] + 1)))
      {
        if (lock == 0)
        {
          newUpDownState = SFG_DOOR_UP_DOWN_MASK;    
        }
        else
        {
          lock = 1 << (lock - 1);

          if (SFG_player.cards & lock) // player has the card?
            newUpDownState = SFG_DOOR_UP_DOWN_MASK;
          else
            SFG_player.cards = 
              (SFG_player.cards & 0x07) | (lock << 3) | (2 << 6);
        }
      }

      if (upDownState != newUpDownState)
        SFG_playGameSound(1,255);

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
          (monster->health != 0 ? 
            SFG_MONSTER_STATE_IDLE : SFG_MONSTER_STATE_DEAD);
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

      if ((state == SFG_MONSTER_STATE_INACTIVE) || 
          (state == SFG_MONSTER_STATE_DEAD))
        continue;

      if (state == SFG_MONSTER_STATE_DYING)
      {
        if (SFG_MR_TYPE(*monster) == SFG_LEVEL_ELEMENT_MONSTER_ENDER)
        {
          SFG_currentLevel.bossCount--;

          // last boss killed gives player a key card

          if (SFG_currentLevel.bossCount == 0)
          {
            SFG_LOG("boss killed, giving player a card");
            SFG_player.cards |= 0x04;
          }
        }

        monster->stateType =
          (monster->stateType & 0xf0) | SFG_MONSTER_STATE_DEAD;
      }
      else if (monster->health == 0)
      {
        monster->stateType = (monster->stateType & SFG_MONSTER_MASK_TYPE) |
          SFG_MONSTER_STATE_DYING;

        if (SFG_MR_TYPE(*monster) == SFG_LEVEL_ELEMENT_MONSTER_EXPLODER)
          SFG_createExplosion(
            SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[0]),
            SFG_MONSTER_COORD_TO_RCL_UNITS(monster->coords[1]),
            SFG_floorCollisionHeightAt(
              SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0]),
              SFG_MONSTER_COORD_TO_SQUARES(monster->coords[0])) +
            RCL_UNITS_PER_SQUARE / 2);
      }
      else
      {
#if SFG_PREVIEW_MODE == 0
        SFG_monsterPerformAI(monster);
#endif
      }
    }
  }
}

/**
  Maps square position on the map to a bit in map reveal mask.
*/
static inline uint16_t SFG_getMapRevealBit(uint8_t squareX, uint8_t squareY)
{
  return 1 << ((squareY / 16) * 4 + squareX / 16);
}

/**
  Checks a 3D point visibility from player's position (WITHOUT considering
  facing direction).
*/
static inline uint8_t SFG_spriteIsVisible(RCL_Vector2D pos, RCL_Unit height,
  uint8_t spriteSize)
{
  return
    RCL_castRay3D(
      SFG_player.camera.position,
      SFG_player.camera.height,
      pos,
      height,
      SFG_floorHeightAt,
      SFG_ceilingHeightAt,
      SFG_game.rayConstraints
    ) == RCL_UNITS_PER_SQUARE;
}

/**
  Returns a tangent in RCL_Unit of vertical autoaim, given current game state.
*/
RCL_Unit SFG_autoaimVertically()
{
  for (uint16_t i = 0; i < SFG_currentLevel.monsterRecordCount; ++i)
  {
    SFG_MonsterRecord m = SFG_currentLevel.monsterRecords[i];
      
    if (SFG_MR_STATE(m) == SFG_MONSTER_STATE_INACTIVE)
      continue;

    RCL_Vector2D worldPosition, toMonster;

    worldPosition.x = SFG_MONSTER_COORD_TO_RCL_UNITS(m.coords[0]);
    worldPosition.y = SFG_MONSTER_COORD_TO_RCL_UNITS(m.coords[1]);
    
    toMonster.x = worldPosition.x - SFG_player.camera.position.x;
    toMonster.y = worldPosition.y - SFG_player.camera.position.y;

    if (RCL_abs(
         RCL_vectorsAngleCos(SFG_player.direction,toMonster) 
         - RCL_UNITS_PER_SQUARE) < SFG_VERTICAL_AUTOAIM_ANGLE_THRESHOLD)
    {
      uint8_t spriteSize = SFG_GET_MONSTER_SPRITE_SIZE(
        SFG_MONSTER_TYPE_TO_INDEX(SFG_MR_TYPE(m)));
        
      RCL_Unit worldHeight = 
        SFG_floorHeightAt(
          SFG_MONSTER_COORD_TO_SQUARES(m.coords[0]),
          SFG_MONSTER_COORD_TO_SQUARES(m.coords[1]))
          + 
          SFG_SPRITE_SIZE_TO_HEIGHT_ABOVE_GROUND(spriteSize);
        
      if (SFG_spriteIsVisible(worldPosition,worldHeight,spriteSize))
      {
        RCL_Unit distance = RCL_len(toMonster);
 
        return ((worldHeight - SFG_player.camera.height) * RCL_UNITS_PER_SQUARE)
          / distance;
      }
    }
  }

  return 0;
}

/**
  Draws text on screen using the bitmap font stored in assets.
*/
void SFG_drawText(
  const char *text,
  uint16_t x,
  uint16_t y,
  uint8_t size,
  uint8_t color,
  uint16_t maxLength,
  uint16_t limitX)
{
  if (size == 0)
    size = 1;

  if (limitX == 0)
    limitX = 65535;

  if (maxLength == 0)
    maxLength = 65535;

  uint16_t pos = 0;

  uint16_t currentX = x;
  uint16_t currentY = y;

  while (text[pos] != 0 && pos < maxLength) // for each character
  {
    uint16_t character = SFG_font[SFG_charToFontIndex(text[pos])];

    for (uint8_t i = 0; i < SFG_FONT_CHARACTER_SIZE; ++i) // for each line
    {
      currentY = y;

      for (uint8_t j = 0; j < SFG_FONT_CHARACTER_SIZE; ++j) // for each row
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
    }
    
    currentX += size; // space
      
    if (currentX > limitX)
    {
      currentX = x;
      y += (SFG_FONT_CHARACTER_SIZE + 1) * size;
    }

    pos++;    
  }
}

void SFG_drawLevelStartOverlay()
{
  uint8_t stage = ((SFG_game.frameTime - SFG_game.stateChangeTime) * 4) /
    SFG_LEVEL_START_DURATION;

  // fade in:

  for (uint16_t y = 0; y < SFG_GAME_RESOLUTION_Y; ++y)
    for (uint16_t x = 0; x < SFG_GAME_RESOLUTION_X; ++x)
    {
      uint8_t draw = 0;

      switch (stage)
      {
        case 0: draw = 1; break;
        case 1: draw = (x % 2) || (y % 2); break;
        case 2: draw = (x % 2) == (y % 2); break;
        case 3: draw = (x % 2) && (y % 2); break;
        default: break;
      }

      if (draw)
        SFG_setGamePixel(x,y,0);
    }

  if (SFG_game.saved == 1)
    SFG_drawText(SFG_TEXT_SAVED,SFG_HUD_MARGIN,SFG_HUD_MARGIN,
      SFG_FONT_SIZE_MEDIUM,7,255,0);
}

/**
  Part of SFG_gameStep() for SFG_GAME_STATE_PLAYING.
*/
void SFG_gameStepPlaying()
{
  if (
    (SFG_keyIsDown(SFG_KEY_C) && SFG_keyIsDown(SFG_KEY_DOWN)) ||
    SFG_keyIsDown(SFG_KEY_MENU))
  {
    SFG_setGameState(SFG_GAME_STATE_MENU);
    return;
  }

  SFG_updateLevel();

  int8_t recomputeDirection = SFG_currentLevel.frameStart == SFG_game.frame;

  RCL_Vector2D moveOffset;

  moveOffset.x = 0;
  moveOffset.y = 0;

  int8_t strafe = 0;

  uint8_t currentWeapon = SFG_player.weapon;

#if SFG_HEADBOB_ENABLED
  int8_t bobbing = 0;
#endif

  int8_t shearing = 0;

  if (SFG_player.weaponCooldownFrames > 0)
    SFG_player.weaponCooldownFrames--;

  if (SFG_keyJustPressed(SFG_KEY_TOGGLE_FREELOOK))
    SFG_game.settings = (SFG_game.settings & 0x04) ?
      (SFG_game.settings & ~0x0c) : (SFG_game.settings | 0x0c );

  int8_t canSwitchWeapon = SFG_player.weaponCooldownFrames == 0;

  if (SFG_keyJustPressed(SFG_KEY_NEXT_WEAPON) && canSwitchWeapon)
    SFG_playerRotateWeapon(1);
  else if (SFG_keyJustPressed(SFG_KEY_PREVIOUS_WEAPON) && canSwitchWeapon)
    SFG_playerRotateWeapon(0);

  uint8_t shearingOn = SFG_game.settings & 0x04;

  if (SFG_keyIsDown(SFG_KEY_A))
  {
    if (shearingOn)                      // A + U/D: shearing (if on)
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
    }

    if (!SFG_keyIsDown(SFG_KEY_C))
    {                                    // A + L/R: strafing
      if (SFG_keyIsDown(SFG_KEY_LEFT))
        strafe = -1;
      else if (SFG_keyIsDown(SFG_KEY_RIGHT))
        strafe = 1;
    }
  }

  if (SFG_keyIsDown(SFG_KEY_C))          // C + AL/BR: weapon switching
  {
    if ((SFG_keyJustPressed(SFG_KEY_LEFT) || SFG_keyJustPressed(SFG_KEY_A)) &&
      canSwitchWeapon)
      SFG_playerRotateWeapon(0);
    else if (
      (SFG_keyJustPressed(SFG_KEY_RIGHT) || SFG_keyJustPressed(SFG_KEY_B)) &&
      canSwitchWeapon)
      SFG_playerRotateWeapon(1);
  }
  else if (!SFG_keyIsDown(SFG_KEY_A))    // L/R: turning
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

  if (!SFG_keyIsDown(SFG_KEY_A) || !shearingOn)     // U/D: movement
  {
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

  int16_t mouseX = 0, mouseY = 0;

  SFG_getMouseOffset(&mouseX,&mouseY);

  if (mouseX != 0)                                  // mouse turning
  {
    SFG_player.camera.direction += 
      (mouseX * SFG_MOUSE_SENSITIVITY_HORIZONTAL) / 128;

    recomputeDirection = 1;
  }

  if ((mouseY != 0) && shearingOn)                  // mouse shearing
    SFG_player.camera.shear =
      RCL_max(RCL_min(
        SFG_player.camera.shear - 
        (mouseY * SFG_MOUSE_SENSITIVITY_VERTICAL) / 128,
        SFG_CAMERA_MAX_SHEAR_PIXELS),
        -1 * SFG_CAMERA_MAX_SHEAR_PIXELS);

  if (recomputeDirection)
    SFG_recomputePLayerDirection();

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
    SFG_PLAYER_JUMP_OFFSET_PER_FRAME : // jump
    (SFG_player.verticalSpeed - SFG_GRAVITY_SPEED_INCREASE_PER_FRAME);
#endif

  if (!shearing && SFG_player.camera.shear != 0 && !(SFG_game.settings & 0x08))
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

    /* When in quadrant in which sin is going away from zero, switch to the
       same value of the next quadrant, so that bobbing starts to go towards
       zero immediately. */

    if (quadrant % 2 == 0)
      SFG_player.headBobFrame =
        ((quadrant + 1) * RCL_UNITS_PER_SQUARE / 4) +
        (RCL_UNITS_PER_SQUARE / 4 - SFG_player.headBobFrame %
        (RCL_UNITS_PER_SQUARE / 4));

    RCL_Unit currentFrame = SFG_player.headBobFrame;
    RCL_Unit nextFrame = SFG_player.headBobFrame + 16;

    // only stop bobbing when we pass a frame at which sin crosses zero
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

    uint8_t state = SFG_MR_STATE(*m);

    if (state == SFG_MONSTER_STATE_INACTIVE || state == SFG_MONSTER_STATE_DEAD)
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

  /* item collisions with player (only those that don't stop player's movement,
     as those are handled differently, via itemCollisionMap): */
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

      if (!SFG_itemCollides(e->type) &&
          SFG_elementCollides(
            SFG_player.camera.position.x,
            SFG_player.camera.position.y,
            SFG_player.camera.height,
            ePos.x,
            ePos.y,
            SFG_floorHeightAt(e->coords[0],e->coords[1]))
         )
      {
        uint8_t eliminate = 1;

        uint8_t onlyKnife = 1;

        for (uint8_t i = 0; i < SFG_AMMO_TOTAL; ++i)
          if (SFG_player.ammo[i] != 0)
          {
            onlyKnife = 0;
            break;
          }

        switch (e->type)
        {
          case SFG_LEVEL_ELEMENT_HEALTH:
            if (SFG_player.health < SFG_PLAYER_MAX_HEALTH)
              SFG_playerChangeHealth(SFG_HEALTH_KIT_VALUE);
            else
              eliminate = 0;
            break;

#define addAmmo(type) \
  if (SFG_player.ammo[SFG_AMMO_##type] < SFG_AMMO_MAX_##type) \
  {\
    SFG_player.ammo[SFG_AMMO_##type] = RCL_min(SFG_AMMO_MAX_##type,\
      SFG_player.ammo[SFG_AMMO_##type] + SFG_AMMO_INCREASE_##type);\
    if (onlyKnife) SFG_playerRotateWeapon(1); \
  }\
  else\
    eliminate = 0;

          case SFG_LEVEL_ELEMENT_BULLETS:
            addAmmo(BULLETS)
            break;

          case SFG_LEVEL_ELEMENT_ROCKETS:
            addAmmo(ROCKETS)
            break;

          case SFG_LEVEL_ELEMENT_PLASMA:
            addAmmo(PLASMA)
            break;

#undef addAmmo

          case SFG_LEVEL_ELEMENT_CARD0:
          case SFG_LEVEL_ELEMENT_CARD1:
          case SFG_LEVEL_ELEMENT_CARD2:
            SFG_player.cards |= 1 << (e->type - SFG_LEVEL_ELEMENT_CARD0);
            break;

          case SFG_LEVEL_ELEMENT_TELEPORT:
            collidesWithTeleport = 1;
            eliminate = 0;
            break;

          case SFG_LEVEL_ELEMENT_FINISH:
            SFG_levelEnds();
            SFG_playGameSound(2,255); 
            SFG_setGameState(SFG_GAME_STATE_WIN);
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
          SFG_playGameSound(3,255);
#endif
        }
        else if (
          e->type == SFG_LEVEL_ELEMENT_TELEPORT &&
          SFG_currentLevel.teleportCount > 1 &&
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

              SFG_playGameSound(4,255);

              break;
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
    verticalOffset,SFG_floorCollisionHeightAt,SFG_ceilingHeightAt,1,1);

  SFG_player.previousVerticalSpeed = SFG_player.verticalSpeed;

  RCL_Unit limit = RCL_max(RCL_max(0,verticalOffset),SFG_player.verticalSpeed);
  
  SFG_player.verticalSpeed =
    RCL_min(limit,SFG_player.camera.height - previousHeight);
  /* ^ By "limit" we assure height increase caused by climbing a step doesn't
     add vertical velocity. */
#endif

#if SFG_PREVIEW_MODE == 0
  if (
    SFG_keyIsDown(SFG_KEY_B) &&
    !SFG_keyIsDown(SFG_KEY_C) &&
    (SFG_player.weaponCooldownFrames == 0))
  {
    /* Player attack/shoot/fire, this has to be done AFTER the player is moved,
       otherwise he could shoot himself while running forward. */

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
        SFG_playGameSound(sound,255);

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
          
        RCL_Unit projectileSpeed = SFG_GET_PROJECTILE_SPEED_UPS(projectile);
        
        /* Vertical speed will be either determined by autoaim (if shearing is
           off) or the camera shear value. */
        RCL_Unit verticalSpeed = (SFG_game.settings & 0x04) ?
          (SFG_player.camera.shear * projectileSpeed) / 
            SFG_CAMERA_MAX_SHEAR_PIXELS
          :
          (projectileSpeed * SFG_autoaimVertically()) / RCL_UNITS_PER_SQUARE;

        for (uint8_t i = 0; i < projectileCount; ++i)
        {
          SFG_launchProjectile(
            projectile,
            SFG_player.camera.position,
            SFG_player.camera.height,
            RCL_angleToDirection(direction),
            verticalSpeed,  
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

          uint8_t state = SFG_MR_STATE(*m);

          if ((state == SFG_MONSTER_STATE_INACTIVE) || 
              (state == SFG_MONSTER_STATE_DEAD))
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

            break;
          }
        }
      }

      SFG_player.weaponCooldownFrames =
        RCL_max(
          SFG_GET_WEAPON_FIRE_COOLDOWN_FRAMES(SFG_player.weapon),
          SFG_MIN_WEAPON_COOLDOWN_FRAMES);

      SFG_getPlayerWeaponInfo(&ammo,&projectileCount,&canShoot);

      if (!canShoot)
      {
        // No more ammo, switch to the second strongest weapon.

        SFG_playerRotateWeapon(1);

        uint8_t previousWeapon = SFG_player.weapon;

        SFG_playerRotateWeapon(0);

        if (previousWeapon > SFG_player.weapon)
          SFG_playerRotateWeapon(1);
      }
    } // endif: has enough ammo?
  } // attack
#endif // SFG_PREVIEW_MODE == 0

  SFG_player.squarePosition[0] =
    SFG_player.camera.position.x / RCL_UNITS_PER_SQUARE;

  SFG_player.squarePosition[1] =
    SFG_player.camera.position.y / RCL_UNITS_PER_SQUARE;

  SFG_currentLevel.mapRevealMask |= 
    SFG_getMapRevealBit(
      SFG_player.squarePosition[0],
      SFG_player.squarePosition[1]);

  if ( // squeezer check
    (SFG_ceilingHeightAt(
       SFG_player.squarePosition[0],SFG_player.squarePosition[1]) -
     SFG_floorHeightAt(
       SFG_player.squarePosition[0],SFG_player.squarePosition[1]))
     <
     (RCL_CAMERA_COLL_HEIGHT_ABOVE + RCL_CAMERA_COLL_HEIGHT_BELOW))
  {
    SFG_LOG("player is squeezed");
    SFG_player.health = 0;
  }

  if (SFG_player.weapon != currentWeapon) // if weapon switched, start cooldown
    SFG_player.weaponCooldownFrames =
      SFG_GET_WEAPON_FIRE_COOLDOWN_FRAMES(SFG_player.weapon) / 2;

#if SFG_IMMORTAL == 0
  if (SFG_player.health == 0)
  {
    SFG_LOG("player dies");
    SFG_levelEnds();
    SFG_setGameState(SFG_GAME_STATE_LOSE);
  }
#endif
}

/**
  This function defines which items are displayed in the menu.
*/
uint8_t SFG_getMenuItem(uint8_t index)
{
  uint8_t current = 0;

  while (1) // find first legitimate item
  {
    if ( // skip non-legitimate items
      ((current <= SFG_MENU_ITEM_MAP) && (SFG_currentLevel.levelPointer == 0))
      || ((current == SFG_MENU_ITEM_LOAD) && ((SFG_game.save[0] >> 4) == 0x0f)))
    {
      current++;
      continue;
    }

    if (index == 0)
      return (current <= (SFG_MENU_ITEM_EXIT - (SFG_CAN_EXIT ? 0 : 1))
        ) ? current : SFG_MENU_ITEM_NONE;

    current++;
    index--;
  }

  return SFG_MENU_ITEM_NONE;
}

void SFG_gameStepMenu()
{
  uint8_t menuItems = 0;

  while (SFG_getMenuItem(menuItems) != SFG_MENU_ITEM_NONE)
    menuItems++;

  uint8_t item = SFG_getMenuItem(SFG_game.selectedMenuItem);

  if (SFG_keyRegisters(SFG_KEY_DOWN) && 
    (SFG_game.selectedMenuItem < menuItems - 1))
  {
    SFG_game.selectedMenuItem++;
    SFG_playGameSound(3,SFG_MENU_CLICK_VOLUME);
  }
  else if (SFG_keyRegisters(SFG_KEY_UP) && (SFG_game.selectedMenuItem > 0))
  {
    SFG_game.selectedMenuItem--;
    SFG_playGameSound(3,SFG_MENU_CLICK_VOLUME);
  }
  else if (SFG_keyJustPressed(SFG_KEY_A))
  {
    switch (item)
    {
      case SFG_MENU_ITEM_PLAY:
        for (uint8_t i = 6; i < SFG_SAVE_SIZE; ++i) // reset totals in save
          SFG_game.save[i] = 0;

        if (SFG_game.selectedLevel == 0)
          SFG_setGameState(SFG_GAME_STATE_INTRO);
        else
          SFG_setAndInitLevel(SFG_game.selectedLevel);

        break;

      case SFG_MENU_ITEM_LOAD:
      {
        SFG_gameLoad();

        uint8_t saveBackup[SFG_SAVE_SIZE];

        for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
          saveBackup[i] = SFG_game.save[i];

        SFG_setAndInitLevel(SFG_game.save[0] >> 4);

        for (uint8_t i = 0; i < SFG_SAVE_SIZE; ++i)
          SFG_game.save[i] = saveBackup[i];

        SFG_player.health = SFG_game.save[2];
        SFG_player.ammo[0] = SFG_game.save[3];
        SFG_player.ammo[1] = SFG_game.save[4];
        SFG_player.ammo[2] = SFG_game.save[5];

        SFG_playerRotateWeapon(0); // this chooses weapon with ammo available
        break;
      }

      case SFG_MENU_ITEM_CONTINUE:
        SFG_setGameState(SFG_GAME_STATE_PLAYING);
        break;

      case SFG_MENU_ITEM_MAP:
        SFG_setGameState(SFG_GAME_STATE_MAP);
        break;

      case SFG_MENU_ITEM_SOUND:
        SFG_LOG("sound changed");

        SFG_game.settings = 
          (SFG_game.settings & ~0x03) | ((SFG_game.settings + 1)  & 0x03);

        SFG_playGameSound(3,SFG_MENU_CLICK_VOLUME);

        if ((SFG_game.settings & 0x02) !=
            ((SFG_game.settings - 1) & 0x02))
            SFG_enableMusic(SFG_game.settings & 0x02);

        SFG_game.save[1] = SFG_game.settings;
        SFG_gameSave();

        break;

      case SFG_MENU_ITEM_SHEAR:
      {
        uint8_t current = (SFG_game.settings >> 2) & 0x03;

        current++;

        if (current == 2) // option that doesn't make sense, skip
          current++;

        SFG_game.settings = 
          (SFG_game.settings & ~0x0c) | ((current & 0x03) << 2);

        SFG_game.save[1] = SFG_game.settings;
        SFG_gameSave();

        break;
      }

      case SFG_MENU_ITEM_EXIT:
        SFG_game.continues = 0;
        break;

      default:
        break;
    }
  }
  else if (item == SFG_MENU_ITEM_PLAY)
  {
    if (SFG_keyRegisters(SFG_KEY_RIGHT) && 
      (SFG_game.selectedLevel < (SFG_game.save[0] & 0x0f)))
    {
      SFG_game.selectedLevel++;
      SFG_playGameSound(3,SFG_MENU_CLICK_VOLUME);
    }
    else if (SFG_keyRegisters(SFG_KEY_LEFT) && SFG_game.selectedLevel > 0)
    {
      SFG_game.selectedLevel--;
      SFG_playGameSound(3,SFG_MENU_CLICK_VOLUME);
    }
  }
}

/**
  Performs one game step (logic, physics, menu, ...), happening SFG_MS_PER_FRAME
  after the previous step.
*/
void SFG_gameStep()
{
  SFG_game.soundsPlayedThisFrame = 0;
  
  SFG_game.blink = (SFG_game.frame / SFG_BLINK_PERIOD_FRAMES) % 2;

  for (uint8_t i = 0; i < SFG_KEY_COUNT; ++i)
    if (!SFG_keyPressed(i))
      SFG_game.keyStates[i] = 0;
    else if (SFG_game.keyStates[i] < 255)
      SFG_game.keyStates[i]++;

  if ((SFG_currentLevel.frameStart - SFG_game.frame) %
      SFG_SPRITE_ANIMATION_FRAME_DURATION == 0)
    SFG_game.spriteAnimationFrame++;

  switch (SFG_game.state)
  {
    case SFG_GAME_STATE_PLAYING:
      SFG_gameStepPlaying();
      break;

    case SFG_GAME_STATE_MENU: 
      SFG_gameStepMenu();
      break;

    case SFG_GAME_STATE_LOSE:
    { 
      // player die animation (lose)

      SFG_updateLevel(); // let monsters and other things continue moving
    
      int32_t t = SFG_game.frameTime - SFG_game.stateChangeTime;

      RCL_Unit h = SFG_floorHeightAt(SFG_player.squarePosition[0],
        SFG_player.squarePosition[1]); 

      SFG_player.camera.height = 
        RCL_max(h,h + ((SFG_LOSE_ANIMATION_DURATION - t) *
            RCL_CAMERA_COLL_HEIGHT_BELOW) / SFG_LOSE_ANIMATION_DURATION);

      SFG_player.camera.shear = 
        RCL_min(SFG_CAMERA_MAX_SHEAR_PIXELS / 4,
        (t * (SFG_CAMERA_MAX_SHEAR_PIXELS / 4)) / SFG_LOSE_ANIMATION_DURATION);

      if (t > SFG_LOSE_ANIMATION_DURATION && 
        (SFG_keyIsDown(SFG_KEY_A) || SFG_keyIsDown(SFG_KEY_B)))
      {
        for (uint8_t i = 6; i < SFG_SAVE_SIZE; ++i)
          SFG_game.save[i] = 0;

        SFG_setAndInitLevel(SFG_currentLevel.levelNumber);
      }

      break;
    }

    case SFG_GAME_STATE_WIN:
    {
      // win animation
     
      SFG_updateLevel();

      int32_t t = SFG_game.frameTime - SFG_game.stateChangeTime;

      if (t > SFG_WIN_ANIMATION_DURATION)
      {
        if (SFG_currentLevel.levelNumber == (SFG_NUMBER_OF_LEVELS - 1))
        {
          if (SFG_keyIsDown(SFG_KEY_A))
            SFG_setGameState(SFG_GAME_STATE_OUTRO);
        }
        else if (SFG_keyIsDown(SFG_KEY_RIGHT) ||
            SFG_keyIsDown(SFG_KEY_LEFT))
        {
          SFG_setAndInitLevel(SFG_currentLevel.levelNumber + 1);
          
          SFG_player.health = SFG_game.save[2];
          SFG_player.ammo[0] = SFG_game.save[3];
          SFG_player.ammo[1] = SFG_game.save[4];
          SFG_player.ammo[2] = SFG_game.save[5];

          if (SFG_keyIsDown(SFG_KEY_RIGHT) && SFG_game.saved != SFG_CANT_SAVE)
          {
            // save the current position
            SFG_gameSave();
            SFG_game.saved = 1;
          }
        }
      }

      break;
    }

    case SFG_GAME_STATE_MAP:
      if (SFG_keyIsDown(SFG_KEY_B))
        SFG_setGameState(SFG_GAME_STATE_MENU);

      break;

    case SFG_GAME_STATE_INTRO:
      if (SFG_keyJustPressed(SFG_KEY_A) || SFG_keyJustPressed(SFG_KEY_B))
        SFG_setAndInitLevel(0);

      break;

    case SFG_GAME_STATE_OUTRO:
      if (((SFG_game.frameTime - SFG_game.stateChangeTime) > 
           SFG_STORYTEXT_DURATION) && (SFG_keyIsDown(SFG_KEY_A) ||
           SFG_keyIsDown(SFG_KEY_B)))
        SFG_setGameState(SFG_GAME_STATE_MENU);

      break;

    case SFG_GAME_STATE_LEVEL_START:
    {
      SFG_updateLevel();

      if (SFG_currentLevel.levelNumber > (SFG_game.save[0] & 0x0f))
      {
        SFG_game.save[0] = // save progress
          (SFG_game.save[0] & 0xf0) | SFG_currentLevel.levelNumber;
        SFG_gameSave();
      }

      int16_t x = 0, y = 0;
      
      SFG_getMouseOffset(&x,&y); // this keeps centering the mouse

      if ((SFG_game.frameTime - SFG_game.stateChangeTime) >= 
        SFG_LEVEL_START_DURATION)     
        SFG_setGameState(SFG_GAME_STATE_PLAYING);

      break;
    }

    default:
      break;
  }
}

void SFG_fillRectangle(
  uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color)
{
  if ((x + width > SFG_GAME_RESOLUTION_X) ||
      (y + height > SFG_GAME_RESOLUTION_Y))
    return;

  for (uint16_t j = y; j < y + height; ++j)
    for (uint16_t i = x; i < x + width; ++i)
      SFG_setGamePixel(i,j,color);
}

static inline void SFG_clearScreen(uint8_t color)
{
  SFG_fillRectangle(0,0,SFG_GAME_RESOLUTION_X,
    SFG_GAME_RESOLUTION_Y,color);
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

  uint8_t playerColor = 
    SFG_game.blink ? SFG_MAP_PLAYER_COLOR1 : SFG_MAP_PLAYER_COLOR2;

  for (int16_t j = 0; j < maxJ; ++j)
  {
    x = topLeftX;

    for (int16_t i = maxI - 1; i >= 0; --i)
    {
      uint8_t color = 0; // init with non-revealed color

      if (SFG_currentLevel.mapRevealMask & SFG_getMapRevealBit(i,j)) 
      {
        uint8_t properties;

        SFG_TileDefinition tile =
          SFG_getMapTile(SFG_currentLevel.levelPointer,i,j,&properties);

        color = playerColor; // start with player color

        if (i != SFG_player.squarePosition[0] ||
            j != SFG_player.squarePosition[1])
        {
          if (properties == SFG_TILE_PROPERTY_ELEVATOR)
            color = SFG_MAP_ELEVATOR_COLOR;
          else if (properties == SFG_TILE_PROPERTY_SQUEEZER)
            color = SFG_MAP_SQUEEZER_COLOR;
          else if (properties == SFG_TILE_PROPERTY_DOOR)
            color = SFG_MAP_DOOR_COLOR;
          else
          {
            color = 0;

            uint8_t c = SFG_TILE_CEILING_HEIGHT(tile) / 4;

            if (c != 0)
              color = (SFG_TILE_FLOOR_HEIGHT(tile) % 8 + 3) * 8 + c - 1;
          }
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
  Draws fullscreen story text (intro/outro).
*/
void SFG_drawStoryText()
{
  const char *text = SFG_outroText;
  uint16_t textColor = 23;
  uint8_t clearColor = 9;
  uint8_t sprite = 18;

  if (SFG_currentLevel.levelNumber != (SFG_NUMBER_OF_LEVELS - 1)) // intro?  
  {
    text = SFG_introText;
    textColor = 7; 
    clearColor = 0;
    sprite = SFG_game.blink * 2;
  }
    
  SFG_clearScreen(clearColor);

  SFG_blitImage(SFG_monsterSprites + sprite * SFG_TEXTURE_STORE_SIZE,
      (SFG_GAME_RESOLUTION_X - SFG_TEXTURE_SIZE * SFG_FONT_SIZE_SMALL) / 2,
      SFG_GAME_RESOLUTION_Y - (SFG_TEXTURE_SIZE + 3) * SFG_FONT_SIZE_SMALL,
      SFG_FONT_SIZE_SMALL);  

  uint16_t textLen = 0;

  while (text[textLen] != 0)
    textLen++;

  uint16_t drawLen =
    RCL_min(textLen,
    ((SFG_game.frameTime - SFG_game.stateChangeTime) * textLen) /
      SFG_STORYTEXT_DURATION + 1);

#define CHAR_SIZE (SFG_FONT_SIZE_SMALL * (SFG_FONT_CHARACTER_SIZE + 1))
#define LINE_LENGTH (SFG_GAME_RESOLUTION_X / CHAR_SIZE)
#define MAX_LENGTH (((SFG_GAME_RESOLUTION_Y / CHAR_SIZE) / 2) * LINE_LENGTH  )

  uint16_t drawShift = (drawLen < MAX_LENGTH) ? 0 :
    (((drawLen - MAX_LENGTH) / LINE_LENGTH) * LINE_LENGTH);

#undef CHAR_SIZE
#undef LINE_LENGTH
#undef MAX_LENGTH

  text += drawShift;
  drawLen -= drawShift;

  SFG_drawText(text,SFG_HUD_MARGIN,SFG_HUD_MARGIN,SFG_FONT_SIZE_SMALL,textColor,
    drawLen,SFG_GAME_RESOLUTION_X - SFG_HUD_MARGIN);
}

/**
  Draws a number as text on screen, returns the number of characters drawn.
*/
uint8_t SFG_drawNumber(
  int16_t number,
  uint16_t x,
  uint16_t y,
  uint8_t size,
  uint8_t color)
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

  SFG_drawText(text + position + 1,x,y,size,color,0,0);

  return 5 - position;
}

/**
  Draws a screen border that indicates something is happening, e.g. being hurt
  or taking an item.
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
  Draws the player weapon, includes handling the shoot animation.
*/
void SFG_drawWeapon(int16_t bobOffset)
{
  uint32_t animationLength =
    RCL_max(SFG_MIN_WEAPON_COOLDOWN_FRAMES,
      SFG_GET_WEAPON_FIRE_COOLDOWN_FRAMES(SFG_player.weapon));

  uint32_t shotAnimationFrame =
    animationLength - SFG_player.weaponCooldownFrames;

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
        SFG_blitImage(SFG_effectSprites,
          SFG_WEAPON_IMAGE_POSITION_X,
          SFG_WEAPON_IMAGE_POSITION_Y -
            (SFG_TEXTURE_SIZE / 3) * SFG_WEAPON_IMAGE_SCALE + bobOffset,
          SFG_WEAPON_IMAGE_SCALE);
    }
  }

  SFG_blitImage(SFG_weaponImages + SFG_player.weapon * SFG_TEXTURE_STORE_SIZE,
  SFG_WEAPON_IMAGE_POSITION_X,
  SFG_WEAPON_IMAGE_POSITION_Y + bobOffset - 1,
  SFG_WEAPON_IMAGE_SCALE);
}

uint16_t SFG_textLen(const char *text)
{
  uint16_t result = 0;

  while (text[result] != 0)
    result++;

  return result;
}

static inline uint16_t SFG_characterSize(uint8_t textSize)
{
  return (SFG_FONT_CHARACTER_SIZE + 1) * textSize;
}

static inline uint16_t
  SFG_textHorizontalSize(const char *text, uint8_t textSize)
{
  return (SFG_textLen(text) * SFG_characterSize(textSize));
}

void SFG_drawMenu()
{
  #define BACKGROUND_SCALE (SFG_GAME_RESOLUTION_X / (4 * SFG_TEXTURE_SIZE ) )

  #if BACKGROUND_SCALE == 0
    #undef BACKGROUND_SCALE
    #define BACKGROUND_SCALE 1
  #endif

  #define SCROLL_PIXELS_PER_FRAME ((64 * SFG_GAME_RESOLUTION_X) / (8 * SFG_FPS))

  #if SCROLL_PIXELS_PER_FRAME == 0
    #undef SCROLL_PIXELS_PER_FRAME
    #define SCROLL_PIXELS_PER_FRAME 1
  #endif

  #define SELECTION_START_X ((SFG_GAME_RESOLUTION_X - 12 * SFG_FONT_SIZE_MEDIUM\
    * (SFG_FONT_CHARACTER_SIZE + 1)) / 2)

  uint16_t scroll = (SFG_game.frame * SCROLL_PIXELS_PER_FRAME) / 64;

  for (uint16_t y = 0; y < SFG_GAME_RESOLUTION_Y; ++y)
    for (uint16_t x = 0; x < SFG_GAME_RESOLUTION_X; ++x)
      SFG_setGamePixel(x,y,
        (y >= (SFG_TEXTURE_SIZE * BACKGROUND_SCALE)) ? 0 :
        SFG_getTexel(SFG_backgroundImages,((x + scroll) / BACKGROUND_SCALE)
          % SFG_TEXTURE_SIZE,y / BACKGROUND_SCALE));

  uint16_t y = SFG_characterSize(SFG_FONT_SIZE_MEDIUM);

  SFG_blitImage(SFG_logoImage,
    SFG_GAME_RESOLUTION_X / 2 - 16 * SFG_FONT_SIZE_MEDIUM,y,
    SFG_FONT_SIZE_MEDIUM);

  y += 32 * SFG_FONT_SIZE_MEDIUM + SFG_characterSize(SFG_FONT_SIZE_MEDIUM);

  uint8_t i = 0;

  while (1) // draw menu items
  {
    uint8_t item = SFG_getMenuItem(i);

    if (item == SFG_MENU_ITEM_NONE)
      break;

#if SFG_VERY_LOW_RESOLUTION
    if (i != SFG_game.selectedMenuItem) // only display selected item     
    {
      i++;
      continue;
    }
#endif

    const char *text = SFG_menuItemTexts[item];
    uint8_t textLen = SFG_textLen(text);

    uint16_t drawX = (SFG_GAME_RESOLUTION_X -
      SFG_textHorizontalSize(text,SFG_FONT_SIZE_MEDIUM)) / 2;

    uint8_t textColor = 7;

    if (i != SFG_game.selectedMenuItem)
      textColor = 23;
    else
      SFG_fillRectangle( // menu item highlight
        SELECTION_START_X,
        y - SFG_FONT_SIZE_MEDIUM,
        SFG_GAME_RESOLUTION_X - SELECTION_START_X * 2,
        SFG_characterSize(SFG_FONT_SIZE_MEDIUM),2);
  
    SFG_drawText(text,drawX,y,SFG_FONT_SIZE_MEDIUM,textColor,0,0);

    if ((item == SFG_MENU_ITEM_PLAY || item == SFG_MENU_ITEM_SOUND
         || item == SFG_MENU_ITEM_SHEAR) &&
        ((i != SFG_game.selectedMenuItem) || SFG_game.blink))
    {
      uint32_t x =
        drawX + SFG_characterSize(SFG_FONT_SIZE_MEDIUM) * (textLen + 1);

      uint8_t c = 93;

      if (item == SFG_MENU_ITEM_PLAY)
        SFG_drawNumber(SFG_game.selectedLevel + 1,x,y,SFG_FONT_SIZE_MEDIUM,c);
      else if (item == SFG_MENU_ITEM_SHEAR)
      {
        uint8_t n = (SFG_game.settings >> 2) & 0x03;

        SFG_drawNumber(n == 3 ? 2 : n,x,y,SFG_FONT_SIZE_MEDIUM,c);
      }
      else
      {
        char settingText[3] = "  ";

        settingText[0] = (SFG_game.settings & 0x01) ? 'S' : ' ';
        settingText[1] = (SFG_game.settings & 0x02) ? 'M' : ' ';

        SFG_drawText(settingText,x,y,SFG_FONT_SIZE_MEDIUM,c,0,0);
      }
    }

    y += SFG_characterSize(SFG_FONT_SIZE_MEDIUM) + SFG_FONT_SIZE_MEDIUM;
    i++;
  }
  
  SFG_drawText("0.7 CC0",SFG_HUD_MARGIN,SFG_GAME_RESOLUTION_Y - SFG_HUD_MARGIN
    - SFG_FONT_SIZE_SMALL * SFG_FONT_CHARACTER_SIZE,SFG_FONT_SIZE_SMALL,4,0,0);

  #if SFG_OS_IS_MALWARE
    if (SFG_game.blink)
      SFG_drawText(SFG_MALWARE_WARNING,SFG_HUD_MARGIN,SFG_HUD_MARGIN,
        SFG_FONT_SIZE_MEDIUM,95,0,0);
  #endif

  #undef MAX_ITEMS
  #undef BACKGROUND_SCALE
  #undef SCROLL_PIXELS_PER_FRAME
}

void SFG_drawWinOverlay()
{
  uint32_t t = RCL_min(SFG_WIN_ANIMATION_DURATION,
     SFG_game.frameTime - SFG_game.stateChangeTime);

  uint32_t t2 = RCL_min(t,SFG_WIN_ANIMATION_DURATION / 4);

  #define STRIP_HEIGHT (SFG_GAME_RESOLUTION_Y / 2)
  #define INNER_STRIP_HEIGHT ((STRIP_HEIGHT * 3) / 4)
  #define STRIP_START ((SFG_GAME_RESOLUTION_Y - STRIP_HEIGHT) / 2)

  RCL_Unit l = (t2 * STRIP_HEIGHT * 4) / SFG_WIN_ANIMATION_DURATION;

  uint8_t n = (t * 5) / SFG_WIN_ANIMATION_DURATION;

  for (uint16_t y = STRIP_START; y < STRIP_START + l; ++y)
    for (uint16_t x = 0; x < SFG_GAME_RESOLUTION_X; ++x)
      SFG_setGamePixel(x,y, 
        RCL_abs(y - (SFG_GAME_RESOLUTION_Y / 2)) <= (INNER_STRIP_HEIGHT / 2) ?
          0 : 172);

  char textLine[] = "level done";

  uint16_t y = SFG_GAME_RESOLUTION_Y / 2 - 
    ((STRIP_HEIGHT + INNER_STRIP_HEIGHT) / 2) / 2;

  uint16_t x = (SFG_GAME_RESOLUTION_X - 
    SFG_textHorizontalSize(textLine,SFG_FONT_SIZE_BIG)) / 2;

  SFG_drawText(textLine,x,y,SFG_FONT_SIZE_BIG,7 + SFG_game.blink * 95,255,0);

  uint32_t timeTotal = SFG_SAVE_TOTAL_TIME;

  uint8_t blinkDouble = (SFG_game.frame / SFG_BLINK_PERIOD_FRAMES) % 4;

  // don't show totals in level 1:
  blinkDouble &= (SFG_currentLevel.levelNumber != 0) || (timeTotal == 0);

  if (t >= (SFG_WIN_ANIMATION_DURATION / 2))
  {
    y += (SFG_FONT_SIZE_BIG + SFG_FONT_SIZE_MEDIUM) * SFG_FONT_CHARACTER_SIZE;
    x = SFG_GAME_RESOLUTION_X / 4;

    #define CHAR_SIZE (SFG_FONT_SIZE_SMALL * SFG_FONT_CHARACTER_SIZE)

#if SFG_VERY_LOW_RESOLUTION
    if (blinkDouble & 0x02)
    {
#endif
    uint32_t time = (blinkDouble & 0x01) ?
      SFG_currentLevel.completionTime10sOfS : timeTotal;

    x += (SFG_drawNumber(time / 10,x,y,SFG_FONT_SIZE_SMALL,7) + 1) *
      CHAR_SIZE;

    char timeRest[5] = ".X s";

    timeRest[1] = '0' + (time % 10);
    
    SFG_drawText(timeRest,x,y,SFG_FONT_SIZE_SMALL,7,4,0);
#if SFG_VERY_LOW_RESOLUTION
    }
    else
    {
#else
    x = SFG_GAME_RESOLUTION_X / 2;
#endif

    if (blinkDouble & 0x01)
    {
      x += SFG_drawNumber(SFG_currentLevel.monstersDead,x,y,
        SFG_FONT_SIZE_SMALL,7) * CHAR_SIZE;

      SFG_drawText("/",x,y,SFG_FONT_SIZE_SMALL,7,1,0);

      x += CHAR_SIZE;

      x += (SFG_drawNumber(SFG_currentLevel.monsterRecordCount,x,y,
        SFG_FONT_SIZE_SMALL,7) + 1) * CHAR_SIZE;
    }
    else
      x += (SFG_drawNumber(SFG_game.save[10] + SFG_game.save[11] * 256,x,y,
        SFG_FONT_SIZE_SMALL,7) + 1) * CHAR_SIZE;
    
    SFG_drawText(SFG_TEXT_KILLS,x,y,SFG_FONT_SIZE_SMALL,7,255,0);
    
#if SFG_VERY_LOW_RESOLUTION
    }
#endif

    if ((t >= (SFG_WIN_ANIMATION_DURATION - 1)) && 
      (SFG_currentLevel.levelNumber != (SFG_NUMBER_OF_LEVELS - 1)))
    {
      y += (SFG_FONT_SIZE_BIG + SFG_FONT_SIZE_MEDIUM) * SFG_FONT_CHARACTER_SIZE;

      SFG_drawText(SFG_TEXT_SAVE_PROMPT,
        (SFG_GAME_RESOLUTION_X - SFG_textHorizontalSize(SFG_TEXT_SAVE_PROMPT,
          SFG_FONT_SIZE_MEDIUM)) / 2,y,SFG_FONT_SIZE_MEDIUM,7,255,0);
    }

    #undef CHAR_SIZE
  }

  #undef STRIP_HEIGHT
  #undef STRIP_START
  #undef INNER_STRIP_HEIGHT
}

void SFG_draw()
{
#if SFG_BACKGROUND_BLUR != 0
  SFG_backgroundBlurIndex = 0;
#endif

  if (SFG_game.state == SFG_GAME_STATE_MENU)
  {
    SFG_drawMenu();
    return;
  }

  if (SFG_game.state == SFG_GAME_STATE_INTRO ||
      SFG_game.state == SFG_GAME_STATE_OUTRO)
  {
    SFG_drawStoryText();
    return;
  }

  if (SFG_keyPressed(SFG_KEY_MAP) || (SFG_game.state == SFG_GAME_STATE_MAP))
  {
    SFG_drawMap();
  } 
  else
  { 
    for (uint16_t i = 0; i < SFG_Z_BUFFER_SIZE; ++i)
      SFG_game.zBuffer[i] = 255;

    int16_t weaponBobOffset = 0;

#if SFG_HEADBOB_ENABLED
    RCL_Unit headBobOffset = 0;

    if (SFG_game.state != SFG_GAME_STATE_LOSE)
    {
      RCL_Unit bobSin = RCL_sin(SFG_player.headBobFrame);

      headBobOffset = (bobSin * SFG_HEADBOB_OFFSET) / RCL_UNITS_PER_SQUARE;

      weaponBobOffset =
        (bobSin * SFG_WEAPONBOB_OFFSET_PIXELS) / (RCL_UNITS_PER_SQUARE) + 
        SFG_WEAPONBOB_OFFSET_PIXELS;
    }
    else
    {
      // player die animation

      int32_t t = SFG_game.frameTime - SFG_game.stateChangeTime;
      
      weaponBobOffset = (SFG_WEAPON_IMAGE_SCALE * SFG_TEXTURE_SIZE * t) /
        SFG_LOSE_ANIMATION_DURATION;
    }
      
    // add head bob just for the rendering (we'll will substract it back later)

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

        RCL_Unit worldHeight = 
          SFG_floorHeightAt(
            SFG_MONSTER_COORD_TO_SQUARES(m.coords[0]),
            SFG_MONSTER_COORD_TO_SQUARES(m.coords[1]))
            + 
            SFG_SPRITE_SIZE_TO_HEIGHT_ABOVE_GROUND(spriteSize);

        RCL_PixelInfo p =
          RCL_mapToScreen(worldPosition,worldHeight,SFG_player.camera);

        if (p.depth > 0 &&
          SFG_spriteIsVisible(worldPosition,worldHeight,spriteSize))
        {
          const uint8_t *s =
            SFG_getMonsterSprite(
              SFG_MR_TYPE(m),
              state,
              SFG_game.spriteAnimationFrame & 0x01);

          SFG_drawScaledSprite(s,
            p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
            RCL_perspectiveScaleVertical(
            SFG_SPRITE_SIZE_PIXELS(spriteSize),
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

        const uint8_t *sprite;
        uint8_t spriteSize;

        SFG_getItemSprite(e.type,&sprite,&spriteSize);

        if (sprite != 0)
        {
          RCL_Unit worldHeight = SFG_floorHeightAt(e.coords[0],e.coords[1])
            + SFG_SPRITE_SIZE_TO_HEIGHT_ABOVE_GROUND(spriteSize);

          RCL_PixelInfo p =
            RCL_mapToScreen(worldPosition,worldHeight,SFG_player.camera);

          if (p.depth > 0 &&
            SFG_spriteIsVisible(worldPosition,worldHeight,spriteSize))
            SFG_drawScaledSprite(
              sprite,
              p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
              RCL_perspectiveScaleVertical(SFG_SPRITE_SIZE_PIXELS(spriteSize),p.depth),
              p.depth / (RCL_UNITS_PER_SQUARE * 2),p.depth);
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
       
      const uint8_t *s =
        SFG_effectSprites + proj->type * SFG_TEXTURE_STORE_SIZE;

      int16_t spriteSize = SFG_SPRITE_SIZE_PIXELS(0);

      if (proj->type == SFG_PROJECTILE_EXPLOSION ||
          proj->type == SFG_PROJECTILE_DUST)
      {
        int16_t doubleFramesToLive =
          RCL_nonZero(SFG_GET_PROJECTILE_FRAMES_TO_LIVE(proj->type) / 2);

        // grow the explosion/dust sprite as an animation
        spriteSize =
          (
            SFG_SPRITE_SIZE_PIXELS(2) *
            RCL_sin(          
              ((doubleFramesToLive -
               proj->doubleFramesToLive) * RCL_UNITS_PER_SQUARE / 4)
               / doubleFramesToLive) 
          ) / RCL_UNITS_PER_SQUARE;
      }

      if (p.depth > 0 && 
        SFG_spriteIsVisible(worldPosition,proj->position[2],spriteSize))
        SFG_drawScaledSprite(s,
            p.position.x * SFG_RAYCASTING_SUBSAMPLE,p.position.y,
            RCL_perspectiveScaleVertical(spriteSize,p.depth),
            SFG_fogValueDiminish(p.depth),
            p.depth);  
    }

#if SFG_HEADBOB_ENABLED
    // after rendering sprites substract back the head bob offset
    SFG_player.camera.height -= headBobOffset;
#endif

#if SFG_PREVIEW_MODE == 0
    SFG_drawWeapon(weaponBobOffset);
#endif

    // draw HUD:

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
      SFG_player.weapon != SFG_WEAPON_KNIFE ?
        SFG_player.ammo[SFG_weaponAmmo(SFG_player.weapon)] : 0,
      SFG_GAME_RESOLUTION_X - SFG_HUD_MARGIN -
        (SFG_FONT_CHARACTER_SIZE + 1) * SFG_FONT_SIZE_MEDIUM * 3,
      TEXT_Y,
      SFG_FONT_SIZE_MEDIUM,
      4); 

    for (uint8_t i = 0; i < 3; ++i) // access cards
      if ( 
        ((SFG_player.cards >> i) | ((SFG_player.cards >> (i + 3))
        & SFG_game.blink)) & 0x01)
        SFG_fillRectangle(
          SFG_HUD_MARGIN + (SFG_FONT_CHARACTER_SIZE + 1) *
            SFG_FONT_SIZE_MEDIUM * (5 + i),
          TEXT_Y,
          SFG_FONT_SIZE_MEDIUM * SFG_FONT_CHARACTER_SIZE,
          SFG_FONT_SIZE_MEDIUM * SFG_FONT_CHARACTER_SIZE,
          i == 0 ? 93 : (i == 1 ? 124 : 60));

    #undef TEXT_Y

    // border indicator

    if ((SFG_game.frame - SFG_player.lastHurtFrame
        <= SFG_HUD_BORDER_INDICATOR_DURATION_FRAMES) ||
        (SFG_game.state == SFG_GAME_STATE_LOSE))
      SFG_drawIndicationBorder(SFG_HUD_BORDER_INDICATOR_WIDTH_PIXELS,
      SFG_HUD_HURT_INDICATION_COLOR);
    else if (SFG_game.frame - SFG_player.lastItemTakenFrame
        <= SFG_HUD_BORDER_INDICATOR_DURATION_FRAMES)
      SFG_drawIndicationBorder(SFG_HUD_BORDER_INDICATOR_WIDTH_PIXELS,
      SFG_HUD_ITEM_TAKEN_INDICATION_COLOR);

    if (SFG_game.state == SFG_GAME_STATE_WIN)
      SFG_drawWinOverlay();
    else if (SFG_game.state == SFG_GAME_STATE_LEVEL_START)
      SFG_drawLevelStartOverlay();
  }
}

uint8_t SFG_mainLoopBody()
{
  /* Standard deterministic game loop, independed of actual achieved FPS.
     Each game logic (physics) frame is performed with the SFG_MS_PER_FRAME
     delta time. */
  uint32_t timeNow = SFG_getTimeMs();
  uint32_t timeNextFrame = SFG_game.lastFrameTimeMs + SFG_MS_PER_FRAME;

  SFG_game.frameTime = timeNow;

  if (timeNow >= timeNextFrame)
  {
    uint32_t timeSinceLastFrame = timeNow - SFG_game.lastFrameTimeMs;

    uint8_t steps = 0;

    // perform game logic (physics etc.), for each frame
    while (timeSinceLastFrame >= SFG_MS_PER_FRAME)
    {
      SFG_gameStep();

      timeSinceLastFrame -= SFG_MS_PER_FRAME;

      SFG_game.frame++;
      steps++;
    }

    if ((steps > 1) && (SFG_game.antiSpam == 0))
    {
      SFG_LOG("failed to reach target FPS! consider setting a lower value")
      SFG_game.antiSpam = 30;
    }

    if (SFG_game.antiSpam > 0)
      SFG_game.antiSpam--;

    // render noly once
    SFG_draw();

    SFG_game.lastFrameTimeMs = timeNow;
  }
  else
  {
    SFG_sleepMs((timeNextFrame - timeNow) / 2); // wait, relieve CPU
  }

  return SFG_game.continues;
}

#undef SFG_SAVE_TOTAL_TIME

#endif // guard
