#include "message.h"
#include "bubble_text.h"

bool message_new(struct Message **message, SDL_Renderer *renderer) {
    *message = calloc(1, sizeof(struct Message));
    if (!*message) {
        fprintf(stderr, "Error in calloc of message!\n");
        return true;
    }
    struct Message *m = *message;

    m->renderer = renderer;

    SDL_Color front_color = {255, 255, 255, 255};
    SDL_Color back_color = {0, 0, 0, 255};

    m->font = TTF_OpenFont("fonts/freesansbold.ttf", MESSAGE_FONT_SIZE);
    if (!m->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    m->surface = bubble_text_surf("Press", m->font, 8, front_color, back_color);
    if (!m->surface) {
        return true;
    }

    m->press_image = SDL_CreateTextureFromSurface(m->renderer, m->surface);
    if (!m->press_image) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    if (SDL_QueryTexture(m->press_image, NULL, NULL, &m->press_rect.w, &m->press_rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(m->surface);
    m->surface = NULL;

    m->surface = bubble_text_surf("SPACE", m->font, 8, front_color, back_color);

    if (!m->surface) {
        return true;
    }

    m->space_image = SDL_CreateTextureFromSurface(m->renderer, m->surface);
    if (!m->space_image) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    if (SDL_QueryTexture(m->space_image, NULL, NULL, &m->space_rect.w, &m->space_rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(m->surface);
    m->surface = NULL;

    m->press_rect.x = 180 - (int)(m->press_rect.w / 2.0);
    m->press_rect.y = 300 - m->press_rect.h;

    m->space_rect.x = 180 - (int)(m->space_rect.w / 2.0);
    m->space_rect.y = 300;

    return false;
}

void message_free(struct Message **message) {
    if (*message) {
        TTF_CloseFont((*message)->font);
        (*message)->font = NULL;
        SDL_FreeSurface((*message)->surface);
        (*message)->surface = NULL;
        SDL_DestroyTexture((*message)->press_image);
        (*message)->press_image = NULL;
        SDL_DestroyTexture((*message)->space_image);
        (*message)->space_image = NULL;
        (*message)->renderer = NULL;
        free(*message);
        *message = NULL;
    }
}

bool message_draw(struct Message *m) {
    if (SDL_RenderCopy(m->renderer, m->press_image, NULL, &m->press_rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    if (SDL_RenderCopy(m->renderer, m->space_image, NULL, &m->space_rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}
