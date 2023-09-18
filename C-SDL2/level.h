#ifndef LEVEL_H
#define LEVEL_H

#include "main.h"

struct Level {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_Surface *surface;
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect rect;
    Uint32 level;
};

bool level_new(struct Level **level, SDL_Renderer *renderer);
void level_free(struct Level **level);
bool level_reset(struct Level *l);
bool level_update(struct Level *l);
bool level_increment(struct Level *l);
bool level_draw(struct Level *l);

#endif
