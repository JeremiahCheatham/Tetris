#include "bubble_text.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>

SDL_Surface *bubble_text_surf(
    const char *text,
    TTF_Font *font,
    int radius,
    SDL_Color outer_color,
    SDL_Color center_color
) {
    SDL_Surface *text_surf = TTF_RenderText_Blended(font, text, outer_color);
    if (!text_surf) {
        fprintf(stderr, "Error creating first TTF surface: %s\n", SDL_GetError());
        return NULL;
    }

    int padding = radius * 2;
    SDL_Surface *target_surf = SDL_CreateRGBSurfaceWithFormat(
        0, text_surf->w + padding, text_surf->h + padding, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!target_surf) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        SDL_FreeSurface(text_surf);
        text_surf = NULL;
        return NULL;
    }

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    blit_symmetric_points(text_surf, target_surf, radius, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        blit_symmetric_points(text_surf, target_surf, radius, x, y);
    }

    SDL_FreeSurface(text_surf);
    text_surf = NULL;

    text_surf = TTF_RenderText_Blended(font, text, center_color);
    if (!text_surf) {
        fprintf(stderr, "Error creating second TTF surface: %s\n", SDL_GetError());
        SDL_FreeSurface(target_surf);
        target_surf = NULL;
        return NULL;
    }

    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius, radius, text_surf->w, text_surf->h });
    
    SDL_FreeSurface(text_surf);
    text_surf = NULL;

    return target_surf;
}

void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf, int radius, int x, int y) {
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + x, radius + y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + x, radius - y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - x, radius + y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - x, radius - y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + y, radius + x, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + y, radius - x, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - y, radius + x, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - y, radius - x, text_surf->w, text_surf->h });
}

