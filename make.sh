#!/bin/bash

clear; clear; g++ -x c -g -fmax-errors=5 -pedantic -O3 -Wall -Wextra -o game main.c -lSDL2 2>&1 >/dev/null && ./game
