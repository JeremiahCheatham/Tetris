#include "lines.h"

bool lines_new(struct Lines **lines, SDL_Renderer *renderer) {
    *lines = calloc(1, sizeof(struct Lines));
    if (!*lines) {
        fprintf(stderr, "Error in calloc of lines!\n");
        return true;
    }
    struct Lines *l = *lines;

    l->renderer = renderer;
    l->color = (SDL_Color){160, 32, 240, 255};

    l->font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_FONT_SIZE);
    if (!l->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    l->rect.y = 420;

    if (lines_reset(l)) {
        return true;
    }

    return false;
}

void lines_free(struct Lines **lines) {
    if (*lines) {
        TTF_CloseFont((*lines)->font);
        (*lines)->font = NULL;
        SDL_FreeSurface((*lines)->surface);
        (*lines)->surface = NULL;
        SDL_DestroyTexture((*lines)->image);
        (*lines)->image = NULL;
        (*lines)->renderer = NULL;
        free(*lines);
        *lines = NULL;
    }
}

bool lines_reset(struct Lines *l) {
    l->lines = 0;
    if (lines_update(l)) {
        return true;
    }

    return false;
}

bool lines_update(struct Lines *l) {
    if (l->image) {
        SDL_DestroyTexture(l->image);
    }
    
    int length = snprintf(NULL, 0, "%d", l->lines) + 1;
    char lines_str[length];
    snprintf(lines_str, (size_t)length, "%d", l->lines);

    l->surface = TTF_RenderText_Blended(l->font, lines_str, l->color);
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

bool lines_increment(struct Lines *l, Uint32 new_lines) {
    l->lines += new_lines;
    if (lines_update(l)) {
        return true;
    }

    return false;
}

bool lines_draw(struct Lines *l) {
    if (SDL_RenderCopy(l->renderer, l->image, NULL, &l->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}
