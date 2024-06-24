/**
  Program to export maps from Anarch as a 3D model in OBJ format. The code isn't
  nicest, it serves just to dig out the data. Usage is this: define which map
  you want to export (plus other things) down below, compile the program, run it
  and it will write out the 3D model in OBJ format, so just redirect it to some
  file, then you can happily import the model in any 3D software.

  by drummyfish, released under CC0 1.0, public domain
*/

#include <stdio.h>
#include "../game.h"

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

#define EXPORT_LEVEL 0     // which level to export
#define EXPORT_SPRITES 1   // whether to export items and monsters

#define MAX_ELEMENTS 131072

#define UNIT 8
#define HALF_UNIT (UNIT - UNIT / 2)
#define HEIGHT_STEP 2
#define SKY_OFFSET (UNIT * 2)

#define UV_STEPS 8

typedef struct
{
  int x;
  int y;
  int z;
} Vertex;

typedef struct
{
  unsigned int v0;
  unsigned int v1;
  unsigned int v2;

  unsigned int material; /* 0 - 16: walls,
                            0x01XX: flat color XX
                            0x02XX: item XX
                            0x03XX: monster XX 
                            0x04XX: skybox XX */
} Triangle;

Vertex vertices[MAX_ELEMENTS];
unsigned int vertexCount;

Triangle triangles[MAX_ELEMENTS];
unsigned int triangleCount;

void addTriangle(int coords[9], unsigned int material)
{
  Triangle t;

  if (triangleCount >= MAX_ELEMENTS)
    return;

  t.material = material;

  int *c = coords;

  for (int i = 0; i < 3; ++i)
  {
    int index = -1;

    for (int j = 0; j < vertexCount; ++j)
      if (vertices[j].x == c[0] && vertices[j].y == c[1] &&
        vertices[j].z == c[2])
      {
        // vertex already exists
        index = j;
        break;
      }

    if (index < 0)
    {
      if (vertexCount < MAX_ELEMENTS)
      {
        index = vertexCount;
        vertices[vertexCount].x = c[0];
        vertices[vertexCount].y = c[1];
        vertices[vertexCount].z = c[2];
        vertexCount++;
      }
      else
        return; // can't add vertex => can't add triangle
    }

    if (i == 0)
      t.v0 = index;
    else if (i == 1)
      t.v1 = index;
    else
      t.v2 = index;

    c += 3;
  }

  triangles[triangleCount] = t;
  triangleCount++;
}

void flipTriangle(Triangle *triangle)
{
  int tmpV = triangle->v0;
  triangle->v0 = triangle->v1;
  triangle->v1 = tmpV;
}

void makeTriangleUVs(Triangle *t, int *uv0, int *uv1, int *uv2)
{
  #define determineUV(va,vb,vc,c1,c2) \
  2 * (!((vertices[t->vb].c1 > vertices[t->va].c1) || (vertices[t->vc].c1 > vertices[t->va].c1))) + \
    (vertices[t->vb].c2 < vertices[t->va].c2 || vertices[t->vc].c2 < vertices[t->va].c2)

  #define wind(c0,c1) \
    (vertices[t->v1].c0 - vertices[t->v0].c0) * (vertices[t->v2].c1 - vertices[t->v1].c1) - \
    (vertices[t->v1].c1 - vertices[t->v0].c1) * (vertices[t->v2].c0 - vertices[t->v1].c0)
  
  int winding = 0;
  int xAligned = (vertices[t->v0].z == vertices[t->v1].z) &&
                   (vertices[t->v0].z == vertices[t->v2].z);

  if (vertices[t->v0].y == vertices[t->v1].y &&
      vertices[t->v0].y == vertices[t->v2].y)
  {
    // aligned with Y
    *uv0 = determineUV(v0,v1,v2,x,z);
    *uv1 = determineUV(v1,v0,v2,x,z);
    *uv2 = determineUV(v2,v0,v1,x,z);
  }
  else if ((vertices[t->v0].z == vertices[t->v1].z) &&
    (vertices[t->v0].z == vertices[t->v2].z))
  {
    // aligned with X
    *uv0 = determineUV(v0,v1,v2,y,x);
    *uv1 = determineUV(v1,v0,v2,y,x);
    *uv2 = determineUV(v2,v0,v1,y,x);

    winding = wind(y,x);
  }
  else
  {
    // aligned with Z
    *uv0 = determineUV(v0,v1,v2,y,z);
    *uv1 = determineUV(v1,v0,v2,y,z);
    *uv2 = determineUV(v2,v0,v1,y,z);

    winding = wind(y,z);
  }

  if (winding > 0)
  {
    *uv0 = *uv0 ^ 0x01;
    *uv1 = *uv1 ^ 0x01;
    *uv2 = *uv2 ^ 0x01;
  }

  if (vertices[t->v0].x < 0 || vertices[t->v0].x > (63 * UNIT)) // skybox?
  {
    if (vertices[t->v0].y == vertices[t->v1].y &&
        vertices[t->v0].y == vertices[t->v2].y)
    {
      *uv0 = 2 * (vertices[t->v0].y > 0);
      *uv1 = *uv0;
      *uv2 = *uv0;
    }

    return;
  }

  int height = 1;

  int dy = (vertices[t->v0].y - vertices[t->v1].y) / UNIT;

  if (dy < 0)
    dy *= -1;

  if (dy > height)
    height = dy;

  dy = (vertices[t->v0].y - vertices[t->v2].y) / UNIT;

  if (dy < 0)
    dy *= -1;

  if (dy > height)
    height = dy;

  if (height >= UV_STEPS)
    height = UV_STEPS - 1;

  height = (height - 1) * 2;

  if (*uv0 > 1)
    *uv0 += height;

  if (*uv1 > 1)
    *uv1 += height;

  if (*uv2 > 1)
    *uv2 += height;
}

void printObj(void)
{
  for (int i = 0; i < UV_STEPS; ++i)
    printf("vt 0 %d\nvt 1 %d\n",i,i);

  for (int i = 0; i < vertexCount; ++i)
    printf("v %.4f %.4f %.4f\n",
      ((float) vertices[i].x) / ((float) UNIT),
      ((float) vertices[i].y) / ((float) UNIT),
      ((float) vertices[i].z) / ((float) UNIT));

  puts("s off");

  int lastMaterial = -1;

  for (int i = 0; i < triangleCount; ++i)
  {
    if (triangles[i].material != lastMaterial)
    {
      lastMaterial = triangles[i].material;

      switch (lastMaterial >> 8)
      {
        case 0: printf("usemtl wall%d\n",lastMaterial); break;
        case 1: printf("usemtl flat%d\n",lastMaterial & 0xff); break;
        case 2: printf("usemtl item%d\n",lastMaterial & 0xff); break;
        case 3: printf("usemtl mons%d\n",lastMaterial & 0xff); break;
        case 4: printf("usemtl back%d\n",lastMaterial & 0xff); break;
        default: break;
      }
    }

    int uv0, uv1, uv2;

    makeTriangleUVs(triangles + i,&uv0,&uv1,&uv2);

    printf("f %d/%d %d/%d %d/%d\n",
      triangles[i].v0 + 1,uv0 + 1,
      triangles[i].v1 + 1,uv1 + 1,
      triangles[i].v2 + 1,uv2 + 1);
  }
}
  
int coords[9];

void addHorizontalTile(int x, int y, int z, int flip, unsigned int material)
{
  flip = flip ? 3 : 0;

  x *= UNIT;
  z *= UNIT;

  coords[0] = x;               coords[1] = y;        coords[2] = z;
  coords[3 + flip] = x;        coords[4 + flip] = y; coords[5 + flip] = z + UNIT;
  coords[6 - flip] = x + UNIT; coords[7 - flip] = y; coords[8 - flip] = z;
  addTriangle(coords,material);

  coords[0] = x + UNIT;        coords[1] = y;        coords[2] = z + UNIT;
  coords[3 + flip] = x + UNIT; coords[4 + flip] = y; coords[5 + flip] = z;
  coords[6 - flip] = x;        coords[7 - flip] = y; coords[8 - flip] = z + UNIT;
  addTriangle(coords,material);
}

void addSprite(int x, int y, unsigned int material, int doubleSided)
{
  int z = (SFG_floorHeightAt(x,y) / SFG_WALL_HEIGHT_STEP) * HEIGHT_STEP;

  x = 63 - x;

  x *= UNIT;
  y *= UNIT;

  int xc = x + HALF_UNIT;
  int yc = y + HALF_UNIT;

  for (int i = 0; i < 2; ++i)
  {
    coords[0] = i ? xc : x;          coords[1] = z;        coords[2] = i ? y : yc;
    coords[3] = i ? xc : x;          coords[4] = z + UNIT; coords[5] = i ? y : yc;
    coords[6] = i ? xc : (x + UNIT); coords[7] = z + UNIT; coords[8] = i ? y + UNIT : yc;
    addTriangle(coords,material);

    if (doubleSided)
    {
      addTriangle(coords,material);
      flipTriangle(triangles + triangleCount - 1);
    }

    coords[3] = i ? xc : (x + UNIT); coords[4] = z; coords[5] = i ? y + UNIT : yc;
    addTriangle(coords,material);
    flipTriangle(triangles + triangleCount - 1);

    if (doubleSided)
      addTriangle(coords,material);
  }  
}

void addVerticalTile(int x, int y1, int y2, int z, int turn, int flip, unsigned int material)
{
  int addX = turn ? UNIT : 0;
  int addZ = turn ? 0 : UNIT;
  Vertex tmpV;

  x *= UNIT;
  z *= UNIT;

  if (y2 < y1)
  {
    int tmp = y1;
    y1 = y2;
    y2 = tmp;
  }

  coords[0] = x;        coords[1] = y1;  coords[2] = z;
  coords[3] = x + addX; coords[4] = y1;  coords[5] = z + addZ;
  coords[6] = x;        coords[7] = y2;  coords[8] = z;
  addTriangle(coords,material);

  if (flip)
    flipTriangle(triangles + triangleCount - 1);

  coords[0] = x + addX; coords[1] = y2;  coords[2] = z + addZ;
  coords[3] = x;        coords[4] = y2;  coords[5] = z;
  coords[6] = x + addX; coords[7] = y1;  coords[8] = z + addZ;
  addTriangle(coords,material);

  if (flip)
    flipTriangle(triangles + triangleCount - 1);
}

void getTile(int x, int y, int ceiling, int *height, int *texture)
{
  if (x < 0 || x >= 64 || y < 0 || y >= 64)
  {
    *height = 0;
    *texture = -1;
    return;
  }
  
  uint8_t p;
  
  SFG_TileDefinition t = SFG_getMapTile(SFG_currentLevel.levelPointer,63 - x,y,&p);

  RCL_Unit (*heightFunc)(int16_t, int16_t) = ceiling ?
    SFG_ceilingHeightAt : SFG_floorHeightAt;

  *height = (heightFunc(63 - x,y) / SFG_WALL_HEIGHT_STEP) * HEIGHT_STEP;

  *texture = ceiling ? (SFG_TILE_CEILING_TEXTURE(t)) : (SFG_TILE_FLOOR_TEXTURE(t));

  *texture = (*texture != SFG_TILE_TEXTURE_TRANSPARENT) ?
     (
       ((!ceiling) && p == SFG_TILE_PROPERTY_DOOR) ?
       SFG_currentLevel.levelPointer->doorTextureIndex :
       SFG_currentLevel.levelPointer->textureIndices[*texture] 
     ) : -1;
}

int main(void)
{
  SFG_init();

  SFG_setAndInitLevel(EXPORT_LEVEL);

  int sky = SFG_currentLevel.levelPointer->backgroundImage + 0x0400;

  // skybox:
  for (int i = 0; i < 3; ++i)
  {
#define ADD_LEN (64 * UNIT + 2 * SKY_OFFSET)

    int cx = i, cy = (i + 1) % 3, cz = (i + 2) % 3;

    for (int j = 0; j < 9; ++j)
      coords[j] = -1 * SKY_OFFSET;

    coords[3 + cx] += ADD_LEN; 

    coords[6 + cx] = coords[3 + cx];
    coords[6 + cy] = coords[3 + cx]; 

    addTriangle(coords,sky);

    coords[cz] += ADD_LEN;
    coords[3 + cz] += ADD_LEN;
    coords[6 + cz] += ADD_LEN;
    
    addTriangle(coords,sky);
    flipTriangle(triangles + triangleCount - 1);

    coords[3 + cx] = coords[cx];
    coords[3 + cy] = coords[6 + cx];
    addTriangle(coords,sky);

    coords[cz] -= ADD_LEN;
    coords[3 + cz] -= ADD_LEN;
    coords[6 + cz] -= ADD_LEN;

    addTriangle(coords,sky);
    flipTriangle(triangles + triangleCount - 1);
  }

  RCL_Unit (*heightFunc)(int16_t, int16_t) = SFG_floorHeightAt;

  for (int i = 0; i < 2; ++i) // floor and ceiling
  {
    for (int y = 0; y < 64; ++y)
    {
      for (int x = 0; x < 64; ++x)
      {
        int h, t, h2, t2;

        getTile(x,y,i,&h,&t);
        getTile(x,y,!i,&h2,&t2);

        if (h != h2 && (i == 0 || h != (SFG_CEILING_MAX_HEIGHT / SFG_WALL_HEIGHT_STEP) * HEIGHT_STEP))
          addHorizontalTile(x,h,y,i,0x0100 + 
            (i ? SFG_currentLevel.ceilingColor : SFG_currentLevel.floorColor));

        for (int j = 0; j < 2; ++j) // walls
        {
          int x2 = x + (j == 0);
          int y2 = y + (j == 1);

          if (x2 < 64 && y2 < 64)
            getTile(x2,y2,i,&h2,&t2);
          else
            h2 = h;

          if (h2 != h)
          {
            int goingUp = h2 > h;

            int texture = (goingUp != i) ? t2 : t;

            if (texture != -1)
              addVerticalTile(x2,h,h2,y2,j,goingUp != (i == j),texture);
          }
        }
      }
    }
  }

  if (EXPORT_SPRITES)
    for (int i = 0; i < SFG_MAX_LEVEL_ELEMENTS; ++i)
    {
      SFG_LevelElement e = SFG_currentLevel.levelPointer->elements[i];

      if (e.type != SFG_LEVEL_ELEMENT_NONE && e.type != SFG_LEVEL_ELEMENT_LOCK0 &&
          e.type != SFG_LEVEL_ELEMENT_LOCK1 && e.type != SFG_LEVEL_ELEMENT_LOCK2 &&
          e.type != SFG_LEVEL_ELEMENT_BLOCKER)
        addSprite(e.coords[0],e.coords[1],0x0200 + ((e.type & 0xf0) == 0x20) * 0x0100 + e.type,1);
    }

  printObj();

  return 0;
}
