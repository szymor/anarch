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

#define SFG_OUTSIDE_TILE SFG_TD(63,0,7,7)

typedef uint8_t SFG_MapArray[SFG_MAP_SIZE * SFG_MAP_SIZE];
/**<
  Game map represented as a 2D array. Array item has this format:

    MSB aabbbbbb LSB

    aa:     type of square, possible values:
      00:   normal
      01:   moving floor (elevator), moves from height 0 to floor height
      10:   moving ceiling, moves from ceiling height to floor height
      11:   door
    bbbbbb: index into tile dictionary
*/

typedef struct
{
  SFG_TileDictionary tileDictionary;
  SFG_MapArray mapArray;
} SFG_Map;

typedef struct
{
  SFG_Map map;
} SFG_Level;

static inline SFG_TileDefinition SFG_getMapTile
(
  const SFG_Map *map,
  int16_t x,
  int16_t y
)
{
  if (x < 0 || x >= SFG_MAP_SIZE || y < 0 || y >= SFG_MAP_SIZE)
    return SFG_OUTSIDE_TILE;

  return map->tileDictionary[map->mapArray[y * SFG_MAP_SIZE + x] & 0x3f];
}

static inline uint8_t SFG_getMapTileProperties
(
  const SFG_Map *map,
  int16_t x,
  int16_t y
)
{
  if (x < 0 || x >= SFG_MAP_SIZE || y < 0 || y >= SFG_MAP_SIZE)
    return 0;

  return map->mapArray[y * SFG_MAP_SIZE + x] & 0xc0;
}

static const SFG_Level SFG_level0 =
{
  .map =
  {
    .tileDictionary =
    {
      SFG_TD(0 ,31,0,0),SFG_TD(5 ,63,0,0),SFG_TD(1 ,63,4,0),SFG_TD(2 ,63,4,0), // 0
      SFG_TD(3 ,31,4,0),SFG_TD(4 ,63,4,0),SFG_TD(5 ,63,4,0),SFG_TD(1 ,10,0,0), // 4
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

    .mapArray =
    {
      #define o 0
      o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      o ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,7 ,7 ,7 ,7 ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      o ,o ,o ,o ,o ,o ,o ,o ,2 ,o ,7 ,7 ,7 ,7 ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      o ,o ,o ,o ,o ,o ,o ,o ,3 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      o ,o ,o ,o ,o ,o ,o ,o ,4 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,1 ,1 ,1 ,1 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      6 ,o ,o ,o ,o ,6 ,6 ,6 ,5 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      6 ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      6 ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      6 ,o ,o ,o ,o ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
      6 ,6 ,6 ,6 ,6 ,6 ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,o ,
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
    }
  }
};

#endif // guard

