#ifndef BUBBLE_TEXT_H
#define BUBBLE_TEXT_H

#include "main.h"

SDL_Surface *bubble_text_surf(
        const char* text,
        TTF_Font *font,
        int radius,
        SDL_Color outer_color,
        SDL_Color center_color
);
void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *surface, int radius, int x, int y); 
#endif
