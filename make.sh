# Optional helper build script for Anarch.
# by drummyfish, released under CC0 1.0, public domain
#
# usage:
#
# ./make.sh platform [compiler]

#!/bin/bash

if [ $# -lt 1 ]; then
  echo "need a parameter (sdl, pokitto, gb, emscripten, ...)"
  exit 0
fi

clear; clear; 

C_FLAGS='-x c -Wall -Wextra -fmax-errors=5 -pedantic -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -o game'

COMPILER='g++'

if [ $# -eq 2 ]; then
  COMPILER=$2
fi

echo "compiling"

if [ $1 == "sdl" ]; then
  # PC SDL build, requires:
  # - g++
  # - SDL2 (dev) package

  COMMAND="${COMPILER} ${C_FLAGS} main_sdl.c -lSDL2"

  echo ${COMMAND}

  ${COMMAND} && ./game
elif [ $1 == "terminal" ]; then
  # PC terminal build, requires:
  # - g++

  COMMAND="${COMPILER} ${C_FLAGS} main_terminal.c"

  echo ${COMMAND}

  ${COMMAND} && ./game
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
elif [ $1 == "emscripten" ]; then
  # emscripten (browser Javascript) build, requires:
  # - emscripten

  emcc ./main_sdl.c -s USE_SDL=2 -O3 -lopenal --shell-file HTMLshell.html -o game.html
else
  echo "unknown parameter: $1"
fi
