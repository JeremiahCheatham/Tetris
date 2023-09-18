#ifndef TETROMINOS_H
#define TETROMINOS_H

#include "main.h"

typedef bool Shape[4][4];

struct Tetrominos {
    SDL_Renderer *renderer;
    SDL_Rect rect;
    int x;
    int y;
    SDL_Texture *tetromino[4][4];
    SDL_Texture *next_tetromino[4][4];
    int size;
    int next_size;
    int next_x;
    int next_y;
};

bool tetrominos_new(struct Tetrominos **tetrominos, SDL_Renderer *renderer);
void tetrominos_free(struct Tetrominos **tetrominos);
bool tetrominos_generate_image(SDL_Renderer *renderer, SDL_Texture **image, SDL_Color front_color, SDL_Color back_color); 
bool tetrominos_generate_shape(struct Tetrominos *t);
bool tetrominos_draw(struct Tetrominos *t);

#endif
