/** Simple program to export Anarch maps as isometric images in PPM format.
  The code is not very nice, the goal is just to get the images :)

  by drummyfish, released under CC0 */

#include <stdio.h>
#include "../game.h"

#define IMAGE_W 4096 // if changed you also have to change PPM header data
#define IMAGE_H 2500

#define CENTER_X (IMAGE_W / 2)
#define CENTER_Y (IMAGE_H / 6)

#define TILE_H 25    // vertical height
#define TILE_TILT 2  // view angle, by how many H pixels we'll shift one down
 
unsigned char image[IMAGE_H * IMAGE_W * 3];

int maxHeights[4]; // ceiling limits for different quadrants

void drawPixel(int x, int y, unsigned char color, int multiply)
{
  if (x < 0 || x >= IMAGE_W || y < 0 || y >= IMAGE_H)
    return;

  uint16_t rgb = paletteRGB565[color];

  int index = (y * IMAGE_W + x) * 3;

  image[index] = ((((rgb >> 11) << 3) & 0xff) * multiply) / 128;
  image[index + 1] = ((((rgb >> 5) << 2) & 0xff) * multiply) / 128;
  image[index + 2] = (((rgb << 3) & 0xff) * multiply) / 128;
}

void drawColum(int x, int y, int z1, int z2, int texture, int doorTexture, const uint8_t *sprite)
{
  if (texture == SFG_TILE_TEXTURE_TRANSPARENT)
    return;

  int inc = z2 > z1 ? 1 : -1;

  int minZ = z1 < z2 ? z1 : z2;

  int brightness = (minZ / 2) % 128;

  if (brightness < 0)
    brightness *= -1;

  brightness += 30;

  for (int i = 0; i < SFG_TEXTURE_SIZE; ++i) // platform
    for (int j = -1 * i / TILE_TILT; j <= i / TILE_TILT; ++j)
    {
      drawPixel(x + i,y + minZ + j,SFG_currentLevel.floorColor,brightness);
      drawPixel(x + 2 * SFG_TEXTURE_SIZE - 1 - i,y + minZ + j,SFG_currentLevel.floorColor,brightness);
    }

  if (sprite != 0)
    for (int sy = 0; sy < SFG_TEXTURE_SIZE; ++sy)
      for (int sx = 0; sx < SFG_TEXTURE_SIZE; ++sx)
      {
        uint8_t color = SFG_getTexel(sprite,sx,sy);

        if (color != SFG_TRANSPARENT_COLOR)
          drawPixel(x + sx + SFG_TEXTURE_SIZE / 2,y + minZ + sy - SFG_TEXTURE_SIZE,color,110);
      }

  if (z1 == z2)
    return;

  z1 += inc;

  int texY = 0;

  while (z1 != z2) // column (walls)
  {
    int ty = (texY * SFG_TEXTURE_SIZE) / TILE_H;

    uint8_t *t = (doorTexture < 0 || ty > SFG_TEXTURE_SIZE) ?
      SFG_currentLevel.textures[texture] : 
      (SFG_wallTextures + doorTexture * SFG_TEXTURE_STORE_SIZE);

    for (int i = 0; i < SFG_TEXTURE_SIZE; ++i)
    {
      uint8_t color = SFG_getTexel(t,i,ty);

      if (color != SFG_TRANSPARENT_COLOR)
        drawPixel(x + i,y + z1 + i / TILE_TILT,color,75);

      color = SFG_getTexel(t,SFG_TEXTURE_SIZE - i,ty);

      if (color != SFG_TRANSPARENT_COLOR)
        drawPixel(x + 2 * SFG_TEXTURE_SIZE - 1 - i,y + z1 + i / TILE_TILT,
        color,128);
    }

    texY++;
    z1 += inc;
  }
}

void tileIndexToXY(int n, int *x, int *y)
{
  int reverse = 0;

  if (n > SFG_MAP_SIZE * SFG_MAP_SIZE / 2)
  {
    reverse = 1;
    n = SFG_MAP_SIZE * SFG_MAP_SIZE - 1 - n;
  }

  *y = 0;
  *x = 0;

  while (*y < n)
  {
    *y += 1;
    n -= *y;
  }

  while (n > 0)
  {
    *x += 1;
    *y -= 1;

    n--;
  }

  if (reverse)
  {
    *x = SFG_MAP_SIZE - 1 - *x;
    *y = SFG_MAP_SIZE - 1 - *y;
  }

  *x = SFG_MAP_SIZE - 1 - *x;
}

void exportMap(int index)
{
  SFG_setAndInitLevel(index);

  for (int i = 0; i < IMAGE_H * IMAGE_W * 3; ++i)
    image[i] = 0;

  unsigned char header[] =
    {'P', '6', ' ', '4', '0', '9', '6',   ' ',   '2', '5', '0', '0',  ' ',  
    '2', '5', '5', '\n'};

  char fname[] = "map0.ppm";

  fname[3] += index;

  FILE *f = fopen(fname,"wb");

  fwrite(header,sizeof(header),1,f);

  int n = 0;

  for (int drawY = 0; drawY < SFG_MAP_SIZE; ++drawY)
  {
    for (int i = 0; i < 2; ++i)
    {
      int xLimit = (1 + 2 * drawY + i);

      if (drawY >= SFG_MAP_SIZE / 2)
        xLimit = SFG_MAP_SIZE * 2 - xLimit;

#define TW (2 * SFG_TEXTURE_SIZE)
#define TH (SFG_TEXTURE_SIZE / TILE_TILT)

      int startX = -1 * xLimit * TW / 2;

      for (int drawX = 0; drawX < xLimit; ++drawX)   
      {
        uint8_t properties;
        int tx, ty;

        tileIndexToXY(n,&tx,&ty);

        int maxHeightTiles = maxHeights[2 * ((ty * 2) / SFG_MAP_SIZE) +
          (tx * 2) / SFG_MAP_SIZE];

        SFG_TileDefinition tile =
          SFG_getMapTile(SFG_currentLevel.levelPointer,tx,ty,&properties);

        int h = SFG_TILE_FLOOR_HEIGHT(tile);

        if (properties == SFG_TILE_PROPERTY_ELEVATOR)
        {
          h += SFG_TILE_CEILING_HEIGHT(tile);
          h /= 2;
        }

        if (maxHeightTiles * 4 < h)
          h = maxHeightTiles * 4;

        h = (h * SFG_WALL_HEIGHT_STEP * TILE_H) / RCL_UNITS_PER_SQUARE;

        const uint8_t *sprite = 0;

        for (int i = 0; i < SFG_MAX_LEVEL_ELEMENTS; ++i)
        {
          SFG_LevelElement e =
            SFG_currentLevel.levelPointer->elements[i];

          if (e.type == SFG_LEVEL_ELEMENT_NONE)
            break;

          if (e.coords[0] == tx && e.coords[1] == ty   &&     !(e.type & 0x10))
          {
            uint8_t ss;

            if (e.type & 0x20)
              sprite = SFG_getMonsterSprite(e.type,0,0);
            else
              SFG_getItemSprite(e.type,&sprite,&ss);
             
            break;
          } 
        }

        drawColum(
          CENTER_X + startX + drawX * TW,
          CENTER_Y + (2 * drawY + i) * TH - drawY,-1 * h,
          0,SFG_TILE_FLOOR_TEXTURE(tile),
          (properties == SFG_TILE_PROPERTY_DOOR) ?
          SFG_currentLevel.levelPointer->doorTextureIndex : -1,
          sprite);

       int maxH = 16 * TILE_H;

       if (maxHeightTiles * TILE_H < maxH)
         maxH = maxHeightTiles * TILE_H;

       maxH *= -1;

       h = SFG_TILE_FLOOR_HEIGHT(tile) + SFG_TILE_CEILING_HEIGHT(tile);
       h = -1 * (h * SFG_WALL_HEIGHT_STEP * TILE_H) / RCL_UNITS_PER_SQUARE + 1;

       if (SFG_TILE_CEILING_HEIGHT(tile) < 31 &&
         maxH < h && properties != SFG_TILE_PROPERTY_ELEVATOR)
       {
         drawColum(
           CENTER_X + startX + drawX * TW,
           CENTER_Y + (2 * drawY + i) * TH - drawY,maxH,
           h,SFG_TILE_CEILING_TEXTURE(tile),-1,0);
       }

       n++;
      }
    }
  }

  fwrite(image,1,IMAGE_W * IMAGE_H * 3,f);

  fclose(f);
}

int main(void)
{
  SFG_init();

#define E(m,h1,h2,h3,h4)\
  printf("exporting %d\n",m);\
  maxHeights[0] = h1;\
  maxHeights[1] = h2;\
  maxHeights[2] = h3;\
  maxHeights[3] = h4;\
  exportMap(m);

  E(0,100,100,100,100)
  E(1,7,5,7,5)
  E(2,5,6,5,5)
  E(3,7,7,6,6)
  E(4,100,6,100,6)
  E(5,100,100,100,100)
  E(6,5,4,3,3)
  E(7,7,7,7,7)
  E(8,5,5,5,5)
  E(9,100,100,100,100)

  return 0;
}

// just create empty functions, game.h requires it:
int8_t SFG_keyPressed(uint8_t key) { return 0; }
void SFG_getMouseOffset(int16_t *x, int16_t *y) { }
uint32_t SFG_getTimeMs() { return 0; }
void SFG_sleepMs(uint16_t timeMs) { }
static inline void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex) { }
void SFG_playSound(uint8_t soundIndex, uint8_t volume) { }
void SFG_setMusic(uint8_t value) { }
void SFG_processEvent(uint8_t event, uint8_t data) { }
void SFG_save(uint8_t data[SFG_SAVE_SIZE]) { }
uint8_t SFG_load(uint8_t data[SFG_SAVE_SIZE]) { return 0; }
