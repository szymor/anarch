#!/bin/sh

# Optional helper build script for Anarch.
# by drummyfish, released under CC0 1.0, public domain
#
# usage:
#
# ./make.sh [frontend [compiler]]

C_FLAGS="-std=c99 -Wall -Wextra -pedantic -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -D _DEFAULT_SOURCE -o anarch"
# note: _DEFAULT_SOURCE shuts up the warning about undeclared usleep

if [ $# -lt 1 ]; then
  FRONTEND="sdl"
else
  FRONTEND="$1"
fi

if [ $# -eq 2 ]; then
  COMPILER="$2"

  if [ $2 = "tcc" ]; then # you'll probably want to modify this
    C_FLAGS="${C_FLAGS} -L/usr/lib/x86_64-linux-gnu/pulseaudio/ 
      -I/home/tastyfish/git/tcc/tcc-0.9.27/include
      -I/usr/lib/gcc/x86_64-linux-gnu/8/include/"
  fi
else
  COMPILER="cc"
fi

echo "compiling"

if [ "$FRONTEND" = "sdl" ]; then
  # PC SDL build, requires:
  # - g++
  # - SDL2 (dev) package

  SDL_FLAGS=`sdl2-config --cflags --libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_sdl.c -I/usr/local/include ${SDL_FLAGS}"
elif [ "$FRONTEND" = "x11" ]; then
  # X11 build

  X11_FLAGS=`pkg-config --cflags --libs x11`
  COMMAND="${COMPILER} ${C_FLAGS} main_x11.c ${X11_FLAGS}"
elif [ "$FRONTEND" = "ncurses" ]; then
  # ncurses build, requires:
  # - libncurses-dev

  NCURSES_FLAGS=`ncurses5-config --cflags --libs` # may need edit
  COMMAND="${COMPILER} ${C_FLAGS} main_ncurses.c ${NCURSES_FLAGS}"
elif [ "$FRONTEND" = "saf" ]; then
  # SAF build using SDL, requires:
  # - saf.h
  # - SDL2 (dev) package

  SDL_FLAGS=`sdl2-config --cflags --libs --static-libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_saf.c -I/usr/local/include ${SDL_FLAGS}"
elif [ "$FRONTEND" = "terminal" ]; then
  # PC terminal build, requires:
  # - g++

  COMMAND="${COMPILER} ${C_FLAGS} main_terminal.c"
elif [ "$FRONTEND" = "csfml" ]; then
  # csfml build, requires:
  # - csfml

  COMMAND="${COMPILER} ${C_FLAGS} main_csfml.c -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio"
elif [ "$FRONTEND" = "test" ]; then
  # test build, requires:
  # - g++

  COMMAND="${COMPILER} ${C_FLAGS} main_test.c"
elif [ "$FRONTEND" = "pokitto" ]; then
  # Pokitto build, requires:
  # - PokittoLib, in this folder create a symlink named "PokittoLib" to the 
  #   "Pokitto" subfolder of PokittoLib
  # - Pokitto Makefile
  # - GNU embedded toolchain, in this folder create a symlink named "gtc" to the
  #   "bin" subfolder
  # - files like My_settings.h required by Pokitto

  COMMAND="make && ./PokittoEmu BUILD/firmware.bin"
elif [ "$FRONTEND" = "emscripten" ]; then
  # emscripten (browser Javascript) build, requires:
  # - emscripten

  COMMAND="emcc ./main_sdl.c -s USE_SDL=2 -O3 -lopenal --shell-file HTMLshell.html -o anarch.html -s EXPORTED_FUNCTIONS='[\"_main\",\"_webButton\",\"_webSetTouchControls\"]' -s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'"
elif [ $1 = "sdl1" ]; then
  # PC SDL 1.2 build, requires:
  # - g++
  # - SDL 1.2 (dev) package

  SDL_FLAGS=`sdl-config --libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_sdl1.c -I/usr/local/include ${SDL_FLAGS}"
elif [ $1 = "miyoo" ]; then
  # SDL 1.2 build for miyoo, requires:
  # - arm-linux-g++
  # - SDL 1.2 (dev) package

  COMPILER="arm-linux-g++"
  SYSROOT=`${COMPILER} --print-sysroot`
  SDL_FLAGS=`${SYSROOT}/usr/bin/sdl-config --libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_sdl1.c ${SDL_FLAGS} -DMIYOO"
elif [ $1 = "retrofw" ]; then
  # SDL 1.2 build for retrofw, requires:
  # - mipsel-linux-g++
  # - SDL 1.2 (dev) package

  COMPILER="mipsel-linux-g++"
  SYSROOT=`${COMPILER} --print-sysroot`
  SDL_FLAGS=`${SYSROOT}/usr/bin/sdl-config --libs`
  COMMAND="${COMPILER} ${C_FLAGS} main_sdl1.c ${SDL_FLAGS} -DRETROFW"
else
  echo "unknown parameter: $1"
  return 1
fi
  
echo ${COMMAND}
eval ${COMMAND}

if [ $1 = "retrofw" ]; then
  mksquashfs anarch icon.png LICENSE README.md anarch.retrofw.desktop anarch.opk -noappend -no-xattrs
fi

return $?
