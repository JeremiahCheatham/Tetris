#include "level.h"

bool level_new(struct Level **level, SDL_Renderer *renderer) {
    *level = calloc(1, sizeof(struct Level));
    if (!*level) {
        fprintf(stderr, "Error in calloc of level!\n");
        return true;
    }
    struct Level *l = *level;

    l->renderer = renderer;
    l->color = (SDL_Color){0, 255, 0, 255};

    l->font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_FONT_SIZE);
    if (!l->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    l->rect.y = 180;

    if (level_reset(l)) {
        return true;
    }
    return false;
}

void level_free(struct Level **level) {
    if (*level) {
        TTF_CloseFont((*level)->font);
        (*level)->font = NULL;
        SDL_FreeSurface((*level)->surface);
        (*level)->surface = NULL;
        SDL_DestroyTexture((*level)->image);
        (*level)->image = NULL;
        (*level)->renderer = NULL;
        free(*level);
        *level = NULL;
    }
}

bool level_reset(struct Level *l) {
    l->level = 0;
    if (level_update(l)) {
        return true;
    }

    return false;
}

bool level_update(struct Level *l) {
    if (l->image) {
        SDL_DestroyTexture(l->image);
    }
    
    int length = snprintf(NULL, 0, "%d", l->level) + 1;
    char level_str[length];
    snprintf(level_str, (size_t)length, "%d", l->level);

    l->surface = TTF_RenderText_Blended(l->font, level_str, l->color);
    if (!l->surface) {
        fprintf(stderr, "Error creating a surface: %s\n", SDL_GetError());
        return true;
    }

    l->image = SDL_CreateTextureFromSurface(l->renderer, l->surface);
    if (!l->image) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(l->surface);
    l->surface = NULL;

    if (SDL_QueryTexture(l->image, NULL, NULL, &l->rect.w, &l->rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    l->rect.x = 510 - (int)(l->rect.w / 2.0);
    
    return false;
}

bool level_increment(struct Level *l) {
    l->level++;
    if (level_update(l)) {
        return true;
    }

    return false;
}

bool level_draw(struct Level *l) {
    if (SDL_RenderCopy(l->renderer, l->image, NULL, &l->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}
