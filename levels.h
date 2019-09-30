#ifndef _SFG_LEVELS_H
#define _SFG_LEVELS_H

#define SFG_MAP_SIZE 64
#define SFG_TILE_DICTIONARY_SIZE 64

typedef uint16_t SFG_TileDefinition;
/**<
  Defines a single game map tile. The format is following:

    MSB aaabbbbb cccddddd LSB

   aaa:  ceiling texture index (from texture available on the map), 111
         means completely transparent texture
   bbbb: ceiling height (1111 meaning no ceiling) ABOVE the floor
   ccc:  floor texture index, 111 means completely transparent texture
   dddd: floor height
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

static const SFG_Level SFG_level0 =
{
  { // mapArray
    #define o 0
    #define AA (0x09 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x0f | SFG_TILE_PROPERTY_ELEVATOR)
    #define CC (0x11 | SFG_TILE_PROPERTY_ELEVATOR)
    #define DD (0x1f | SFG_TILE_PROPERTY_ELEVATOR)
    22,22,22,22,22,22,22,22,1 ,11,11,11,11,11,28,11,11,28,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25,
    22,10,10,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25,
    12,10,10,10,10,10,10,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,21,o ,21,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,25,25,25,25,25,25,25,25,25,25,25,25,
    22,10,10,10,10,10,10,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25,
    22,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25,
    22,10,10,10,22,10,10,10,10,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25,
    12,10,10,10,22,10,10,10,1 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25,
    22,10,10,10,22,22,22,22,1 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,11,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,25,
    22,22,10,10,22,23,23,22,1 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,6 ,6 ,6 ,6 ,6 ,6 ,5 ,5 ,5 ,5 ,5 ,25,
    22,22,10,10,22,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25,
    22,22,10,10,24,23,23,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,5 ,5 ,13,o ,o ,o ,o ,o ,o ,o ,o ,o ,13,o ,o ,26,26,26,26,3 ,3 ,o ,o ,o ,o ,8 ,8 ,8 ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25,
    22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,13,13,13,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25,
    22,22,10,10,24,23,23,30,29,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,26,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25,
    22,10,10,10,22,23,23,22,1 ,o ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,25,
    12,10,10,10,22,23,23,22,1 ,8 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11,
    22,10,10,10,22,23,23,22,1 ,7 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11,
    12,10,10,10,22,23,23,22,1 ,6 ,32,32,32,33,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11,
    22,10,10,10,10,30,22,22,1 ,5 ,32,32,32,32,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11,
    12,10,10,10,10,DD,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,4 ,20,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11,
    22,10,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,5 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,19,19,19,19,19,o ,o ,11,
    22,22,10,10,10,22,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,6 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11,
    11,1 ,1 ,10,1 ,1 ,1 ,1 ,1 ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,7 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11,
    11,8 ,7 ,6 ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,8 ,19,20,20,20,20,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,19,19,19,19,19,19,19,o ,o ,11,
    11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,19,19,19,19,19,19,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,
    11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,o ,1 ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,
    11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,11,
    11,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,3 ,3 ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,16,o ,o ,o ,o ,o ,o ,o ,11,
    11,o ,18,18,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,22,22,22,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,16,o ,o ,2 ,2 ,2 ,2 ,o ,11,
    11,o ,18,18,o ,o ,o ,o ,o ,o ,22,22,22,22,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,22,AA,22,AA,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,16,o ,o ,2 ,2 ,2 ,2 ,o ,11,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,2 ,2 ,o ,11,
    22,22,22,22,22,22,22,1 ,o ,o ,1 ,1 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11,
    22,22,22,22,22,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,1 ,1 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,6 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,7 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,8 ,11,
    22,22,10,10,10,10,22,1 ,o ,o ,5 ,5 ,6 ,7 ,8 ,o ,1 ,o ,1 ,o ,o ,1 ,o ,1 ,o ,8 ,7 ,6 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11,
    22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,22,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,22,22,22,1 ,o ,o ,2 ,2 ,o ,2 ,o ,11,
    22,10,10,10,10,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,22,1 ,1 ,1 ,22,o ,o ,2 ,2 ,o ,2 ,o ,11,
    22,10,10,22,22,10,10,1 ,o ,o ,5 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,2 ,2 ,2 ,o ,2 ,o ,11,
    22,10,10,22,22,10,10,1 ,o ,o ,6 ,5 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11,
    22,10,10,10,10,10,10,1 ,o ,o ,7 ,5 ,AA,1 ,1 ,14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,22,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,2 ,o ,11,
    22,10,10,10,10,10,10,1 ,o ,o ,8 ,5 ,1 ,1 ,1 ,14,14,14,14,o ,o ,o ,o ,o ,o ,o ,14,14,1 ,1 ,1 ,1 ,1 ,1 ,22,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,BB,11,
    22,22,10,10,10,10,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11,
    22,22,22,10,10,22,22,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,2 ,2 ,2 ,2 ,2 ,11,
    1 ,1 ,1 ,10,10,1 ,1 ,1 ,o ,o ,3 ,3 ,BB,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,BB,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,2 ,o ,o ,o ,o ,o ,o ,11,
    2 ,2 ,13,5 ,5 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11,
    2 ,2 ,13,6 ,6 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11,
    2 ,2 ,13,7 ,7 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11,
    2 ,2 ,13,8 ,8 ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,7 ,o ,o ,o ,7 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11,
    2 ,2 ,13,o ,o ,13,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,o ,o ,5 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,27,11,
    13,3 ,3 ,o ,o ,o ,o ,3 ,3 ,3 ,5 ,5 ,5 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,27,11,
    3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11,
    3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,o ,27,11,
    3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,11,11,11,11,1 ,1 ,1 ,o ,BB,11,
    3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,5 ,14,14,14,14,14,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,22,1 ,1 ,22,5 ,5 ,5 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,8 ,8 ,8 ,8 ,1 ,1 ,1 ,1 ,1 ,1 ,
    3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,6 ,6 ,6 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,
    3 ,o ,o ,o ,o ,o ,o ,21,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,7 ,7 ,7 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 ,
    3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,o ,o ,1 ,1 ,1 ,1 ,8 ,8 ,8 ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,16,16,16,16,12,1 ,
    3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,18,18,18,1 ,1 ,1 ,1 ,1 ,1 ,1 ,18,18,14,14,14,14,14,14,14,14,14,14,14,CC,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,2 ,6 ,7 ,8 ,o ,o ,o ,o ,o ,11,o ,o ,o ,o ,16,16,16,16,12,1 ,
    14,14,14,14,14,14,14,14,14,14,14,14,14,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,13,13,1 ,1 ,1 ,22,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,1 ,1 ,1 ,1 ,1 ,1 ,
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
  },
  { // tileDictionary
    SFG_TD(10,31,4,0),SFG_TD(30,31,1,0),SFG_TD(0 ,31,4,0),SFG_TD(14,31,3,0), // 0
    SFG_TD(15,31,4,0),SFG_TD(14,31,4,0),SFG_TD(13,31,4,0),SFG_TD(12,31,4,0), // 4
    SFG_TD(11,31,4,0),SFG_TD(10,30,2,0),SFG_TD(14, 5,5,1),SFG_TD(20,31,5,0), // 8
    SFG_TD(30,31,6,0),SFG_TD(17,31,4,0),SFG_TD(26,31,1,0),SFG_TD(0 ,10,0,0), // 12
    SFG_TD(10, 5,1,1),SFG_TD(10,26,0,0),SFG_TD(14,31,0,0),SFG_TD(18,31,1,0), // 16
    SFG_TD(15,31,1,0),SFG_TD(14,31,1,0),SFG_TD(30,31,5,0),SFG_TD(12, 7,5,1), // 20
    SFG_TD(13, 6,5,1),SFG_TD(25,31,5,0),SFG_TD(13,31,5,0),SFG_TD(4 ,31,4,0), // 24
    SFG_TD(20,31,3,0),SFG_TD(31,0 ,1,1),SFG_TD(30,0 ,5,1),SFG_TD(14,30,0,0), // 28
    SFG_TD(28,31,1,0),SFG_TD(29,31,4,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 32
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 36
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 40
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 44
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 48
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 52
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 56
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 60
  },
  {0, 1, 2, 6, 3, 12, 10}, // textureIndices
  7,                       // doorTextureIndex
  10,                      // floorColor
  32                       // ceilingColor
};

static const SFG_Level SFG_level1 =
{
  { // mapArray
    #define o 0
    #define AA (0x04 | SFG_TILE_PROPERTY_ELEVATOR)
    #define BB (0x04 | SFG_TILE_PROPERTY_SQUEEZER)
    #define CC (0x02 | SFG_TILE_PROPERTY_DOOR)
    #define DD (0x03 | SFG_TILE_PROPERTY_DOOR)
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,DD,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,AA,BB,5 ,o ,BB,o ,AA,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,CC,2 ,2 ,2 ,2 ,CC,5 ,5 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,5 ,5 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,2 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    1 ,o ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
    #undef o
    #undef AA
    #undef BB
    #undef CC
    #undef DD
  },
  { // tileDictionary
    SFG_TD(0 ,31,0,0),SFG_TD(15,31,6,0),SFG_TD(28,31,1,0),SFG_TD(4 ,31,6,0), // 0
    SFG_TD(10,28,2,4),SFG_TD(4 ,26,4,6),SFG_TD(5 ,31,4,0),SFG_TD(1 ,10,1,0), // 4
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 8
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
    SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0),SFG_TD(0 ,31,0,0), // 60
  },
  {0, 1, 2, 3, 4, 5, 11}, // textureIndices
  7,                      // doorTextureIndex
  10,                     // floorColor
  32                      // ceilingColor
};

#endif // guard

