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
#define SFG_LEVEL_ELEMENT_ACTIVE_DISTANCE 10

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
  Maximum player health.
*/
#define SFG_PLAYER_MAX_HEALTH 100

/**
  At which value health indicator shows a warning (red color).
*/
#define SFG_PLAYER_HEALTH_WARNING_LEVEL 20

/**
  Amount of health that is increased by taking a health kit.
*/
#define SFG_HEALTH_KIT_VALUE 20

/**
  How much randomness (positive and negative) will be added to damage
  (e.g. by weapons, explisions). This constant is is 0 to 255, 255 meaning
  100% of the base value.
*/
#define SFG_DAMAGE_RANDOMNESS 64 

/**
  Angle in which multiple projectiles are spread, RCL_Units.
*/
#define SFG_PROJECTILE_SPREAD_ANGLE 100

#define SFG_MAX_MONSTERS 64

#define SFG_MAX_PROJECTILES 12

#define SFG_MAX_DOORS 32

// ----------------------------
// derived constants

#define SFG_GAME_RESOLUTION_X \
  (SFG_SCREEN_RESOLUTION_X / SFG_RESOLUTION_SCALEDOWN)

#define SFG_GAME_RESOLUTION_Y \
  (SFG_SCREEN_RESOLUTION_Y / SFG_RESOLUTION_SCALEDOWN)

#define SFG_MS_PER_FRAME (1000 / SFG_FPS) // ms per frame with target FPS

#define SFG_BASE_SPRITE_SIZE \
  ((SFG_GAME_RESOLUTION_Y * 2) / 3)

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

#define SFG_Z_BUFFER_SIZE SFG_GAME_RESOLUTION_X

/**
  Step in which walls get higher, in raycastlib units.
*/
#define SFG_WALL_HEIGHT_STEP (RCL_UNITS_PER_SQUARE / 4)

#define SFG_CEILING_MAX_HEIGHT\
  (16 * RCL_UNITS_PER_SQUARE - RCL_UNITS_PER_SQUARE / 2 )

#define SFG_DOOR_DEFAULT_STATE 0x1f
#define SFG_DOOR_UP_DOWN_MASK 0x20
#define SFG_DOOR_VERTICAL_POSITION_MASK 0x1f
#define SFG_DOOR_HEIGHT_STEP (RCL_UNITS_PER_SQUARE / 0x1f)

#define SFG_DOOR_INCREMENT_PER_FRAME \
  (SFG_DOOR_OPEN_SPEED / (SFG_DOOR_HEIGHT_STEP * SFG_FPS))

#if SFG_DOOR_INCREMENT_PER_FRAME == 0
  #define SFG_DOOR_INCREMENT_PER_FRAME 1
#endif

#define SFG_MAX_ITEMS SFG_MAX_LEVEL_ELEMENTS

#define SFG_MAX_SPRITE_SIZE SFG_GAME_RESOLUTION_X

#define SFG_MAP_PIXEL_SIZE (SFG_GAME_RESOLUTION_Y / SFG_MAP_SIZE)

#if SFG_MAP_PIXEL_SIZE == 0
  #define SFG_MAP_SIZE 1
#endif

#define SFG_AI_UPDATE_FRAME_INTERVAL \
  (SFG_FPS / SFG_AI_FPS)

#if SFG_AI_UPDATE_FRAME_INTERVAL == 0
  #define SFG_AI_UPDATE_FRAME_INTERVAL 1
#endif

#if SFG_EXPLOSION_DURATION_DOUBLE_FRAMES == 0
  #define SFG_EXPLOSION_DURATION_FRAMES 1
#endif

#define SFG_SPRITE_ANIMATION_FRAME_DURATION \
  (SFG_FPS / SFG_SPRITE_ANIMATION_SPEED)

#if SFG_SPRITE_ANIMATION_FRAME_DURATION == 0
  #define SFG_SPRITE_ANIMATION_FRAME_DURATION 1
#endif

#define SFG_HUD_MARGIN (SFG_GAME_RESOLUTION_X / 40)

#define SFG_HUD_HEALTH_INDICATOR_WIDTH_PIXELS \
  (SFG_GAME_RESOLUTION_Y /  SFG_HUD_HEALTH_INDICATOR_WIDTH)

#define SFG_HUD_HEALTH_INDICATOR_DURATION_FRAMES \
  (SFG_HUD_HEALTH_INDICATOR_DURATION / SFG_MS_PER_FRAME)

#if SFG_HUD_HEALTH_INDICATOR_DURATION_FRAMES == 0
  #define SFG_HUD_HEALTH_INDICATOR_DURATION_FRAMES 1
#endif

#define SFG_HUD_BAR_HEIGHT \
  (SFG_FONT_CHARACTER_SIZE * SFG_FONT_SIZE_MEDIUM + SFG_HUD_MARGIN * 2 + 1)

// ----------------------------
// monsters

#define SFG_MONSTER_ATTACK_MELEE 0
#define SFG_MONSTER_ATTACK_FIREBALL 1
#define SFG_MONSTER_ATTACK_BULLET 2
#define SFG_MONSTER_ATTACK_FIREBALL_BULLET 3
#define SFG_MONSTER_ATTACK_PLASMA 4
#define SFG_MONSTER_ATTACK_EXPLODE 5

#define SFG_MONSTER_ATTRIBUTE(attackType,aggressivity0to255,health0to255,spriteSize0to3) \
  ((uint16_t) ( \
   attackType | \
   ((aggressivity0to255 / 8) << 3) | \
   (spriteSize0to3 << 8) | \
   ((health0to255 / 4) << 10)))

#define SFG_GET_MONSTER_ATTACK_TYPE(monsterNumber) \
  (SFG_monsterAttributeTable[monsterNumber] & 0x0007)

#define SFG_GET_MONSTER_AGGRESSIVITY(monsterNumber) \
  (((SFG_monsterAttributeTable[monsterNumber] >> 3) & 0x1F) * 8)

#define SFG_GET_MONSTER_SPRITE_SIZE(monsterNumber) \
  ((SFG_monsterAttributeTable[monsterNumber] >> 8) & 0x03)

#define SFG_GET_MONSTER_MAX_HEALTH(monsterNumber) \
  (((SFG_monsterAttributeTable[monsterNumber] >> 10) & 0x3F) * 4)

/**
  Table of monster attributes, each as a 16bit word in format:

  MSB hhhhhhssaaaattt LSB

  ttt:    attack type
  aaaaa:  aggressivity (frequence of attacks), 0 to 31
  ss:     sprite size
  hhhhhh: health, 0 to 63
*/
uint16_t SFG_monsterAttributeTable[SFG_MONSTERS_TOTAL] =
{
  /* spider  */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_FIREBALL,40,120,2),
  /* destr.  */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_FIREBALL_BULLET,50,130,3),
  /* warrior */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_MELEE,255,70,1),
  /* plasma  */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_PLASMA,55,92,1),
  /* ender   */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_FIREBALL_BULLET,75,255,3),
  /* turret  */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_BULLET,32,50,0),
  /* explod. */ SFG_MONSTER_ATTRIBUTE(SFG_MONSTER_ATTACK_EXPLODE,255,60,1)
};

// ----------------------------
// weapons and projectiles

#define SFG_WEAPON_KNIFE 0
#define SFG_WEAPON_SHOTGUN 1
#define SFG_WEAPON_MACHINE_GUN 2
#define SFG_WEAPON_ROCKET_LAUNCHER 3
#define SFG_WEAPON_PLASMAGUN 4
#define SFG_WEAPON_SOLUTION 5

#define SFG_WEAPONS_TOTAL 6

#define SFG_WEAPON_ATTRIBUTE(fireType,projectileCount,fireCooldownMs) \
  ((uint8_t) (fireType | ((projectileCount - 1) << 2) | ((fireCooldownMs / (SFG_MS_PER_FRAME * 16)) << 4)))

#define SFG_GET_WEAPON_FIRE_TYPE(weaponNumber) \
  (SFG_weaponAttributeTable[weaponNumber] & 0x03)

#define SFG_GET_WEAPON_FIRE_COOLDOWN_FRAMES(weaponNumber) \
  ((SFG_weaponAttributeTable[weaponNumber] >> 4) * 16)

#define SFG_GET_WEAPON_PROJECTILE_COUNT(weaponNumber) \
  (((SFG_weaponAttributeTable[weaponNumber] >> 2) & 0x03) + 1)

#define SFG_WEAPON_FIRE_TYPE_MELEE 0
#define SFG_WEAPON_FIRE_TYPE_BULLET 1
#define SFG_WEAPON_FIRE_TYPE_FIREBALL 2
#define SFG_WEAPON_FIRE_TYPE_PLASMA 3

#define SFG_WEAPON_FIRE_TYPES_TOTAL 4

/**
  Table of weapon attributes, each as a byte in format:

  MSB ccccnnff LSB

  ff:     fire type
  nn:     number of projectiles - 1
  cccc:   fire cooldown in frames, i.e. time after which the next shot can be
          shot again, ccccc has to be multiplied by 16 to ge the real value
*/
SFG_PROGRAM_MEMORY uint8_t SFG_weaponAttributeTable[SFG_WEAPONS_TOTAL] =
{
  /* knife    */ SFG_WEAPON_ATTRIBUTE(SFG_WEAPON_FIRE_TYPE_MELEE,1,650),
  /* shotgun  */ SFG_WEAPON_ATTRIBUTE(SFG_WEAPON_FIRE_TYPE_BULLET,2,800),
  /* m. gun   */ SFG_WEAPON_ATTRIBUTE(SFG_WEAPON_FIRE_TYPE_BULLET,1,500),
  /* r. laun. */ SFG_WEAPON_ATTRIBUTE(SFG_WEAPON_FIRE_TYPE_FIREBALL,1,900),
  /* plasma   */ SFG_WEAPON_ATTRIBUTE(SFG_WEAPON_FIRE_TYPE_PLASMA,1,600),
  /* solution */ SFG_WEAPON_ATTRIBUTE(SFG_WEAPON_FIRE_TYPE_PLASMA,4,1000)
};

SFG_PROGRAM_MEMORY uint8_t SFG_attackDamageTable[SFG_WEAPON_FIRE_TYPES_TOTAL] =
{
  /* melee                 */ 8,
  /* bullet                */ 10,
  /* explostion (fireball) */ 13,
  /* plasma                */ 17
};

#define SFG_PROJECTILE_EXPLOSION 0
#define SFG_PROJECTILE_FIREBALL 1
#define SFG_PROJECTILE_PLASMA 2
#define SFG_PROJECTILE_DUST 3
#define SFG_PROJECTILE_BULLET 4
#define SFG_PROJECTILE_NONE 255

#define SFG_PROJECTILES_TOTAL 5

#define SFG_PROJECTILE_ATTRIBUTE(speedSquaresPerSec,timeToLiveMs) \
  ((uint8_t) \
   ((((speedSquaresPerSec / 4 == 0) && (speedSquaresPerSec != 0)) ? 1 : speedSquaresPerSec / 4) | \
    ((timeToLiveMs / (8 * SFG_MS_PER_FRAME)) << 3)))

#define SFG_GET_PROJECTILE_SPEED_UPS(projectileNumber) \
  (((SFG_projectileAttributeTable[projectileNumber] & 0x07) * 4 * RCL_UNITS_PER_SQUARE) / SFG_FPS)

#define SFG_GET_PROJECTILE_FRAMES_TO_LIVE(projectileNumber) \
  ((SFG_projectileAttributeTable[projectileNumber] >> 3) * 8)

/**
  Table of projectile attributes, each as a byte in format:

  MSB lllllsss LSB

  fff:   half speed in game squares per second
  lllll: eigth of frames to live
*/
SFG_PROGRAM_MEMORY uint8_t SFG_projectileAttributeTable[SFG_PROJECTILES_TOTAL] =
{
  /* explosion */ SFG_PROJECTILE_ATTRIBUTE(0,400),
  /* fireball  */ SFG_PROJECTILE_ATTRIBUTE(18,1000),
  /* plasma    */ SFG_PROJECTILE_ATTRIBUTE(20,500),
  /* dust      */ SFG_PROJECTILE_ATTRIBUTE(0,450),
  /* bullet    */ SFG_PROJECTILE_ATTRIBUTE(28,1000)
};

#endif // guard
