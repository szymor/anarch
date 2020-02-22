/**
  @file levels.h

  This file contains game levels and related code.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef _SFG_LEVELS_H
#define _SFG_LEVELS_H

#define SFG_MAP_SIZE 64
#define SFG_TILE_DICTIONARY_SIZE 64

typedef uint16_t SFG_TileDefinition;
/**<
  Defines a single game map tile. The format is following:

    MSB aaabbbbb cccddddd LSB

   aaa:   ceiling texture index (from texture available on the map), 111
          means completely transparent texture
   bbbbb: ceiling height (1111 meaning no ceiling) ABOVE the floor
   ccc:   floor texture index, 111 means completely transparent texture
   ddddd: floor height
*/

#define SFG_TILE_CEILING_MAX_HEIGHT 31
#define SFG_TILE_TEXTURE_TRANSPARENT 7

typedef SFG_TileDefinition SFG_TileDictionary[SFG_TILE_DICTIONARY_SIZE];

/// helper macros for SFG_TileDefinition
#define SFG_TD(floorH, ceilH, floorT, ceilT)\
  ((floorH & 0x001f) |\
   ((floorT & 0x0007) << 5) |\
   ((ceilH & 0x001f) << 8) |\
   ((ceilT & 0x0007) << 13))

#define SFG_TILE_FLOOR_HEIGHT(tile) (tile & 0x1f)
#define SFG_TILE_FLOOR_TEXTURE(tile) ((tile & 0xe0) >> 5)
#define SFG_TILE_CEILING_HEIGHT(tile) ((tile & 0x1f00) >> 8)
#define SFG_TILE_CEILING_TEXTURE(tile) ((tile & 0xe000) >> 13)

#define SFG_OUTSIDE_TILE SFG_TD(63,0,7,7)

typedef uint8_t SFG_MapArray[SFG_MAP_SIZE * SFG_MAP_SIZE];
/**<
  Game map represented as a 2D array. Array item has this format:

    MSB aabbbbbb LSB

    aa:     type of square, possible values:
      00:   normal
      01:   moving floor (elevator), moves from floor height to ceililing height
            (there is no ceiling above)
      10:   moving ceiling, moves from ceiling height to floor height
      11:   door
    bbbbbb: index into tile dictionary
*/

#define SFG_TILE_PROPERTY_MASK 0xc0
#define SFG_TILE_PROPERTY_NORMAL 0x00
#define SFG_TILE_PROPERTY_ELEVATOR 0x40
#define SFG_TILE_PROPERTY_SQUEEZER 0x80
#define SFG_TILE_PROPERTY_DOOR 0xc0

/**
  Serves to place elements (items, enemies etc.) into the game level.
*/
typedef struct
{
  uint8_t type;
  uint8_t coords[2];
} SFG_LevelElement;

#define SFG_MAX_LEVEL_ELEMENTS 128

/*
  Definitions of level element type. These values must leave the highest bit
  unused because that will be used by the game engine, so the values must be
  lower than 128.
*/
#define SFG_LEVEL_ELEMENT_NONE 255
#define SFG_LEVEL_ELEMENT_BARREL 0x01
#define SFG_LEVEL_ELEMENT_HEALTH 0x02
#define SFG_LEVEL_ELEMENT_BULLETS 0x03
#define SFG_LEVEL_ELEMENT_ROCKETS 0x04
#define SFG_LEVEL_ELEMENT_PLASMA 0x05
#define SFG_LEVEL_ELEMENT_TREE 0x06
#define SFG_LEVEL_ELEMENT_FINISH 0x07
#define SFG_LEVEL_ELEMENT_TELEPORT 0x08
#define SFG_LEVEL_ELEMENT_TERMINAL 0x09
#define SFG_LEVEL_ELEMENT_CARD0 0x0a ///< Access card, unlocks doors with lock.
#define SFG_LEVEL_ELEMENT_CARD1 0x0b
#define SFG_LEVEL_ELEMENT_CARD2 0x0c
#define SFG_LEVEL_ELEMENT_LOCK0 0x0d /**< Special level element that must be
                                     placed on a tile with door. This door is
                                     then unlocked by taking the corresponding
                                     access card. */
#define SFG_LEVEL_ELEMENT_LOCK1 0x0e
#define SFG_LEVEL_ELEMENT_LOCK2 0x0f

/* 
  Monsters have lower 4 bits zero and are only distinguished by the 4 upper
  bits, as this is convenient for the code.
*/
#define SFG_LEVEL_ELEMENT_MONSTER_SPIDER 0x00
#define SFG_LEVEL_ELEMENT_MONSTER_DESTROYER 0x10
#define SFG_LEVEL_ELEMENT_MONSTER_WARRIOR 0x20
#define SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT 0x30
#define SFG_LEVEL_ELEMENT_MONSTER_ENDER 0x40
#define SFG_LEVEL_ELEMENT_MONSTER_TURRET 0x50
#define SFG_LEVEL_ELEMENT_MONSTER_EXPLODER 0x60

#define SFG_MONSTERS_TOTAL 7

#define SFG_MONSTER_TYPE_TO_INDEX(monsterType) \
  ((monsterType) >> 4)

#define SFG_LEVEL_ELEMENT_TYPE_IS_MOSTER(t) (((t) & 0x0f) == 0)

typedef struct
{
  SFG_MapArray mapArray;
  SFG_TileDictionary tileDictionary;
  uint8_t textureIndices[7]; /**< Says which textures are used on the map. There
                             can be at most 7 because of 3bit indexing (one
                             value is reserved for special transparent
                             texture). */
  uint8_t doorTextureIndex; /**< Index (global, NOT from textureIndices) of a
                             texture used for door. */
  uint8_t floorColor;
  uint8_t ceilingColor;
  uint8_t playerStart[3];   /**< Player starting location: square X, square Y,
                                 direction (fourths of RCL_Unit). */
  uint8_t backgroundImage;  /** Index of level background image. */
  SFG_LevelElement elements[SFG_MAX_LEVEL_ELEMENTS];
} SFG_Level;

static inline SFG_TileDefinition SFG_getMapTile
(
  const SFG_Level *level,
  int16_t x,
  int16_t y,
  uint8_t *properties
)
{
  if (x < 0 || x >= SFG_MAP_SIZE || y < 0 || y >= SFG_MAP_SIZE)
  {
    *properties = SFG_TILE_PROPERTY_NORMAL;
    return SFG_OUTSIDE_TILE;
  }

  uint8_t tile = level->mapArray[y * SFG_MAP_SIZE + x];

  *properties = tile & 0xc0;
  return level->tileDictionary[tile & 0x3f];
}

#define SFG_NUMBER_OF_LEVELS 10

SFG_PROGRAM_MEMORY SFG_Level SFG_levels[SFG_NUMBER_OF_LEVELS] =
{
  { // level 0
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {12, 16}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TERMINAL, {11, 25}},     {SFG_LEVEL_ELEMENT_TERMINAL, {11, 26}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {10, 45}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 1
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 0
/*1 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 1
/*2 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 2
/*3 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 3
/*4 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 4
/*5 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 5
/*6 */ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 6
/*7 */ o ,o ,o ,o ,o ,5 ,5 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 7
/*8 */ o ,o ,o ,o ,o ,5 ,7 ,7 ,7 ,7 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 8
/*9 */ o ,o ,o ,o ,o ,5 ,7 ,4 ,4 ,4 ,4 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 9
/*10*/ o ,o ,o ,o ,o ,5 ,7 ,4 ,7 ,7 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 10
/*11*/ o ,o ,o ,o ,o ,5 ,7 ,4 ,7 ,7 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 11
/*12*/ o ,o ,o ,o ,o ,5 ,7 ,4 ,7 ,7 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 12
/*13*/ o ,o ,o ,o ,o ,5 ,7 ,4 ,7 ,7 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 13
/*14*/ o ,o ,o ,o ,o ,5 ,5 ,4 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 14
/*15*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 15
/*16*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 16
/*17*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 17
/*18*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 18
/*19*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 19
/*20*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 20
/*21*/ o ,o ,o ,o ,o ,8 ,o ,o ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 21
/*22*/ o ,o ,o ,8 ,8 ,o ,o ,o ,o ,8 ,8 ,o ,o ,o ,o ,o ,o ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 22
/*23*/ o ,o ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,o ,o ,5 ,5 ,5 ,4 ,4 ,5 ,5 ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 23
/*24*/ o ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 24
/*25*/ o ,8 ,o ,o ,o ,9 ,9 ,9 ,9 ,o ,o ,o ,8 ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 25
/*26*/ 8 ,o ,o ,o ,9 ,9 ,9 ,9 ,9 ,9 ,o ,o ,o ,8 ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 26
/*27*/ 8 ,o ,o ,o ,9 ,9 ,9 ,9 ,9 ,9 ,o ,o ,o ,8 ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 27
/*28*/ 8 ,o ,o ,o ,9 ,9 ,9 ,9 ,9 ,9 ,o ,o ,o ,8 ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 28
/*29*/ 8 ,o ,o ,o ,9 ,9 ,9 ,9 ,9 ,9 ,o ,o ,o ,8 ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 29
/*30*/ o ,8 ,o ,o ,o ,9 ,9 ,9 ,9 ,o ,o ,o ,8 ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 30
/*31*/ o ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 31
/*32*/ o ,o ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,o ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 32
/*33*/ o ,o ,o ,8 ,8 ,o ,o ,o ,o ,8 ,8 ,o ,o ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 33
/*34*/ o ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,o ,o ,o ,o ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 34
/*35*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 35
/*36*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 36
/*37*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,4 ,4 ,4 ,4 ,4 ,4 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 37
/*38*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 38
/*39*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 39
/*40*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 40
/*41*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 41
/*42*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 42
/*43*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 43
/*44*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 44
/*45*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 45
/*46*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 46
/*47*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 47
/*48*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 48
/*49*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 49
/*50*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 50
/*51*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 51
/*52*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 52
/*53*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 53
/*54*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 54
/*55*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 55
/*56*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 56
/*57*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 57
/*58*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 58
/*59*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 59
/*60*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 60
/*61*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 61
/*62*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 62
/*63*/ o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(14,31,0,0),SFG_TD(18,31,0,0),SFG_TD(26,31,0,0),SFG_TD(16,31,0,0), // 0
      SFG_TD(18, 8,0,0),SFG_TD(26, 0,0,0),SFG_TD(18,31,4,0),SFG_TD(12,12,0,5), // 4
      SFG_TD(26, 0,4,0),SFG_TD(15,31,2,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 8
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 12
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 16
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 20
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 24
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 28
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 2, 4, 5, 11, 14, 15}, // textureIndices
    13,                       // doorTextureIndex
    102,                      // floorColor
    32,                       // ceilingColor
    {10,10,64},               // player start: x, y, direction
    1,                        // backgroundImage
    {                         // elements
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },















  { // level 2
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 3
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 4
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 5
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 6
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 7
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 8
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
  { // level 9
    { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    #define EE (0x22 | SFG_TILE_PROPERTY_DOOR)
    #define FF (0x23 | SFG_TILE_PROPERTY_DOOR)
    #define GG (0x05 | SFG_TILE_PROPERTY_DOOR)
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
/*0 */ 22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 0
/*1 */ 22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 1
/*2 */ 12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25, // 2
/*3 */ 22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 3
/*4 */ 22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 4
/*5 */ 22,10,10,10,22,10,10,10,EE,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 5
/*6 */ 12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 6
/*7 */ 22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25, // 7
/*8 */ 22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 8
/*9 */ 22,22,22,EE,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 9
/*10*/ 22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 10
/*11*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,GG,5 ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 11
/*12*/ 22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 12
/*13*/ 22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25, // 13
/*14*/ 12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 14
/*15*/ 22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 15
/*16*/ 12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 16
/*17*/ 22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 17
/*18*/ 12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 18
/*19*/ 22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11, // 19
/*20*/ 22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 20
/*21*/ 11,1 ,1 ,EE,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 21
/*22*/ 11,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11, // 22
/*23*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11, // 23
/*24*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11, // 24
/*25*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 25
/*26*/ 11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,o ,o ,o ,o ,o ,11, // 26
/*27*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,FF,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 27
/*28*/ 11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 28
/*29*/ 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11, // 29
/*30*/ 22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 30
/*31*/ 22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 31
/*32*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 32
/*33*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 33
/*34*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11, // 34
/*35*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11, // 35
/*36*/ 22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11, // 36
/*37*/ 22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 37
/*38*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11, // 38
/*39*/ 22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11, // 39
/*40*/ 22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11, // 40
/*41*/ 22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 41
/*42*/ 22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11, // 42
/*43*/ 22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11, // 43
/*44*/ 22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 44
/*45*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 45
/*46*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 46
/*47*/ 22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11, // 47
/*48*/ 1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11, // 48
/*49*/ 2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 49
/*50*/ 2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 50
/*51*/ 2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 51
/*52*/ 2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 52
/*53*/ 2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11, // 53
/*54*/ 13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11, // 54
/*55*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 55
/*56*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11, // 56
/*57*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11, // 57
/*58*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 , // 58
/*59*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 , // 59
/*60*/ 3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 60
/*61*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 , // 61
/*62*/ 3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 , // 62
/*63*/ 14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 , // 63
    // 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
    #undef FF
    #undef GG
    },
    { // tileDictionary
      SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
      SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
      SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
      SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
      SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
      SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
      SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
      SFG_TD(20,31,3,0),SFG_TD(30,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
      SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(18, 0,1,1),SFG_TD(14, 0,1,1), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0)  // 60
    },
    {0, 1, 2, 6, 3, 12, 10}, // textureIndices
    13,                      // doorTextureIndex
    10,                      // floorColor
    32,                      // ceilingColor
    {8,19,0},                // player start: x, y, direction
    0,                       // backgroundImage
    {                        // elements
      {SFG_LEVEL_ELEMENT_BARREL, {9, 1}},  {SFG_LEVEL_ELEMENT_BARREL, {9, 13}},
      {SFG_LEVEL_ELEMENT_HEALTH, {12, 4}},  {SFG_LEVEL_ELEMENT_HEALTH, {15, 4}},
      {SFG_LEVEL_ELEMENT_BARREL, {24, 10}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BARREL, {13, 4}},     {SFG_LEVEL_ELEMENT_BARREL, {12, 6}},
      {SFG_LEVEL_ELEMENT_MONSTER_ENDER, {23, 10}},   {SFG_LEVEL_ELEMENT_MONSTER_SPIDER, {16, 8}},
      {SFG_LEVEL_ELEMENT_MONSTER_DESTROYER, {16, 7}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_WARRIOR, {18, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_PLASMABOT, {19, 31}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_TURRET, {5, 24}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_MONSTER_EXPLODER, {22, 18}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_BULLETS, {2, 56}},     {SFG_LEVEL_ELEMENT_BULLETS, {3, 56}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 37}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 38}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 39}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 40}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 41}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 42}},
      {SFG_LEVEL_ELEMENT_BARREL, {9, 43}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 44}},
      {SFG_LEVEL_ELEMENT_BARREL, {8, 45}},     {SFG_LEVEL_ELEMENT_BARREL, {9, 46}},
      {SFG_LEVEL_ELEMENT_ROCKETS, {4, 56}},     {SFG_LEVEL_ELEMENT_ROCKETS, {5, 56}},
      {SFG_LEVEL_ELEMENT_PLASMA, {3, 57}},     {SFG_LEVEL_ELEMENT_PLASMA, {3, 58}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 44}},     {SFG_LEVEL_ELEMENT_TREE, {16, 45}},
      {SFG_LEVEL_ELEMENT_TREE, {16, 46}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_CARD0, {13, 20}},     {SFG_LEVEL_ELEMENT_CARD1, {14, 20}},
      {SFG_LEVEL_ELEMENT_CARD2, {13, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_LOCK1, {3, 21}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_FINISH, {8, 44}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {11, 12}},     {SFG_LEVEL_ELEMENT_TELEPORT, {17, 20}},
      {SFG_LEVEL_ELEMENT_TELEPORT, {20, 40}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}},
      {SFG_LEVEL_ELEMENT_NONE, {0, 0}},     {SFG_LEVEL_ELEMENT_NONE, {0, 0}}
    }
  },
};

#endif // guard

