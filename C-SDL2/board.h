#ifndef BOARD_H
#define BOARD_H

#include "main.h"

struct Board {
    SDL_Renderer *renderer;
    SDL_Texture *board[20][10];
    SDL_Rect rect;
};

bool board_new(struct Board **board, SDL_Renderer *renderer);
void board_free(struct Board **board);
void board_reset(struct Board *b);
void board_remove_line(struct Board *b, int line);
Uint32 board_check_lines(struct Board *b);
bool board_draw(struct Board *b);

#endif
