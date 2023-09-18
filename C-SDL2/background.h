#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "main.h"

struct Background {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_Rect rect;
    TTF_Font *font;
    SDL_Surface *target_surface;
    SDL_Surface *text_surface;
    SDL_Color red;
    SDL_Color orange;
    SDL_Color yellow;
    SDL_Color green;
    SDL_Color blue;
    SDL_Color purple;
};


bool background_new(struct Background **background, SDL_Renderer *renderer);
void background_free(struct Background **background);
bool background_borders(struct Background *b);
bool background_tetris(struct Background *b);
bool background_messages(struct Background *b);
bool background_draw(struct Background *b);

#endif
