#include "background.h"
#include "bubble_text.h"

bool background_new(struct Background **background, SDL_Renderer *renderer) {
    *background = calloc(1, sizeof(struct Background));
    if (!*background) {
        fprintf(stderr, "Error in calloc of background!\n");
        return true;
    }
    struct Background *b = *background;

    b->renderer = renderer;

    b->red = (SDL_Color){255, 0, 0, 255};
    b->orange = (SDL_Color){255, 165, 0, 255};
    b->yellow = (SDL_Color){255, 255, 0, 255};
    b->green = (SDL_Color){0, 255, 0, 255};
    b->blue = (SDL_Color){0, 0, 255, 255};
    b->purple = (SDL_Color){160, 32, 240, 255};

    b->target_surface = SDL_CreateRGBSurfaceWithFormat(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!b->target_surface) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        return true;
    }

    if (background_borders(b)) {
        return true;
    }
    if (background_tetris(b)) {
        return true;
    }
    if (background_messages(b)) {
        return true;
    }

    b->image = SDL_CreateTextureFromSurface(b->renderer, b->target_surface);
    if (!b->image) {
        fprintf(stderr, "Error creating a text texture: %s\n", SDL_GetError());
        return true;
    }

    if (SDL_QueryTexture(b->image, NULL, NULL, &b->rect.w, &b->rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

void background_free(struct Background **background) {
    if (*background) {
        TTF_CloseFont((*background)->font);
        (*background)->font = NULL;

        SDL_FreeSurface((*background)->target_surface);
        (*background)->target_surface = NULL;

        SDL_FreeSurface((*background)->text_surface);
        (*background)->text_surface = NULL;

        SDL_DestroyTexture((*background)->image);
        (*background)->image = NULL;

        (*background)->renderer = NULL;

        free(*background);
        *background = NULL;
    }
    return;
}

bool background_borders(struct Background *b) {
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color colors[] = {b->purple, b->blue, b->green, b->yellow, b->orange, b->red, black};

    b->rect.h = WINDOW_HEIGHT;
    b->rect.w = 360;
    for (Uint32 i = 0; i < LEN(colors); i++) {
        SDL_Color color = colors[i];
        if (SDL_FillRect(b->target_surface, &b->rect, SDL_MapRGB(b->target_surface->format, color.r, color.g, color.b))) {
            fprintf(stderr, "Error creating a filled rectangle: %s\n", SDL_GetError());
            return true;
        }
        b->rect.w -= 10;
        b->rect.x += 5;
    }
    b->rect.x = 0;
    return false;
}

bool background_tetris(struct Background *b) {
    SDL_Color darkred = {139, 0, 0, 255};
    SDL_Color chocolate = {210, 105, 30, 255};
    SDL_Color goldenrod = {218, 165, 32, 255};
    SDL_Color darkgreen = {0, 100, 0, 255};
    SDL_Color darkblue = {0, 0, 139, 255};
    SDL_Color darkslateblue = {72, 61, 139, 255};

    b->font = TTF_OpenFont("fonts/freesansbold.ttf", 90);
    if (!b->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    b->text_surface = bubble_text_surf("s", b->font, 8, b->purple, darkslateblue);
    if (!b->text_surface) {
        return true;
    }
    int x = (int)((300.0 / 7 * 6) - (b->text_surface->w / 2.0)) + 360;
    int y = WINDOW_HEIGHT - b->text_surface->h;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = bubble_text_surf("i", b->font, 8, b->blue, darkblue);
    if (!b->text_surface) {
        return true;
    }
    x -= b->text_surface->w - 12;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = bubble_text_surf("r", b->font, 8, b->green, darkgreen);
    if (!b->text_surface) {
        return true;
    }
    x -= b->text_surface->w - 12;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = bubble_text_surf("t", b->font, 8, b->yellow, goldenrod);
    if (!b->text_surface) {
        return true;
    }
    x -= b->text_surface->w - 12;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = bubble_text_surf("e", b->font, 8, b->orange, chocolate);
    if (!b->text_surface) {
        return true;
    }
    x -= b->text_surface->w - 12;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = bubble_text_surf("T", b->font, 8, b->red, darkred);
    if (!b->text_surface) {
        return true;
    }
    x -= b->text_surface->w - 12;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

bool background_messages(struct Background *b) {
    TTF_CloseFont(b->font);
    b->font = NULL;
    b->font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_FONT_SIZE);
    if (!b->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = TTF_RenderText_Blended(b->font, "Score", b->red);
    if (!b->text_surface) {
        fprintf(stderr, "Error creating first TTF surface: %s\n", SDL_GetError());
        return true;
    }
    int x = 510 - (b->text_surface->w / 2);
    int y = 60 - b->text_surface->h;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }
 
    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = TTF_RenderText_Blended(b->font, "Level", b->yellow);
    if (!b->text_surface) {
        fprintf(stderr, "Error creating first TTF surface: %s\n", SDL_GetError());
        return true;
    }
    x = 510 - (b->text_surface->w / 2);
    y = 180 - b->text_surface->h;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }
    
    SDL_FreeSurface(b->text_surface);
    b->text_surface = NULL;
    b->text_surface = TTF_RenderText_Blended(b->font, "Lines", b->blue);
    if (!b->text_surface) {
        fprintf(stderr, "Error creating first TTF surface: %s\n", SDL_GetError());
        return true;
    }
    x = 510 - (b->text_surface->w / 2);
    y = 420 - b->text_surface->h;
    if (SDL_BlitSurface(b->text_surface, NULL, b->target_surface, &(SDL_Rect){ x, y, b->text_surface->w, b->text_surface->h })) {
        fprintf(stderr, "Error blitting surface: %s\n", SDL_GetError());
        return true;
    }

    return false;
}

bool background_draw(struct Background *b) {
    if (SDL_RenderCopy(b->renderer, b->image, NULL, &b->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}
