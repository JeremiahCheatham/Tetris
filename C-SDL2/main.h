#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SDL_FLAGS (SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)
#define WINDOW_TITLE "Tetris"
#define WINDOW_WIDTH 660
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60
#define TEXT_FONT_SIZE 46
#define MESSAGE_FONT_SIZE 80
#define SIZE 30

// Length of array macro.
#define LEN(x) (sizeof(x)/sizeof(x[0]))

#endif
