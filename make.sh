#!/bin/bash

clear; clear; g++ -x c -g -Wall -Wextra -fmax-errors=5 -pedantic -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -o game main.c -lSDL2 2>&1 >/dev/null && ./game
