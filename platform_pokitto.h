#ifndef _SFG_PLATFORM_H
#define _SFG_PLATFORM_H

#include "settings.h"

#undef SFG_FPS
#define SFG_FPS 30

#undef SFG_RESOLUTION_X
#define SFG_RESOLUTION_X 110

#undef SFG_RESOLUTION_Y
#define SFG_RESOLUTION_Y 88

#undef SFG_DITHERED_SHADOW
#define SFG_DITHERED_SHADOW 0

#undef SFG_RAYCASTING_MAX_STEPS
#define SFG_RAYCASTING_MAX_STEPS 20  

#undef SFG_RAYCASTING_MAX_HITS
#define SFG_RAYCASTING_MAX_HITS 6

#include "Pokitto.h"

Pokitto::Core pokitto;

uint8_t *pokittoScreen;

void SFG_setPixel(uint16_t x, uint16_t y, uint8_t colorIndex)
{
  pokittoScreen[y * SFG_RESOLUTION_X + x] = colorIndex;
}

uint32_t SFG_getTimeMs()
{
  return pokitto.getTime();
}

void SFG_sleepMs(uint16_t timeMs)
{
}

int8_t SFG_keyPressed(uint8_t key)
{
  switch (key)
  {
    case SFG_KEY_UP: return pokitto.upBtn(); break;
    case SFG_KEY_RIGHT: return pokitto.rightBtn(); break;
    case SFG_KEY_DOWN: return pokitto.downBtn(); break;
    case SFG_KEY_LEFT: return pokitto.leftBtn(); break;
    case SFG_KEY_A: return pokitto.aBtn(); break;
    case SFG_KEY_B: return pokitto.bBtn(); break;
    case SFG_KEY_C: return pokitto.cBtn(); break;
    default: return 0; break;
  }
}

int main()
{
  pokitto.begin(); 

  pokitto.setFrameRate(SFG_FPS);
  pokitto.display.setFont(fontTiny);
  pokitto.display.persistence = 1;
  pokitto.display.setInvisibleColor(-1);
  pokitto.display.load565Palette(paletteRGB565);

  pokittoScreen = pokitto.display.screenbuffer;

  SFG_init();

  while (pokitto.isRunning())
  {
    if (pokitto.update())
    {
      SFG_mainLoopBody();
    }
  }

  return 0;
}

#endif // guard
