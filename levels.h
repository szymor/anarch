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
} SFG_Map;

typedef struct
{
  SFG_Map map;
} SFG_Level;

static inline SFG_TileDefinition SFG_getMapTile
(
  const SFG_Map *map,
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

  uint8_t tile = map->mapArray[y * SFG_MAP_SIZE + x];

  *properties = tile & 0xc0;
  return map->tileDictionary[tile & 0x3f];
}

static const SFG_Level SFG_level0 =
{
  { // map
    { // map.mapArray
      #define o 0
      #define AA (0x04 | SFG_TILE_PROPERTY_ELEVATOR)
      #define BB (0x04 | SFG_TILE_PROPERTY_SQUEEZER)
      #define CC (0x02 | SFG_TILE_PROPERTY_DOOR)
      1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,3 ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
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
    },
    { // map.tileDictionary
      SFG_TD(0 ,31,0,0),SFG_TD(15,63,5,0),SFG_TD(28,63,1,0),SFG_TD(6 ,63,6,0), // 0
      SFG_TD(10,28,2,4),SFG_TD(4 ,26,4,6),SFG_TD(5 ,63,4,0),SFG_TD(1 ,10,1,0), // 4
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 8
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 12
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 16
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 20
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 24
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 28
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 32
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 36
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 40
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 44
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 48
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 52
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 56
      SFG_TD(0 ,31,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0),SFG_TD(0 ,63,0,0), // 60
    },
    {0, 1, 2, 3, 4, 5, 6}, // map.textureIndices
    0                      // map.doorTextureIndex
  }
};

#endif // guard

