#ifndef LINES_H
#define LINES_H

#include "main.h"

struct Lines {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_Surface *surface;
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect rect;
    Uint32 lines;
};

bool lines_new(struct Lines **lines, SDL_Renderer *renderer);
void lines_free(struct Lines **lines);
bool lines_reset(struct Lines *l);
bool lines_update(struct Lines *l);
bool lines_increment(struct Lines *l, Uint32 new_lines);
bool lines_draw(struct Lines *l);

#endif
