# Build script for Anarch.
# by drummyfish, released under CC0 1.0, public domain

#!/bin/bash

if [ $# -ne 1 ]; then
  echo "need a parameter (sdl, pokitto, gb, ...)"
  exit 0
fi

clear; clear; 

if [ $1 == "sdl" ]; then

  # PC SDL build, requires:
  # - SDL2 (dev) package

  g++ -x c -g -Wall -Wextra -fmax-errors=5 -pedantic -O3 -Wall -Wextra \
  -Wno-unused-parameter -Wno-missing-field-initializers -o game main_sdl.c \
  -lSDL2 2>&1 >/dev/null && ./game
elif [ $1 == "pokitto" ]; then

  # Pokitto build, requires:
  # - PokittoLib, in this folder create a symlink named "PokittoLib" to the 
  #   "Pokitto" subfolder of PokittoLib
  # - Pokitto Makefile
  # - GNU embedded toolchain, in this folder create a symlink named "gtc" to the
  #   "bin" subfolder
  # - files like My_settings.h required by Pokitto

  make
  ./PokittoEmu BUILD/firmware.bin 
else
  echo "unknown parameter: $1"
fi
