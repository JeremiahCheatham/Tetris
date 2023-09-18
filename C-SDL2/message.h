#ifndef MESSAGE_H
#define MESSAGE_H

#include "main.h"

struct Message {
    SDL_Renderer *renderer;
    SDL_Texture *press_image;
    SDL_Texture *space_image;
    SDL_Surface *surface;
    TTF_Font *font;
    SDL_Rect press_rect;
    SDL_Rect space_rect;
};

bool message_new(struct Message **message, SDL_Renderer *renderer);
void message_free(struct Message **message);
bool message_draw(struct Message *m);

#endif
