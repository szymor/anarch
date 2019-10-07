#ifndef RAYCASTLIB_H
#define RAYCASTLIB_H

/**
  raycastlib (RCL) - Small C header-only raycasting library for embedded and
  low performance computers, such as Arduino. Only uses integer math and stdint
  standard library.

  Check the defines below to fine-tune accuracy vs performance! Don't forget
  to compile with optimizations.

  Before including the library define RCL_PIXEL_FUNCTION to the name of the
  function (with RCL_PixelFunction signature) that will render your pixels!

  - All public (and most private) library identifiers start with RCL_.
  - Game field's bottom left corner is at [0,0].
  - X axis goes right in the ground plane.
  - Y axis goes up in the ground plane.
  - Height means the Z (vertical) coordinate.
  - Each game square is RCL_UNITS_PER_SQUARE * RCL_UNITS_PER_SQUARE points.
  - Angles are in RCL_Units, 0 means pointing right (x+) and positively rotates
    clockwise. A full angle has RCL_UNITS_PER_SQUARE RCL_Units.
  - Most things are normalized with RCL_UNITS_PER_SQUARE (sin, cos, vector
    unit length, texture coordinates etc.).
  - Screen coordinates are normal: [0,0] = top left, x goes right, y goes down.

  author: Miloslav "drummyfish" Ciz
  license: CC0 1.0
  version: 0.86
*/

#include <stdint.h>

#ifndef RCL_RAYCAST_TINY /** Turns on super efficient version of this library.
                             Only use if neccesarry, looks ugly. Also not done
                             yet. */
  #define RCL_UNITS_PER_SQUARE 1024 /**< Number of RCL_Units in a side of a
                                         spatial square. */
  typedef int32_t RCL_Unit; /**< Smallest spatial unit, there is
                                 RCL_UNITS_PER_SQUARE units in a square's
                                 length. This effectively serves the purpose of
                                 a fixed-point arithmetic. */
  #define RCL_INFINITY 2000000000
#else
  #define RCL_UNITS_PER_SQUARE 32
  typedef int16_t RCL_Unit;
  #define RCL_INFINITY 30000
  #define RCL_USE_DIST_APPROX 2
#endif

#ifndef RCL_COMPUTE_WALL_TEXCOORDS
#define RCL_COMPUTE_WALL_TEXCOORDS 1
#endif

#ifndef RCL_COMPUTE_FLOOR_TEXCOORDS
#define RCL_COMPUTE_FLOOR_TEXCOORDS 0
#endif

#ifndef RCL_FLOOR_TEXCOORDS_HEIGHT
#define RCL_FLOOR_TEXCOORDS_HEIGHT 0 /** If RCL_COMPUTE_FLOOR_TEXCOORDS == 1,
                                      this says for what height level the
                                      texture coords will be computed for
                                      (for simplicity/performance only one
                                      level is allowed). */
#endif

#ifndef RCL_USE_COS_LUT
#define RCL_USE_COS_LUT 0 /**< type of look up table for cos function:
                           0: none (compute)
                           1: 64 items
                           2: 128 items */
#endif

#ifndef RCL_USE_DIST_APPROX
#define RCL_USE_DIST_APPROX 0 /**< What distance approximation to use:
                            0: none (compute full Euclidean distance)
                            1: accurate approximation
                            2: octagonal approximation (LQ) */
#endif

#ifndef RCL_RECTILINEAR
#define RCL_RECTILINEAR 1 /**< Whether to use rectilinear perspective (normally
                              used), or curvilinear perspective (fish eye). */
#endif

#ifndef RCL_TEXTURE_VERTICAL_STRETCH
#define RCL_TEXTURE_VERTICAL_STRETCH 1 /**< Whether textures should be
                                       stretched to wall height (possibly
                                       slightly slower if on). */
#endif

#ifndef RCL_COMPUTE_FLOOR_DEPTH
#define RCL_COMPUTE_FLOOR_DEPTH 1 /**< Whether depth should be computed for
                                   floor pixels - turns this off if not
                                   needed. */
#endif

#ifndef RCL_COMPUTE_CEILING_DEPTH
#define RCL_COMPUTE_CEILING_DEPTH 1 /**< As RCL_COMPUTE_FLOOR_DEPTH but for
                                     ceiling. */
#endif

#ifndef RCL_ROLL_TEXTURE_COORDS
#define RCL_ROLL_TEXTURE_COORDS 1 /**< Says whether rolling doors should also
                                   roll the texture coordinates along (mostly
                                   desired for doors). */
#endif

#ifndef RCL_VERTICAL_FOV
#define RCL_VERTICAL_FOV (RCL_UNITS_PER_SQUARE / 2)
#endif

#ifndef RCL_HORIZONTAL_FOV
#define RCL_HORIZONTAL_FOV (RCL_UNITS_PER_SQUARE / 4)
#endif

#define RCL_HORIZONTAL_FOV_HALF (RCL_HORIZONTAL_FOV / 2)

#ifndef RCL_CAMERA_COLL_RADIUS
#define RCL_CAMERA_COLL_RADIUS RCL_UNITS_PER_SQUARE / 4
#endif

#ifndef RCL_CAMERA_COLL_HEIGHT_BELOW
#define RCL_CAMERA_COLL_HEIGHT_BELOW RCL_UNITS_PER_SQUARE
#endif 

#ifndef RCL_CAMERA_COLL_HEIGHT_ABOVE
#define RCL_CAMERA_COLL_HEIGHT_ABOVE (RCL_UNITS_PER_SQUARE / 3)
#endif

#ifndef RCL_CAMERA_COLL_STEP_HEIGHT
#define RCL_CAMERA_COLL_STEP_HEIGHT (RCL_UNITS_PER_SQUARE / 2)
#endif

#ifndef RCL_TEXTURE_INTERPOLATION_SCALE
  #define RCL_TEXTURE_INTERPOLATION_SCALE 1024 /**< This says scaling of fixed
                                             poit vertical texture coord
                                             computation. This should be power
                                             of two! Higher number can look more 
                                             accurate but may cause overflow. */
#endif

#define RCL_HORIZON_DEPTH (11 * RCL_UNITS_PER_SQUARE) /**< What depth the
                                                       horizon has (the floor
                                                       depth is only
                                                       approximated with the
                                                       help of this
                                                       constant). */
#ifndef RCL_VERTICAL_DEPTH_MULTIPLY
#define RCL_VERTICAL_DEPTH_MULTIPLY 2 /**< Defines a multiplier of height
                                       difference when approximating floor/ceil
                                       depth. */
#endif

#define RCL_min(a,b) ((a) < (b) ? (a) : (b))
#define RCL_max(a,b) ((a) > (b) ? (a) : (b))
#define RCL_nonZero(v) ((v) != 0 ? (v) : 1) ///< To prevent zero divisions.

#define RCL_logV2D(v)\
  printf("[%d,%d]\n",v.x,v.y);

#define RCL_logRay(r){\
  printf("ray:\n");\
  printf("  start: ");\
  RCL_logV2D(r.start);\
  printf("  dir: ");\
  RCL_logV2D(r.direction);}

#define RCL_logHitResult(h){\
  printf("hit:\n");\
  printf("  square: ");\
  RCL_logV2D(h.square);\
  printf("  pos: ");\
  RCL_logV2D(h.position);\
  printf("  dist: %d\n", h.distance);\
  printf("  dir: %d\n", h.direction);\
  printf("  texcoord: %d\n", h.textureCoord);}

#define RCL_logPixelInfo(p){\
  printf("pixel:\n");\
  printf("  position: ");\
  RCL_logV2D(p.position);\
  printf("  texCoord: ");\
  RCL_logV2D(p.texCoords);\
  printf("  depth: %d\n", p.depth);\
  printf("  height: %d\n", p.height);\
  printf("  wall: %d\n", p.isWall);\
  printf("  hit: ");\
  RCL_logHitResult(p.hit);\
  }

#define RCL_logCamera(c){\
  printf("camera:\n");\
  printf("  position: ");\
  RCL_logV2D(c.position);\
  printf("  height: %d\n",c.height);\
  printf("  direction: %d\n",c.direction);\
  printf("  shear: %d\n",c.shear);\
  printf("  resolution: %d x %d\n",c.resolution.x,c.resolution.y);\
  }

/// Position in 2D space.
typedef struct
{
  RCL_Unit x;
  RCL_Unit y;
} RCL_Vector2D;

typedef struct
{
  RCL_Vector2D start;
  RCL_Vector2D direction;
} RCL_Ray;

typedef struct
{
  RCL_Unit     distance; /**< Distance to the hit position, or -1 if no
                              collision happened. If RCL_RECTILINEAR != 0, then
                              the distance is perpendicular to the projection
                              plane (fish eye correction), otherwise it is
                              the straight distance to the ray start
                              position. */
  uint8_t      direction;    /**< Direction of hit. The convention for angle
                                  units is explained above. */
  RCL_Unit     textureCoord; /**< Normalized (0 to RCL_UNITS_PER_SQUARE - 1)
                                  texture coordinate (horizontal). */
  RCL_Vector2D square;       ///< Collided square coordinates.
  RCL_Vector2D position;     ///< Exact collision position in RCL_Units.
  RCL_Unit     arrayValue;   /**  Value returned by array function (most often
                                  this will be the floor height). */
  RCL_Unit     type;         /**< Integer identifying type of square (number
                                  returned by type function, e.g. texture
                                  index).*/
  RCL_Unit     doorRoll;     ///< Holds value of door roll.
} RCL_HitResult;

typedef struct
{
  RCL_Vector2D position;
  RCL_Unit     direction;
  RCL_Vector2D resolution;
  int16_t      shear; /**< Shear offset in pixels (0 => no shear), can simulate
                           looking up/down. */
  RCL_Unit     height;
} RCL_Camera;

/**
  Holds an information about a single rendered pixel (for a pixel function
  that works as a fragment shader).
*/
typedef struct
{
  RCL_Vector2D  position;  ///< On-screen position.
  int8_t        isWall;    ///< Whether the pixel is a wall or a floor/ceiling.
  int8_t        isFloor;   ///< Whether the pixel is floor or ceiling.
  int8_t        isHorizon; ///< If the pixel belongs to horizon segment.
  RCL_Unit      depth;     ///< Corrected depth.
  RCL_Unit      wallHeight;///< Only for wall pixels, says its height.
  RCL_Unit      height;    ///< World height (mostly for floor).
  RCL_HitResult hit;       ///< Corresponding ray hit.
  RCL_Vector2D  texCoords; /**< Normalized (0 to RCL_UNITS_PER_SQUARE - 1)
                                texture coordinates. */
} RCL_PixelInfo;

void RCL_PIXEL_FUNCTION (RCL_PixelInfo *pixel);

typedef struct
{
  uint16_t maxHits;
  uint16_t maxSteps;
} RCL_RayConstraints;

/**
  Function used to retrieve some information about cells of the rendered scene.
  It should return a characteristic of given square as an integer (e.g. square
  height, texture index, ...) - between squares that return different numbers
  there is considered to be a collision.

  This function should be as fast as possible as it will typically be called
  very often.
*/ 
typedef RCL_Unit (*RCL_ArrayFunction)(int16_t x, int16_t y);

/**
  Function that renders a single pixel at the display. It is handed an info
  about the pixel it should draw.

  This function should be as fast as possible as it will typically be called
  very often.
*/
typedef void (*RCL_PixelFunction)(RCL_PixelInfo *info);

typedef void
  (*RCL_ColumnFunction)(RCL_HitResult *hits, uint16_t hitCount, uint16_t x,
   RCL_Ray ray);

/**
  Simple-interface function to cast a single ray.
  @return          The first collision result.
*/
RCL_HitResult RCL_castRay(RCL_Ray ray, RCL_ArrayFunction arrayFunc);

/**
  Maps a single point in the world to the screen (2D position + depth).
*/
RCL_PixelInfo RCL_mapToScreen(RCL_Vector2D worldPosition, RCL_Unit height,
  RCL_Camera camera);

/**
  Casts a single ray and returns a list of collisions.

  @param ray ray to be cast, if RCL_RECTILINEAR != 0 then the computed hit
         distance is divided by the ray direction vector length (to correct
         the fish eye effect)
  @param arrayFunc function that will be used to determine collisions (hits)
         with the ray (squares for which this function returns different values
         are considered to have a collision between them), this will typically
         be a function returning floor height
  @param typeFunc optional (can be 0) function - if provided, it will be used
         to mark the hit result with the number returned by this function
         (it can be e.g. a texture index)
  @param hitResults array in which the hit results will be stored (has to be
         preallocated with at space for at least as many hit results as
         maxHits specified with the constraints parameter)
  @param hitResultsLen in this variable the number of hit results will be
         returned
  @param constraints specifies constraints for the ray cast
*/
void RCL_castRayMultiHit(RCL_Ray ray, RCL_ArrayFunction arrayFunc,
  RCL_ArrayFunction typeFunc, RCL_HitResult *hitResults,
  uint16_t *hitResultsLen, RCL_RayConstraints constraints);

RCL_Vector2D RCL_angleToDirection(RCL_Unit angle);

/**
Cos function.

@param  input to cos in RCL_Units (RCL_UNITS_PER_SQUARE = 2 * pi = 360 degrees)
@return RCL_normalized output in RCL_Units (from -RCL_UNITS_PER_SQUARE to
        RCL_UNITS_PER_SQUARE)
*/
RCL_Unit RCL_cosInt(RCL_Unit input);

RCL_Unit RCL_sinInt(RCL_Unit input);

/// Normalizes given vector to have RCL_UNITS_PER_SQUARE length.
RCL_Vector2D RCL_normalize(RCL_Vector2D v);

/// Computes a cos of an angle between two vectors.
RCL_Unit RCL_vectorsAngleCos(RCL_Vector2D v1, RCL_Vector2D v2);

uint16_t RCL_sqrtInt(RCL_Unit value);
RCL_Unit RCL_dist(RCL_Vector2D p1, RCL_Vector2D p2);
RCL_Unit RCL_len(RCL_Vector2D v);

/**
  Converts an angle in whole degrees to an angle in RCL_Units that this library
  uses.
*/   
RCL_Unit RCL_degreesToUnitsAngle(int16_t degrees);

///< Computes the change in size of an object due to perspective.
RCL_Unit RCL_perspectiveScale(RCL_Unit originalSize, RCL_Unit distance);

RCL_Unit RCL_perspectiveScaleInverse(RCL_Unit originalSize,
  RCL_Unit scaledSize);

/**
  Casts rays for given camera view and for each hit calls a user provided
  function.
*/
void RCL_castRaysMultiHit(RCL_Camera cam, RCL_ArrayFunction arrayFunc,
  RCL_ArrayFunction typeFunction, RCL_ColumnFunction columnFunc,
  RCL_RayConstraints constraints);

/**
  Using provided functions, renders a complete complex (multilevel) camera
  view.

  This function should render each screen pixel exactly once.

  function rendering summary:
  - performance:            slower
  - accuracy:               higher
  - wall textures:          yes
  - different wall heights: yes
  - floor/ceiling textures: no
  - floor geometry:         yes, multilevel
  - ceiling geometry:       yes (optional), multilevel
  - rolling door:           no
  - camera shearing:        yes
  - rendering order:        left-to-right, not specifically ordered vertically

  @param cam camera whose view to render
  @param floorHeightFunc function that returns floor height (in RCL_Units)
  @param ceilingHeightFunc same as floorHeightFunc but for ceiling, can also be
                           0 (no ceiling will be rendered)
  @param typeFunction function that says a type of square (e.g. its texture
                     index), can be 0 (no type in hit result)
  @param pixelFunc callback function to draw a single pixel on screen
  @param constraints constraints for each cast ray
*/
void RCL_renderComplex(RCL_Camera cam, RCL_ArrayFunction floorHeightFunc,
  RCL_ArrayFunction ceilingHeightFunc, RCL_ArrayFunction typeFunction,
  RCL_RayConstraints constraints);

/**
  Renders given camera view, with help of provided functions. This function is
  simpler and faster than RCL_renderComplex(...) and is meant to be rendering
  flat levels.

  function rendering summary:
  - performance:            faster
  - accuracy:               lower
  - wall textures:          yes
  - different wall heights: yes
  - floor/ceiling textures: yes (only floor, you can mirror it for ceiling)
  - floor geometry:         no (just flat floor, with depth information)
  - ceiling geometry:       no (just flat ceiling, with depth information)
  - rolling door:           yes
  - camera shearing:        no
  - rendering order:        left-to-right, top-to-bottom

  Additionally this function supports rendering rolling doors.

  This function should render each screen pixel exactly once.

  @param rollFunc function that for given square says its door roll in
         RCL_Units (0 = no roll, RCL_UNITS_PER_SQUARE = full roll right,
         -RCL_UNITS_PER_SQUARE = full roll left), can be zero (no rolling door,
         rendering should also be faster as fewer intersections will be tested)
*/
void RCL_renderSimple(RCL_Camera cam, RCL_ArrayFunction floorHeightFunc,
  RCL_ArrayFunction typeFunc, RCL_ArrayFunction rollFunc,
  RCL_RayConstraints constraints);

/**
  Function that moves given camera and makes it collide with walls and
  potentially also floor and ceilings. It's meant to help implement player
  movement.

  @param camera camera to move
  @param planeOffset offset to move the camera in
  @param heightOffset height offset to move the camera in
  @param floorHeightFunc function used to retrieve the floor height
  @param ceilingHeightFunc function for retrieving ceiling height, can be 0
                           (camera won't collide with ceiling)
  @param computeHeight whether to compute height - if false (0), floor and
                       ceiling functions won't be used and the camera will
                       only collide horizontally with walls (good for simpler
                       game, also faster)
  @param force if true, forces to recompute collision even if position doesn't
               change
*/
void RCL_moveCameraWithCollision(RCL_Camera *camera, RCL_Vector2D planeOffset,
  RCL_Unit heightOffset, RCL_ArrayFunction floorHeightFunc,
  RCL_ArrayFunction ceilingHeightFunc, int8_t computeHeight, int8_t force);

void RCL_initCamera(RCL_Camera *camera);
void RCL_initRayConstraints(RCL_RayConstraints *constraints);

//=============================================================================
// privates

#define _RCL_UNUSED(what) (void)(what);

// global helper variables, for precomputing stuff etc.
RCL_Camera _RCL_camera;
RCL_Unit _RCL_horizontalDepthStep = 0; 
RCL_Unit _RCL_startFloorHeight = 0;
RCL_Unit _RCL_startCeil_Height = 0;
RCL_Unit _RCL_camResYLimit = 0;
RCL_Unit _RCL_middleRow = 0;
RCL_ArrayFunction _RCL_floorFunction = 0;
RCL_ArrayFunction _RCL_ceilFunction = 0;
RCL_Unit _RCL_fHorizontalDepthStart = 0;
RCL_Unit _RCL_cHorizontalDepthStart = 0;
int16_t _RCL_cameraHeightScreen = 0;
RCL_ArrayFunction _RCL_rollFunction = 0; // says door rolling
RCL_Unit *_RCL_floorPixelDistances = 0;

#ifdef RCL_PROFILE
  // function call counters for profiling
  uint32_t profile_RCL_sqrtInt = 0;
  uint32_t profile_RCL_clamp = 0;
  uint32_t profile_RCL_cosInt = 0;
  uint32_t profile_RCL_angleToDirection = 0;
  uint32_t profile_RCL_dist = 0;
  uint32_t profile_RCL_len = 0;
  uint32_t profile_RCL_pointIsLeftOfRay = 0;
  uint32_t profile_RCL_castRayMultiHit = 0; 
  uint32_t profile_RCL_castRay = 0;
  uint32_t profile_RCL_absVal = 0;
  uint32_t profile_RCL_normalize = 0;
  uint32_t profile_RCL_vectorsAngleCos = 0;
  uint32_t profile_RCL_perspectiveScale = 0;
  uint32_t profile_RCL_wrap = 0;
  uint32_t profile_RCL_divRoundDown = 0;
  #define RCL_profileCall(c) profile_##c += 1

  #define printProfile() {\
    printf("profile:\n");\
    printf("  RCL_sqrtInt: %d\n",profile_RCL_sqrtInt);\
    printf("  RCL_clamp: %d\n",profile_RCL_clamp);\
    printf("  RCL_cosInt: %d\n",profile_RCL_cosInt);\
    printf("  RCL_angleToDirection: %d\n",profile_RCL_angleToDirection);\
    printf("  RCL_dist: %d\n",profile_RCL_dist);\
    printf("  RCL_len: %d\n",profile_RCL_len);\
    printf("  RCL_pointIsLeftOfRay: %d\n",profile_RCL_pointIsLeftOfRay);\
    printf("  RCL_castRayMultiHit : %d\n",profile_RCL_castRayMultiHit);\
    printf("  RCL_castRay: %d\n",profile_RCL_castRay);\
    printf("  RCL_normalize: %d\n",profile_RCL_normalize);\
    printf("  RCL_vectorsAngleCos: %d\n",profile_RCL_vectorsAngleCos);\
    printf("  RCL_absVal: %d\n",profile_RCL_absVal);\
    printf("  RCL_perspectiveScale: %d\n",profile_RCL_perspectiveScale);\
    printf("  RCL_wrap: %d\n",profile_RCL_wrap);\
    printf("  RCL_divRoundDown: %d\n",profile_RCL_divRoundDown); }
#else
  #define RCL_profileCall(c)
#endif

RCL_Unit RCL_clamp(RCL_Unit value, RCL_Unit valueMin, RCL_Unit valueMax)
{
  RCL_profileCall(RCL_clamp);

  if (value >= valueMin)
  {
    if (value <= valueMax)
      return value;
    else
      return valueMax;
  }
  else
    return valueMin;
}

static inline RCL_Unit RCL_absVal(RCL_Unit value)
{
  RCL_profileCall(RCL_absVal);

  return value >= 0 ? value : -1 * value;
}

/// Like mod, but behaves differently for negative values.
static inline RCL_Unit RCL_wrap(RCL_Unit value, RCL_Unit mod)
{
  RCL_profileCall(RCL_wrap);

  return value >= 0 ? (value % mod) : (mod + (value % mod) - 1);
}

/// Performs division, rounding down, NOT towards zero.
static inline RCL_Unit RCL_divRoundDown(RCL_Unit value, RCL_Unit divisor)
{
  RCL_profileCall(RCL_divRoundDown);

  return value / divisor - ((value >= 0) ? 0 : 1);
}

// Bhaskara's cosine approximation formula
#define trigHelper(x) (((RCL_Unit) RCL_UNITS_PER_SQUARE) *\
  (RCL_UNITS_PER_SQUARE / 2 * RCL_UNITS_PER_SQUARE / 2 - 4 * (x) * (x)) /\
  (RCL_UNITS_PER_SQUARE / 2 * RCL_UNITS_PER_SQUARE / 2 + (x) * (x)))

#if RCL_USE_COS_LUT == 1

  #ifdef RCL_RAYCAST_TINY
  const RCL_Unit cosLUT[64] =
  {
    16,14,11,6,0,-6,-11,-14,-15,-14,-11,-6,0,6,11,14
  };
  #else
  const RCL_Unit cosLUT[64] =
  {
    1024,1019,1004,979,946,903,851,791,724,649,568,482,391,297,199,100,0,-100,
    -199,-297,-391,-482,-568,-649,-724,-791,-851,-903,-946,-979,-1004,-1019,
    -1023,-1019,-1004,-979,-946,-903,-851,-791,-724,-649,-568,-482,-391,-297,
    -199,-100,0,100,199,297,391,482,568,649,724,791,851,903,946,979,1004,1019
  };
  #endif

#elif RCL_USE_COS_LUT == 2
const RCL_Unit cosLUT[128] =
{
  1024,1022,1019,1012,1004,993,979,964,946,925,903,878,851,822,791,758,724,
  687,649,609,568,526,482,437,391,344,297,248,199,150,100,50,0,-50,-100,-150,
  -199,-248,-297,-344,-391,-437,-482,-526,-568,-609,-649,-687,-724,-758,-791,
  -822,-851,-878,-903,-925,-946,-964,-979,-993,-1004,-1012,-1019,-1022,-1023,
  -1022,-1019,-1012,-1004,-993,-979,-964,-946,-925,-903,-878,-851,-822,-791,
  -758,-724,-687,-649,-609,-568,-526,-482,-437,-391,-344,-297,-248,-199,-150,
  -100,-50,0,50,100,150,199,248,297,344,391,437,482,526,568,609,649,687,724,
  758,791,822,851,878,903,925,946,964,979,993,1004,1012,1019,1022
};
#endif

RCL_Unit RCL_cosInt(RCL_Unit input)
{
  RCL_profileCall(RCL_cosInt);

  input = RCL_wrap(input,RCL_UNITS_PER_SQUARE);

#if RCL_USE_COS_LUT == 1

  #ifdef RCL_RAYCAST_TINY
    return cosLUT[input];
  #else
    return cosLUT[input / 16];
  #endif

#elif RCL_USE_COS_LUT == 2
  return cosLUT[input / 8];
#else
  if (input < RCL_UNITS_PER_SQUARE / 4)
    return trigHelper(input);
  else if (input < RCL_UNITS_PER_SQUARE / 2)
    return -1 * trigHelper(RCL_UNITS_PER_SQUARE / 2 - input);
  else if (input < 3 * RCL_UNITS_PER_SQUARE / 4)
    return -1 * trigHelper(input - RCL_UNITS_PER_SQUARE / 2);
  else
    return trigHelper(RCL_UNITS_PER_SQUARE - input);
#endif
}

#undef trigHelper

RCL_Unit RCL_sinInt(RCL_Unit input)
{
  return RCL_cosInt(input - RCL_UNITS_PER_SQUARE / 4);
}

RCL_Vector2D RCL_angleToDirection(RCL_Unit angle)
{
  RCL_profileCall(RCL_angleToDirection);

  RCL_Vector2D result;

  result.x = RCL_cosInt(angle);
  result.y = -1 * RCL_sinInt(angle);

  return result;
}

uint16_t RCL_sqrtInt(RCL_Unit value)
{
  RCL_profileCall(RCL_sqrtInt);

#ifdef RCL_RAYCAST_TINY
  uint16_t result = 0;
  uint16_t a = value;
  uint16_t b = 1u << 14;
#else
  uint32_t result = 0;
  uint32_t a = value;
  uint32_t b = 1u << 30;
#endif

  while (b > a)
    b >>= 2;

  while (b != 0)
  {
    if (a >= result + b)
    {
      a -= result + b;
      result = result +  2 * b;
    }

    b >>= 2;
    result >>= 1;
  }

  return result;
}

RCL_Unit RCL_dist(RCL_Vector2D p1, RCL_Vector2D p2)
{
  RCL_profileCall(RCL_dist);

  RCL_Unit dx = p2.x - p1.x;
  RCL_Unit dy = p2.y - p1.y;

#if RCL_USE_DIST_APPROX == 2
  // octagonal approximation

  dx = RCL_absVal(dx);
  dy = RCL_absVal(dy);

  return dy > dx ? dx / 2 + dy : dy / 2 + dx;
#elif RCL_USE_DIST_APPROX == 1
  // more accurate approximation

  RCL_Unit a, b, result;

  dx = dx < 0 ? -1 * dx : dx;
  dy = dy < 0 ? -1 * dy : dy;

  if (dx < dy)
  {
     a = dy;
     b = dx;
  }
  else
  {
     a = dx;
     b = dy;
  }

  result = a + (44 * b) / 102;

  if (a < (b << 4))
    result -= (5 * a) / 128;

  return result;
#else
  dx = dx * dx;
  dy = dy * dy;

  return RCL_sqrtInt((RCL_Unit) (dx + dy));
#endif
}

RCL_Unit RCL_len(RCL_Vector2D v)
{
  RCL_profileCall(RCL_len);

  RCL_Vector2D zero;
  zero.x = 0;
  zero.y = 0;

  return RCL_dist(zero,v);
}

static inline int8_t RCL_pointIsLeftOfRay(RCL_Vector2D point, RCL_Ray ray)
{
  RCL_profileCall(RCL_pointIsLeftOfRay);

  RCL_Unit dX    = point.x - ray.start.x;
  RCL_Unit dY    = point.y - ray.start.y;
  return (ray.direction.x * dY - ray.direction.y * dX) > 0;
         // ^ Z component of cross-product
}

void RCL_castRayMultiHit(RCL_Ray ray, RCL_ArrayFunction arrayFunc,
  RCL_ArrayFunction typeFunc, RCL_HitResult *hitResults,
  uint16_t *hitResultsLen, RCL_RayConstraints constraints)
{
  RCL_profileCall(RCL_castRayMultiHit);

  RCL_Vector2D currentPos = ray.start;
  RCL_Vector2D currentSquare;

  currentSquare.x = RCL_divRoundDown(ray.start.x,RCL_UNITS_PER_SQUARE);
  currentSquare.y = RCL_divRoundDown(ray.start.y,RCL_UNITS_PER_SQUARE);

  *hitResultsLen = 0;

  RCL_Unit squareType = arrayFunc(currentSquare.x,currentSquare.y);

  // DDA variables
  RCL_Vector2D nextSideDist; // dist. from start to the next side in given axis
  RCL_Vector2D delta;
  RCL_Vector2D step;         // -1 or 1 for each axis
  int8_t stepHorizontal = 0; // whether the last step was hor. or vert.

  nextSideDist.x = 0;
  nextSideDist.y = 0;

  RCL_Unit dirVecLengthNorm = RCL_len(ray.direction) * RCL_UNITS_PER_SQUARE;

  delta.x = RCL_absVal(dirVecLengthNorm / RCL_nonZero(ray.direction.x));
  delta.y = RCL_absVal(dirVecLengthNorm / RCL_nonZero(ray.direction.y));

  // init DDA

  if (ray.direction.x < 0)
  {
    step.x = -1;
    nextSideDist.x = (RCL_wrap(ray.start.x,RCL_UNITS_PER_SQUARE) * delta.x) /
                       RCL_UNITS_PER_SQUARE;
  }
  else
  {
    step.x = 1;
    nextSideDist.x =
      ((RCL_wrap(RCL_UNITS_PER_SQUARE - ray.start.x,RCL_UNITS_PER_SQUARE)) *
        delta.x) / RCL_UNITS_PER_SQUARE;
  }

  if (ray.direction.y < 0)
  {
    step.y = -1;
    nextSideDist.y = (RCL_wrap(ray.start.y,RCL_UNITS_PER_SQUARE) * delta.y) /
                       RCL_UNITS_PER_SQUARE;
  }
  else
  {
    step.y = 1;
    nextSideDist.y =
      ((RCL_wrap(RCL_UNITS_PER_SQUARE - ray.start.y,RCL_UNITS_PER_SQUARE)) *
        delta.y) / RCL_UNITS_PER_SQUARE;
  }

  // DDA loop

  for (uint16_t i = 0; i < constraints.maxSteps; ++i)
  {
    RCL_Unit currentType = arrayFunc(currentSquare.x,currentSquare.y);

    if (currentType != squareType)
    {
      // collision

      RCL_HitResult h;

      h.arrayValue = currentType;
      h.doorRoll = 0;
      h.position = currentPos;
      h.square   = currentSquare;

      if (stepHorizontal)
      {
        h.position.x = currentSquare.x * RCL_UNITS_PER_SQUARE;
        h.direction = 3;

        if (step.x == -1)
        {
          h.direction = 1;
          h.position.x += RCL_UNITS_PER_SQUARE;
        }

        RCL_Unit diff = h.position.x - ray.start.x;
        h.position.y = ray.start.y + ((ray.direction.y * diff) /
          RCL_nonZero(ray.direction.x)); 

#if RCL_RECTILINEAR
        /* Here we compute the fish eye corrected distance (perpendicular to
        the projection plane) as the Euclidean distance divided by the length
        of the ray direction vector. This can be computed without actually
        computing Euclidean distances as a hypothenuse A (distance) divided
        by hypothenuse B (length) is equal to leg A (distance along one axis)
        divided by leg B (length along the same axis). */

        h.distance =
         ((h.position.x - ray.start.x) * RCL_UNITS_PER_SQUARE) /
         RCL_nonZero(ray.direction.x);
#endif
      }
      else
      {
        h.position.y = currentSquare.y * RCL_UNITS_PER_SQUARE;
        h.direction = 2;

        if (step.y == -1)
        {
          h.direction = 0;
          h.position.y += RCL_UNITS_PER_SQUARE;
        }

        RCL_Unit diff = h.position.y - ray.start.y;
        h.position.x = ray.start.x + ((ray.direction.x * diff) /
          RCL_nonZero(ray.direction.y)); 

#if RCL_RECTILINEAR
        h.distance =
         ((h.position.y - ray.start.y) * RCL_UNITS_PER_SQUARE) /
         RCL_nonZero(ray.direction.y);
#endif
      }

#if !RCL_RECTILINEAR
      h.distance = RCL_dist(h.position,ray.start);
#endif

      if (typeFunc != 0)
        h.type = typeFunc(currentSquare.x,currentSquare.y);

#if RCL_COMPUTE_WALL_TEXCOORDS == 1
      switch (h.direction)
      {
        case 0: h.textureCoord =
          RCL_wrap(-1 * h.position.x,RCL_UNITS_PER_SQUARE); break;

        case 1: h.textureCoord =
          RCL_wrap(h.position.y,RCL_UNITS_PER_SQUARE); break;

        case 2: h.textureCoord =
          RCL_wrap(h.position.x,RCL_UNITS_PER_SQUARE); break;

        case 3: h.textureCoord =
          RCL_wrap(-1 * h.position.y,RCL_UNITS_PER_SQUARE); break;

        default: h.textureCoord = 0; break;
      }

      if (_RCL_rollFunction != 0)
      {
        h.doorRoll = _RCL_rollFunction(currentSquare.x,currentSquare.y);
        
        if (h.direction == 0 || h.direction == 1)
          h.doorRoll *= -1;
      }

#else
      h.textureCoord = 0;
#endif

      hitResults[*hitResultsLen] = h;

      *hitResultsLen += 1;

      squareType = currentType;

      if (*hitResultsLen >= constraints.maxHits)
        break;
    }

    // DDA step

    if (nextSideDist.x < nextSideDist.y)
    {
      nextSideDist.x += delta.x;
      currentSquare.x += step.x;
      stepHorizontal = 1;
    }
    else
    {
      nextSideDist.y += delta.y;
      currentSquare.y += step.y;
      stepHorizontal = 0;
    }
  }
}

RCL_HitResult RCL_castRay(RCL_Ray ray, RCL_ArrayFunction arrayFunc)
{
  RCL_profileCall(RCL_castRay);

  RCL_HitResult result;
  uint16_t RCL_len;
  RCL_RayConstraints c;

  c.maxSteps = 1000;
  c.maxHits = 1;

  RCL_castRayMultiHit(ray,arrayFunc,0,&result,&RCL_len,c);

  if (RCL_len == 0)
    result.distance = -1;

  return result;
}

void RCL_castRaysMultiHit(RCL_Camera cam, RCL_ArrayFunction arrayFunc,
  RCL_ArrayFunction typeFunction, RCL_ColumnFunction columnFunc,
  RCL_RayConstraints constraints)
{
  RCL_Vector2D dir1 =
    RCL_angleToDirection(cam.direction - RCL_HORIZONTAL_FOV_HALF);

  RCL_Vector2D dir2 =
    RCL_angleToDirection(cam.direction + RCL_HORIZONTAL_FOV_HALF);

  RCL_Unit dX = dir2.x - dir1.x;
  RCL_Unit dY = dir2.y - dir1.y;

  RCL_HitResult hits[constraints.maxHits];
  uint16_t hitCount;

  RCL_Ray r;
  r.start = cam.position;

  RCL_Unit currentDX = 0;
  RCL_Unit currentDY = 0;

  for (int16_t i = 0; i < cam.resolution.x; ++i)
  {
    /* Here by linearly interpolating the direction vector its length changes,
    which in result achieves correcting the fish eye effect (computing
    perpendicular distance). */

    r.direction.x = dir1.x + currentDX / cam.resolution.x;
    r.direction.y = dir1.y + currentDY / cam.resolution.x;

    RCL_castRayMultiHit(r,arrayFunc,typeFunction,hits,&hitCount,constraints);

    columnFunc(hits,hitCount,i,r);

    currentDX += dX;
    currentDY += dY;
  }
}

/**
  Helper function that determines intersection with both ceiling and floor.
*/
RCL_Unit _RCL_floorCeilFunction(int16_t x, int16_t y)
{
  RCL_Unit f = _RCL_floorFunction(x,y);

  if (_RCL_ceilFunction == 0)
    return f;

  RCL_Unit c = _RCL_ceilFunction(x,y);

#ifndef RCL_RAYCAST_TINY
  return ((f & 0x0000ffff) << 16) | (c & 0x0000ffff);
#else
  return ((f & 0x00ff) << 8) | (c & 0x00ff);
#endif
}

RCL_Unit _floorHeightNotZeroFunction(int16_t x, int16_t y)
{
  return _RCL_floorFunction(x,y) == 0 ? 0 :
    RCL_nonZero((x & 0x00FF) | ((y & 0x00FF) << 8));
    // ^ this makes collisions between all squares - needed for rolling doors
}

RCL_Unit RCL_adjustDistance(RCL_Unit distance, RCL_Camera *camera,
  RCL_Ray *ray)
{
  /* FIXME/TODO: The adjusted (=orthogonal, camera-space) distance could
     possibly be computed more efficiently by not computing Euclidean
     distance at all, but rather compute the distance of the collision
     point from the projection plane (line). */

  RCL_Unit result =
    (distance *
     RCL_vectorsAngleCos(RCL_angleToDirection(camera->direction),
     ray->direction)) / RCL_UNITS_PER_SQUARE;

  return RCL_nonZero(result);
      // ^ prevent division by zero
}

/// Helper for drawing floor or ceiling. Returns the last drawn pixel position.
static inline int16_t _RCL_drawVertical(
  RCL_Unit yCurrent,
  RCL_Unit yTo,
  RCL_Unit limit1, // TODO: int16_t?
  RCL_Unit limit2,
  RCL_Unit verticalOffset,
  int16_t increment,
  int8_t computeDepth,
  int8_t computeCoords,
  int16_t depthIncrementMultiplier,
  RCL_Ray *ray,
  RCL_PixelInfo *pixelInfo
)
{
  _RCL_UNUSED(ray);

  RCL_Unit depthIncrement;
  RCL_Unit dx;
  RCL_Unit dy;

  pixelInfo->isWall = 0;

  int16_t limit = RCL_clamp(yTo,limit1,limit2);

  /* for performance reasons have different version of the critical loop
     to be able to branch early */
  #define loop(doDepth,doCoords)\
  {\
    if (doDepth) /*constant condition - compiler should optimize it out*/\
    {\
      pixelInfo->depth += RCL_absVal(verticalOffset) *\
        RCL_VERTICAL_DEPTH_MULTIPLY;\
      depthIncrement = depthIncrementMultiplier *\
        _RCL_horizontalDepthStep;\
    }\
    if (doCoords) /*constant condition - compiler should optimize it out*/\
    {\
      dx = pixelInfo->hit.position.x - _RCL_camera.position.x;\
      dy = pixelInfo->hit.position.y - _RCL_camera.position.y;\
    }\
    for (int16_t i = yCurrent + increment;\
         increment == -1 ? i >= limit : i <= limit; /* TODO: is efficient? */\
         i += increment)\
    {\
      pixelInfo->position.y = i;\
      if (doDepth)  /*constant condition - compiler should optimize it out*/\
        pixelInfo->depth += depthIncrement;\
      if (doCoords) /*constant condition - compiler should optimize it out*/\
      {\
        RCL_Unit d = _RCL_floorPixelDistances[i];\
        RCL_Unit d2 = RCL_nonZero(pixelInfo->hit.distance);\
        pixelInfo->texCoords.x =\
          _RCL_camera.position.x + ((d * dx) / d2);\
        pixelInfo->texCoords.y =\
          _RCL_camera.position.y + ((d * dy) / d2);\
      }\
      RCL_PIXEL_FUNCTION(pixelInfo);\
    }\
  }

  if (computeDepth) // branch early
  {
    if (!computeCoords)
      loop(1,0)
    else
      loop(1,1)
  }
  else
  {
    if (!computeCoords)
      loop(0,0)
    else
      loop(1,1)
  }

  #undef loop

  return limit;
}

/// Helper for drawing walls. Returns the last drawn pixel position.
static inline int16_t _RCL_drawWall(
  RCL_Unit yCurrent,
  RCL_Unit yFrom,
  RCL_Unit yTo,
  RCL_Unit limit1, // TODO: int16_t?
  RCL_Unit limit2,
  RCL_Unit height,
  int16_t increment,
  RCL_PixelInfo *pixelInfo
  )
{
  _RCL_UNUSED(height)

  height = RCL_absVal(height);

  pixelInfo->isWall = 1;

  RCL_Unit limit = RCL_clamp(yTo,limit1,limit2);

  RCL_Unit wallLength = RCL_nonZero(RCL_absVal(yTo - yFrom - 1));

  RCL_Unit wallPosition = RCL_absVal(yFrom - yCurrent) - increment;

  RCL_Unit heightScaled = height * RCL_TEXTURE_INTERPOLATION_SCALE;
  _RCL_UNUSED(heightScaled);

  RCL_Unit coordStepScaled = RCL_COMPUTE_WALL_TEXCOORDS ?
#if RCL_TEXTURE_VERTICAL_STRETCH == 1
    ((RCL_UNITS_PER_SQUARE * RCL_TEXTURE_INTERPOLATION_SCALE) / wallLength)
#else
    (heightScaled / wallLength)
#endif
    : 0;

  pixelInfo->texCoords.y = RCL_COMPUTE_WALL_TEXCOORDS ?
    (wallPosition * coordStepScaled) : 0;

  if (increment < 0)
  {
    coordStepScaled *= -1;
    pixelInfo->texCoords.y =
#if RCL_TEXTURE_VERTICAL_STRETCH == 1
      (RCL_UNITS_PER_SQUARE * RCL_TEXTURE_INTERPOLATION_SCALE)
      - pixelInfo->texCoords.y;
#else
      heightScaled - pixelInfo->texCoords.y;
#endif
  }

  RCL_Unit textureCoordScaled = pixelInfo->texCoords.y;

  for (RCL_Unit i = yCurrent + increment; 
       increment == -1 ? i >= limit : i <= limit; // TODO: is efficient?
       i += increment)
  {
    pixelInfo->position.y = i;

#if RCL_COMPUTE_WALL_TEXCOORDS == 1
    pixelInfo->texCoords.y =
      textureCoordScaled / RCL_TEXTURE_INTERPOLATION_SCALE;

    textureCoordScaled += coordStepScaled;
#endif

    RCL_PIXEL_FUNCTION(pixelInfo);
  }
  
  return limit;
}

/// Fills a RCL_HitResult struct with info for a hit at infinity.
static inline void _RCL_makeInfiniteHit(RCL_HitResult *hit, RCL_Ray *ray)
{
  hit->distance = RCL_UNITS_PER_SQUARE * RCL_UNITS_PER_SQUARE;
  /* ^ horizon is at infinity, but we can't use too big infinity
       (RCL_INFINITY) because it would overflow in the following mult. */
  hit->position.x = (ray->direction.x * hit->distance) / RCL_UNITS_PER_SQUARE;
  hit->position.y = (ray->direction.y * hit->distance) / RCL_UNITS_PER_SQUARE;

  hit->direction = 0;
  hit->textureCoord = 0;
  hit->arrayValue = 0;
  hit->doorRoll = 0;
  hit->type = 0;
}

void _RCL_columnFunctionComplex(RCL_HitResult *hits, uint16_t hitCount, uint16_t x,
  RCL_Ray ray)
{
  // last written Y position, can never go backwards
  RCL_Unit fPosY = _RCL_camera.resolution.y;
  RCL_Unit cPosY = -1;

  // world coordinates (relative to camera height though)
  RCL_Unit fZ1World = _RCL_startFloorHeight;
  RCL_Unit cZ1World = _RCL_startCeil_Height;

  RCL_PixelInfo p;
  p.position.x = x;
  p.height = 0;
  p.wallHeight = 0;
  p.texCoords.x = 0;
  p.texCoords.y = 0;

  // we'll be simulatenously drawing the floor and the ceiling now  
  for (RCL_Unit j = 0; j <= hitCount; ++j)
  {                    // ^ = add extra iteration for horizon plane
    int8_t drawingHorizon = j == hitCount;

    RCL_HitResult hit;
    RCL_Unit distance = 1;

    RCL_Unit fWallHeight = 0, cWallHeight = 0;
    RCL_Unit fZ2World = 0,    cZ2World = 0;
    RCL_Unit fZ1Screen = 0,   cZ1Screen = 0;
    RCL_Unit fZ2Screen = 0,   cZ2Screen = 0;

    if (!drawingHorizon)
    {
      hit = hits[j];
      distance = RCL_nonZero(hit.distance); 
      p.hit = hit;

      fWallHeight = _RCL_floorFunction(hit.square.x,hit.square.y);
      fZ2World = fWallHeight - _RCL_camera.height;
      fZ1Screen = _RCL_middleRow - RCL_perspectiveScale(
        (fZ1World * _RCL_camera.resolution.y) /
        RCL_UNITS_PER_SQUARE,distance);
      fZ2Screen = _RCL_middleRow - RCL_perspectiveScale(
        (fZ2World * _RCL_camera.resolution.y) /
        RCL_UNITS_PER_SQUARE,distance);

      if (_RCL_ceilFunction != 0)
      {
        cWallHeight = _RCL_ceilFunction(hit.square.x,hit.square.y);
        cZ2World = cWallHeight - _RCL_camera.height;
        cZ1Screen = _RCL_middleRow - RCL_perspectiveScale(
          (cZ1World * _RCL_camera.resolution.y) /
          RCL_UNITS_PER_SQUARE,distance);
        cZ2Screen = _RCL_middleRow - RCL_perspectiveScale(
          (cZ2World * _RCL_camera.resolution.y) /
          RCL_UNITS_PER_SQUARE,distance);
      }
    }
    else
    {
      fZ1Screen = _RCL_middleRow;
      cZ1Screen = _RCL_middleRow + 1;
      _RCL_makeInfiniteHit(&p.hit,&ray);
    }

    RCL_Unit limit;

    p.isWall = 0;
    p.isHorizon = drawingHorizon;

    // draw floor until wall
    p.isFloor = 1;
    p.height = fZ1World + _RCL_camera.height;
    p.wallHeight = 0;

#if RCL_COMPUTE_FLOOR_DEPTH == 1
    p.depth = (_RCL_fHorizontalDepthStart - fPosY) * _RCL_horizontalDepthStep;
#else
    p.depth = 0;
#endif

    limit = _RCL_drawVertical(fPosY,fZ1Screen,cPosY + 1,
     _RCL_camera.resolution.y,fZ1World,-1,RCL_COMPUTE_FLOOR_DEPTH,
     // ^ purposfully allow outside screen bounds
       RCL_COMPUTE_FLOOR_TEXCOORDS && p.height == RCL_FLOOR_TEXCOORDS_HEIGHT,
       1,&ray,&p);

    if (fPosY > limit)
      fPosY = limit;

    if (_RCL_ceilFunction != 0 || drawingHorizon)
    {
      // draw ceiling until wall
      p.isFloor = 0;
      p.height = cZ1World + _RCL_camera.height;

#if RCL_COMPUTE_CEILING_DEPTH == 1
      p.depth = (cPosY - _RCL_cHorizontalDepthStart) *
        _RCL_horizontalDepthStep;
#endif

      limit = _RCL_drawVertical(cPosY,cZ1Screen,
        -1,fPosY - 1,cZ1World,1,RCL_COMPUTE_CEILING_DEPTH,0,1,&ray,&p);
      // ^ purposfully allow outside screen bounds here

      if (cPosY < limit)
        cPosY = limit;
    }

    if (!drawingHorizon) // don't draw walls for horizon plane
    {
      p.isWall = 1;
      p.depth = distance;
      p.isFloor = 1;
      p.texCoords.x = hit.textureCoord;
      p.height = fZ1World + _RCL_camera.height;
      p.wallHeight = fWallHeight;

      // draw floor wall

      if (fPosY > 0)  // still pixels left?
      {
        p.isFloor = 1;

        limit = _RCL_drawWall(fPosY,fZ1Screen,fZ2Screen,cPosY + 1,
                  _RCL_camera.resolution.y,
                  // ^ purposfully allow outside screen bounds here
#if RCL_TEXTURE_VERTICAL_STRETCH == 1
                  RCL_UNITS_PER_SQUARE
#else
                  fZ2World - fZ1World
#endif
                  ,-1,&p);
                

        if (fPosY > limit)
          fPosY = limit;

        fZ1World = fZ2World; // for the next iteration
      }               // ^ purposfully allow outside screen bounds here

      // draw ceiling wall

      if (_RCL_ceilFunction != 0 && cPosY < _RCL_camResYLimit) // pixels left?
      {
        p.isFloor = 0;
        p.height = cZ1World + _RCL_camera.height;
        p.wallHeight = cWallHeight;

        limit = _RCL_drawWall(cPosY,cZ1Screen,cZ2Screen,
                  -1,fPosY - 1,
                // ^ puposfully allow outside screen bounds here
#if RCL_TEXTURE_VERTICAL_STRETCH == 1
                  RCL_UNITS_PER_SQUARE
#else
                  cZ1World - cZ2World 
#endif
                  ,1,&p);
                
        if (cPosY < limit)
          cPosY = limit;

        cZ1World = cZ2World; // for the next iteration
      }              // ^ puposfully allow outside screen bounds here 
    }
  }
}

void _RCL_columnFunctionSimple(RCL_HitResult *hits, uint16_t hitCount,
  uint16_t x, RCL_Ray ray)
{
  RCL_Unit y = 0;
  RCL_Unit wallHeightScreen = 0;
  RCL_Unit wallStart = _RCL_middleRow;
  RCL_Unit heightOffset = 0;

  RCL_Unit dist = 1;

  RCL_PixelInfo p;
  p.position.x = x;
  p.wallHeight = RCL_UNITS_PER_SQUARE;

  if (hitCount > 0)
  {
    RCL_HitResult hit = hits[0];

    uint8_t goOn = 1;

    if (_RCL_rollFunction != 0 && RCL_COMPUTE_WALL_TEXCOORDS == 1)
    {
      if (hit.arrayValue == 0)
      {
        // standing inside door square, looking out => move to the next hit

        if (hitCount > 1)
          hit = hits[1];
        else
          goOn = 0;
      }
      else
      {
        // normal hit, check the door roll

        RCL_Unit texCoordMod = hit.textureCoord % RCL_UNITS_PER_SQUARE;

        int8_t unrolled = hit.doorRoll >= 0 ?
          (hit.doorRoll > texCoordMod) :
          (texCoordMod > RCL_UNITS_PER_SQUARE + hit.doorRoll);

        if (unrolled)
        {
          goOn = 0;

          if (hitCount > 1) /* should probably always be true (hit on square
                               exit) */
          {
            if (hit.direction % 2 != hits[1].direction % 2)
            {
              // hit on the inner side
              hit = hits[1];
              goOn = 1;
            }
            else if (hitCount > 2)
            {
              // hit on the opposite side
              hit = hits[2];
              goOn = 1;
            }
          }
        }
      }
    }

    p.hit = hit;

    if (goOn)
    {
      dist = hit.distance;

      int16_t wallHeightWorld = _RCL_floorFunction(hit.square.x,hit.square.y);

      wallHeightScreen = RCL_perspectiveScale((wallHeightWorld *
        _RCL_camera.resolution.y) / RCL_UNITS_PER_SQUARE,dist);

      int16_t RCL_normalizedWallHeight = wallHeightWorld != 0 ?
        ((RCL_UNITS_PER_SQUARE * wallHeightScreen) / wallHeightWorld) : 0;

      heightOffset = RCL_perspectiveScale(_RCL_cameraHeightScreen,dist);

      wallStart = _RCL_middleRow - wallHeightScreen + heightOffset +
                  RCL_normalizedWallHeight;
    }
  }
  else
  {
    _RCL_makeInfiniteHit(&p.hit,&ray);
  }

  // draw ceiling

  p.isWall = 0;
  p.isFloor = 0;
  p.isHorizon = 1;
  p.depth = 1;
  p.height = RCL_UNITS_PER_SQUARE;

  y = _RCL_drawVertical(-1,wallStart,-1,_RCL_middleRow,_RCL_camera.height,1,
    RCL_COMPUTE_CEILING_DEPTH,0,1,&ray,&p);

  // draw wall

  p.isWall = 1;
  p.isFloor = 1;
  p.depth = dist;
  p.height = 0;

#if RCL_ROLL_TEXTURE_COORDS == 1 && RCL_COMPUTE_WALL_TEXCOORDS == 1 
  p.hit.textureCoord -= p.hit.doorRoll;
#endif

  p.texCoords.x = p.hit.textureCoord;
  p.texCoords.y = 0;

  RCL_Unit limit = _RCL_drawWall(y,wallStart,wallStart + wallHeightScreen - 1,
    -1,_RCL_camResYLimit,p.hit.arrayValue,1,&p);

  y = RCL_max(y,limit); // take max, in case no wall was drawn
  y = RCL_max(y,wallStart);

  // draw floor

  p.isWall = 0;

#if RCL_COMPUTE_FLOOR_DEPTH == 1
  p.depth = (_RCL_camera.resolution.y - y) * _RCL_horizontalDepthStep + 1;
#endif

  _RCL_drawVertical(y,_RCL_camResYLimit,-1,_RCL_camResYLimit,
    _RCL_camera.height,1,RCL_COMPUTE_FLOOR_DEPTH,RCL_COMPUTE_FLOOR_TEXCOORDS,
    -1,&ray,&p);
}

/**
  Precomputes a distance from camera to the floor at each screen row into an
  array (must be preallocated with sufficient (camera.resolution.y) length).
*/
static inline void _RCL_precomputeFloorDistances(RCL_Camera camera,
  RCL_Unit *dest, uint16_t startIndex)
{
  RCL_Unit camHeightScreenSize =
    (camera.height * camera.resolution.y) / RCL_UNITS_PER_SQUARE;

  for (uint16_t i = startIndex
; i < camera.resolution.y; ++i)
    dest[i] = RCL_perspectiveScaleInverse(camHeightScreenSize,
             RCL_absVal(i - _RCL_middleRow));
}

void RCL_renderComplex(RCL_Camera cam, RCL_ArrayFunction floorHeightFunc,
  RCL_ArrayFunction ceilingHeightFunc, RCL_ArrayFunction typeFunction,
  RCL_RayConstraints constraints)
{
  _RCL_floorFunction = floorHeightFunc;
  _RCL_ceilFunction = ceilingHeightFunc;
  _RCL_camera = cam;
  _RCL_camResYLimit = cam.resolution.y - 1;

  uint16_t halfResY = cam.resolution.y / 2;

  _RCL_middleRow = halfResY + cam.shear;

  _RCL_fHorizontalDepthStart = _RCL_middleRow + halfResY;
  _RCL_cHorizontalDepthStart = _RCL_middleRow - halfResY;

  _RCL_startFloorHeight = floorHeightFunc(
    RCL_divRoundDown(cam.position.x,RCL_UNITS_PER_SQUARE),
    RCL_divRoundDown(cam.position.y,RCL_UNITS_PER_SQUARE)) -1 * cam.height;

  _RCL_startCeil_Height = 
    ceilingHeightFunc != 0 ?
      ceilingHeightFunc(
        RCL_divRoundDown(cam.position.x,RCL_UNITS_PER_SQUARE),
        RCL_divRoundDown(cam.position.y,RCL_UNITS_PER_SQUARE)) -1 * cam.height
      : RCL_INFINITY;

  _RCL_horizontalDepthStep = RCL_HORIZON_DEPTH / cam.resolution.y; 

#if RCL_COMPUTE_FLOOR_TEXCOORDS == 1
  RCL_Unit floorPixelDistances[cam.resolution.y];
  _RCL_precomputeFloorDistances(cam,floorPixelDistances,0);
  _RCL_floorPixelDistances = floorPixelDistances; // pass to column function
#endif

  RCL_castRaysMultiHit(cam,_RCL_floorCeilFunction,typeFunction,
    _RCL_columnFunctionComplex,constraints);
}

void RCL_renderSimple(RCL_Camera cam, RCL_ArrayFunction floorHeightFunc,
  RCL_ArrayFunction typeFunc, RCL_ArrayFunction rollFunc,
  RCL_RayConstraints constraints)
{
  _RCL_floorFunction = floorHeightFunc;
  _RCL_camera = cam;
  _RCL_camResYLimit = cam.resolution.y - 1;
  _RCL_middleRow = cam.resolution.y / 2;
  _RCL_rollFunction = rollFunc;

  _RCL_cameraHeightScreen =
    (_RCL_camera.resolution.y * (_RCL_camera.height - RCL_UNITS_PER_SQUARE)) /
    RCL_UNITS_PER_SQUARE;

  _RCL_horizontalDepthStep = RCL_HORIZON_DEPTH / cam.resolution.y; 

  constraints.maxHits = 
    _RCL_rollFunction == 0 ?
      1 : // no door => 1 hit is enough 
      3;  // for correctly rendering rolling doors we'll need 3 hits (NOT 2)

#if RCL_COMPUTE_FLOOR_TEXCOORDS == 1
  RCL_Unit floorPixelDistances[cam.resolution.y];
  _RCL_precomputeFloorDistances(cam,floorPixelDistances,_RCL_middleRow);
  _RCL_floorPixelDistances = floorPixelDistances; // pass to column function
#endif

  RCL_castRaysMultiHit(cam,_floorHeightNotZeroFunction,typeFunc,
    _RCL_columnFunctionSimple, constraints);

#if RCL_COMPUTE_FLOOR_TEXCOORDS == 1
  _RCL_floorPixelDistances = 0;
#endif
}

RCL_Vector2D RCL_normalize(RCL_Vector2D v)
{
  RCL_profileCall(RCL_normalize);

  RCL_Vector2D result;
  RCL_Unit l = RCL_len(v);
  l = RCL_nonZero(l);

  result.x = (v.x * RCL_UNITS_PER_SQUARE) / l;
  result.y = (v.y * RCL_UNITS_PER_SQUARE) / l;

  return result;
}

RCL_Unit RCL_vectorsAngleCos(RCL_Vector2D v1, RCL_Vector2D v2)
{
  RCL_profileCall(RCL_vectorsAngleCos);

  v1 = RCL_normalize(v1);
  v2 = RCL_normalize(v2);

  return (v1.x * v2.x + v1.y * v2.y) / RCL_UNITS_PER_SQUARE;
}

RCL_PixelInfo RCL_mapToScreen(RCL_Vector2D worldPosition, RCL_Unit height,
  RCL_Camera camera)
{
  RCL_PixelInfo result;

  RCL_Unit d = RCL_dist(worldPosition,camera.position);

  RCL_Vector2D toPoint;

  toPoint.x = worldPosition.x - camera.position.x;
  toPoint.y = worldPosition.y - camera.position.y;

  RCL_Unit middleColumn = camera.resolution.x / 2;

  // rotate the point

  RCL_Unit cos = RCL_cosInt(camera.direction);
  RCL_Unit sin = RCL_sinInt(camera.direction);

  RCL_Unit tmp = toPoint.x;

  toPoint.x = (toPoint.x * cos - toPoint.y * sin) / RCL_UNITS_PER_SQUARE; 
  toPoint.y = (tmp * sin + toPoint.y * cos) / RCL_UNITS_PER_SQUARE; 

  result.depth = toPoint.x;

  result.position.x =
    middleColumn + (-1 * toPoint.y * middleColumn) / RCL_nonZero(result.depth);

  result.position.y = camera.resolution.y / 2 -
    (camera.resolution.y *
     RCL_perspectiveScale(height - camera.height,result.depth))
     / RCL_UNITS_PER_SQUARE + camera.shear;

  return result;
}

RCL_Unit RCL_degreesToUnitsAngle(int16_t degrees)
{
  return (degrees * RCL_UNITS_PER_SQUARE) / 360;
}

RCL_Unit RCL_perspectiveScale(RCL_Unit originalSize, RCL_Unit distance)
{
  RCL_profileCall(RCL_perspectiveScale);

  return distance != 0 ?
   (originalSize * RCL_UNITS_PER_SQUARE) /
      ((RCL_VERTICAL_FOV * 2 * distance) / RCL_UNITS_PER_SQUARE)
   : 0;
}

RCL_Unit RCL_perspectiveScaleInverse(RCL_Unit originalSize,
  RCL_Unit scaledSize)
{
  return scaledSize != 0 ?
    (originalSize * RCL_UNITS_PER_SQUARE + RCL_UNITS_PER_SQUARE / 2) /
                                           // ^ take the middle
      ((RCL_VERTICAL_FOV * 2 * scaledSize) / RCL_UNITS_PER_SQUARE)
    : RCL_INFINITY;
}

void RCL_moveCameraWithCollision(RCL_Camera *camera, RCL_Vector2D planeOffset,
  RCL_Unit heightOffset, RCL_ArrayFunction floorHeightFunc,
  RCL_ArrayFunction ceilingHeightFunc, int8_t computeHeight, int8_t force)
{
  int8_t movesInPlane = planeOffset.x != 0 || planeOffset.y != 0;
  int16_t xSquareNew, ySquareNew;

  if (movesInPlane || force)
  {
    RCL_Vector2D corner; // BBox corner in the movement direction
    RCL_Vector2D cornerNew;

    int16_t xDir = planeOffset.x > 0 ? 1 : -1;
    int16_t yDir = planeOffset.y > 0 ? 1 : -1;

    corner.x = camera->position.x + xDir * RCL_CAMERA_COLL_RADIUS;
    corner.y = camera->position.y + yDir * RCL_CAMERA_COLL_RADIUS;

    int16_t xSquare = RCL_divRoundDown(corner.x,RCL_UNITS_PER_SQUARE);
    int16_t ySquare = RCL_divRoundDown(corner.y,RCL_UNITS_PER_SQUARE);

    cornerNew.x = corner.x + planeOffset.x;
    cornerNew.y = corner.y + planeOffset.y;

    xSquareNew = RCL_divRoundDown(cornerNew.x,RCL_UNITS_PER_SQUARE);
    ySquareNew = RCL_divRoundDown(cornerNew.y,RCL_UNITS_PER_SQUARE);

    RCL_Unit bottomLimit = -1 * RCL_INFINITY;
    RCL_Unit topLimit = RCL_INFINITY;

    if (computeHeight)
    {
      bottomLimit = camera->height - RCL_CAMERA_COLL_HEIGHT_BELOW +
        RCL_CAMERA_COLL_STEP_HEIGHT;

      topLimit = camera->height + RCL_CAMERA_COLL_HEIGHT_ABOVE;
    }

    // checks a single square for collision against the camera
    #define collCheck(dir,s1,s2)\
    if (computeHeight)\
    {\
      RCL_Unit height = floorHeightFunc(s1,s2);\
      if (height > bottomLimit)\
        dir##Collides = 1;\
      else if (ceilingHeightFunc != 0)\
      {\
        height = ceilingHeightFunc(s1,s2);\
        if (height < topLimit)\
          dir##Collides = 1;\
      }\
    }\
    else\
      dir##Collides = floorHeightFunc(s1,s2) > RCL_CAMERA_COLL_STEP_HEIGHT;

    // check a collision against non-diagonal square
    #define collCheckOrtho(dir,dir2,s1,s2,x)\
    if (dir##SquareNew != dir##Square)\
    {\
      collCheck(dir,s1,s2)\
    }\
    if (!dir##Collides)\
    { /* now also check for coll on the neighbouring square */ \
      int16_t dir2##Square2 = RCL_divRoundDown(corner.dir2 - dir2##Dir *\
        RCL_CAMERA_COLL_RADIUS * 2,RCL_UNITS_PER_SQUARE);\
      if (dir2##Square2 != dir2##Square)\
      {\
        if (x)\
          collCheck(dir,dir##SquareNew,dir2##Square2)\
        else\
          collCheck(dir,dir2##Square2,dir##SquareNew)\
      }\
    }

    int8_t xCollides = 0;
    collCheckOrtho(x,y,xSquareNew,ySquare,1)

    int8_t yCollides = 0;
    collCheckOrtho(y,x,xSquare,ySquareNew,0)

    #define collHandle(dir)\
    if (dir##Collides)\
      cornerNew.dir = (dir##Square) * RCL_UNITS_PER_SQUARE +\
      RCL_UNITS_PER_SQUARE / 2 + dir##Dir * (RCL_UNITS_PER_SQUARE / 2) -\
      dir##Dir;\

    if (!xCollides && !yCollides) /* if non-diagonal collision happend, corner
                                     collision can't happen */
    {
      if (xSquare != xSquareNew && ySquare != ySquareNew) // corner?
      {
        int8_t xyCollides = 0;
        collCheck(xy,xSquareNew,ySquareNew)
        
        if (xyCollides)
        {
          // normally should slide, but let's KISS
          cornerNew = corner;
        }
      }
    }

    collHandle(x)
    collHandle(y)

    #undef collCheck
    #undef collHandle

    camera->position.x = cornerNew.x - xDir * RCL_CAMERA_COLL_RADIUS;
    camera->position.y = cornerNew.y - yDir * RCL_CAMERA_COLL_RADIUS;
  }

  if (computeHeight && (movesInPlane || heightOffset != 0 || force))
  {
    camera->height += heightOffset;

    int16_t xSquare1 = RCL_divRoundDown(camera->position.x -
      RCL_CAMERA_COLL_RADIUS,RCL_UNITS_PER_SQUARE);

    int16_t xSquare2 = RCL_divRoundDown(camera->position.x +
      RCL_CAMERA_COLL_RADIUS,RCL_UNITS_PER_SQUARE);

    int16_t ySquare1 = RCL_divRoundDown(camera->position.y -
      RCL_CAMERA_COLL_RADIUS,RCL_UNITS_PER_SQUARE);

    int16_t ySquare2 = RCL_divRoundDown(camera->position.y +
      RCL_CAMERA_COLL_RADIUS,RCL_UNITS_PER_SQUARE);

    RCL_Unit bottomLimit = floorHeightFunc(xSquare1,ySquare1);
    RCL_Unit topLimit = ceilingHeightFunc != 0 ?
      ceilingHeightFunc(xSquare1,ySquare1) : RCL_INFINITY;

    RCL_Unit height;

    #define checkSquares(s1,s2)\
    {\
      height = floorHeightFunc(xSquare##s1,ySquare##s2);\
      bottomLimit = RCL_max(bottomLimit,height);\
      height = ceilingHeightFunc != 0 ?\
        ceilingHeightFunc(xSquare##s1,ySquare##s2) : RCL_INFINITY;\
      topLimit = RCL_min(topLimit,height);\
    }

    if (xSquare2 != xSquare1)
      checkSquares(2,1)

    if (ySquare2 != ySquare1)
      checkSquares(1,2)

    if (xSquare2 != xSquare1 && ySquare2 != ySquare1)
      checkSquares(2,2)

    camera->height = RCL_clamp(camera->height,
      bottomLimit + RCL_CAMERA_COLL_HEIGHT_BELOW,
      topLimit - RCL_CAMERA_COLL_HEIGHT_ABOVE);

    #undef checkSquares
  }
}

void RCL_initCamera(RCL_Camera *camera)
{
  camera->position.x = 0;
  camera->position.y = 0;
  camera->direction = 0;
  camera->resolution.x = 20;
  camera->resolution.y = 15;
  camera->shear = 0;
  camera->height = RCL_UNITS_PER_SQUARE;
}

void RCL_initRayConstraints(RCL_RayConstraints *constraints)
{
  constraints->maxHits = 1;
  constraints->maxSteps = 20;
}

#endif
