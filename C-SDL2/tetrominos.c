#include "tetrominos.h"

bool tetrominos_new(struct Tetrominos **tetrominos, SDL_Renderer *renderer) {
    *tetrominos = calloc(1, sizeof(struct Tetrominos));
    if (!*tetrominos) {
        fprintf(stderr, "Error in calloc of tetrominos!\n");
        return true;
    }
    struct Tetrominos *t = *tetrominos;

    t->renderer = renderer;
    t->rect.w = SIZE;
    t->rect.h = SIZE;

    return false;
}

void tetrominos_free(struct Tetrominos **tetrominos) {
    if (*tetrominos) {
        for (Uint32 i = 0; i < 4; i++) {
            for (Uint32 j = 0; j < 4; j++) {
                SDL_DestroyTexture((*tetrominos)->tetromino[i][j]);
                (*tetrominos)->tetromino[i][j] = NULL;
                SDL_DestroyTexture((*tetrominos)->next_tetromino[i][j]);
                (*tetrominos)->next_tetromino[i][j] = NULL;
            }
        }

        (*tetrominos)->renderer = NULL;

        free(*tetrominos);
    *tetrominos = NULL;
    }
}

bool tetrominos_generate_image(SDL_Renderer *renderer, SDL_Texture **image, SDL_Color front_color, SDL_Color back_color) {
    SDL_Surface *surface = SDL_CreateRGBSurface(0,SIZE, SIZE, 32,0,0,0,255);
    if (!surface) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        return true;
    }
    
    SDL_Rect rect = {2, 2, SIZE - 4, SIZE - 4};

    if (SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, front_color.r, front_color.g, front_color.b))) {
        fprintf(stderr, "Error creating a filled rect: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return true;
    }
    
    rect.x += 2;
    rect.y += 2;
    rect.w -= 4;
    rect.h -= 4;
    
    if (SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, back_color.r, back_color.g, back_color.b))) {
        fprintf(stderr, "Error creating a filled rect: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return true;
    }
 
    rect.x += 3;
    rect.y += 3;
    rect.w -= 6;
    rect.h -= 6;
     
    if (SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, front_color.r, front_color.g, front_color.b))) {
        fprintf(stderr, "Error creating a filled rect: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return true;
    }
 
    rect.x += 5;
    rect.y += 5;
    rect.w -= 10;
    rect.h -= 10;
 
    if (SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, back_color.r, back_color.g, back_color.b))) {
        fprintf(stderr, "Error creating a filled rect: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return true;
    }

    *image = SDL_CreateTextureFromSurface(renderer, surface);
    if (!image) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return true;
    }

    SDL_FreeSurface(surface);

    return false;
}

bool tetrominos_generate_shape(struct Tetrominos *t) {
    Uint8 r = (Uint8)(rand() % 235) + 20;
    Uint8 g = (Uint8)(rand() % 235) + 20;
    Uint8 b = (Uint8)(rand() % 235) + 20;
    SDL_Color front_color = {r, g, b, 255};
    SDL_Color back_color = {(Uint8)(r / 2.0), (Uint8)(g / 2.0), (Uint8)(b / 2.0), 255};

    int rand_shape = rand() % 7;
    int shape[4][2];
    switch (rand_shape) {
        case 0:
            shape[0][0] = 1; shape[0][1] = 0;
            shape[1][0] = 1; shape[1][1] = 1;
            shape[2][0] = 1; shape[2][1] = 2;
            shape[3][0] = 1; shape[3][1] = 3;
            t->next_size = 4;
            break;
        case 1:
            shape[0][0] = 0; shape[0][1] = 0;
            shape[1][0] = 1; shape[1][1] = 0;
            shape[2][0] = 1; shape[2][1] = 1;
            shape[3][0] = 1; shape[3][1] = 2;
            t->next_size = 3;
            break;
        case 2:
            shape[0][0] = 0; shape[0][1] = 2;
            shape[1][0] = 1; shape[1][1] = 0;
            shape[2][0] = 1; shape[2][1] = 1;
            shape[3][0] = 1; shape[3][1] = 2;
            t->next_size = 3;
            break;
        case 3:
            shape[0][0] = 0; shape[0][1] = 0;
            shape[1][0] = 0; shape[1][1] = 1;
            shape[2][0] = 1; shape[2][1] = 0;
            shape[3][0] = 1; shape[3][1] = 1;
            t->next_size = 2;
            break;
        case 4:
            shape[0][0] = 0; shape[0][1] = 1;
            shape[1][0] = 0; shape[1][1] = 2;
            shape[2][0] = 1; shape[2][1] = 0;
            shape[3][0] = 1; shape[3][1] = 1;
            t->next_size = 3;
            break;
        case 5:
            shape[0][0] = 0; shape[0][1] = 1;
            shape[1][0] = 1; shape[1][1] = 0;
            shape[2][0] = 1; shape[2][1] = 1;
            shape[3][0] = 1; shape[3][1] = 2;
            t->next_size = 3;
            break;
        case 6:
            shape[0][0] = 0; shape[0][1] = 0;
            shape[1][0] = 0; shape[1][1] = 1;
            shape[2][0] = 1; shape[2][1] = 1;
            shape[3][0] = 1; shape[3][1] = 2;
            t->next_size = 3;
            break;
    }

    t->next_x = 510 - (SIZE * t->next_size / 2);
    if (t->next_size == 4) {
        t->next_y = 300 - SIZE - (SIZE / 2);
    } else {
        t->next_y = 300 - SIZE;
    }

    for (Uint32 i = 0; i < 4; i++) {
        if (tetrominos_generate_image(t->renderer, &t->next_tetromino[shape[i][0]][shape[i][1]], front_color, back_color)) {
            return true;
        }
    }

    return false;
}

bool tetrominos_draw(struct Tetrominos *t) {
    t->rect.w = SIZE;
    t->rect.h = SIZE;
    for (int i = 0; i < 4; i++) {
        t->rect.y = (i + t->y) * SIZE;
        for (int j = 0; j < 4; j++) {
            t->rect.x = (j + 1 + t->x) * SIZE;
            if (t->tetromino[i][j]) {
                if (SDL_RenderCopy(t->renderer, t->tetromino[i][j], NULL, &t->rect)) {
                    fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
                    return true;
                }
            }  
        }
    }
    for (int i = 0; i < 4; i++) {
        t->rect.y = i * SIZE + t->next_y;
        for (int j = 0; j < 4; j++) {
            t->rect.x = j * SIZE + t->next_x;
            if (t->next_tetromino[i][j]) {
                if (SDL_RenderCopy(t->renderer, t->next_tetromino[i][j], NULL, &t->rect)) {
                    fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
                    return true;
                }
            }  
        }
    }

    return false;
}
