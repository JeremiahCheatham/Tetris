#include "game.h"
#include "initialize.h"
#include "level.h"
#include "lines.h"
#include "score.h"
#include "tetrominos.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }
    struct Game *g = *game;

    if (game_initilize(g)) {
        return true;
    }

    if (background_new(&g->background, g->renderer)) {
        return true;
    }

    if (score_new(&g->score, g->renderer)) {
        return true;
    }

    if (level_new(&g->level, g->renderer)) {
        return true;
    }

    if (lines_new(&g->lines, g->renderer)) {
        return true;
    }

    if (tetrominos_new(&g->tetrominos, g->renderer)) {
        return true;
    }

    if (board_new(&g->board, g->renderer)) {
        return true;
    }

    if (tetrominos_generate_shape(g->tetrominos)) {
        return true;
    }

    if (message_new(&g->message, g->renderer)) {
        return true;
    }

    if (fps_new(&g->fps)) {
        return true;
    }

    g->playing = false;

    return false;
}

void game_free(struct Game **game) {
    fps_free(&(*game)->fps);
    background_free(&(*game)->background);
    score_free(&(*game)->score);
    level_free(&(*game)->level);
    lines_free(&(*game)->lines);
    tetrominos_free(&(*game)->tetrominos);
    board_free(&(*game)->board);
    message_free(&(*game)->message);

    SDL_DestroyRenderer((*game)->renderer);
    (*game)->renderer = NULL;
    SDL_DestroyWindow((*game)->window);
    (*game)->window = NULL;
    TTF_Quit();
    SDL_Quit();
    free(*game);
    *game = NULL;
}

void game_transfer_shape(struct Tetrominos *t, struct Board *b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->tetromino[i][j]) {
                if (b->board[i + t->y][j + t->x]) {
                    SDL_DestroyTexture(b->board[i + t->y][j + t->x]);
                }
                b->board[i + t->y][j + t->x] = t->tetromino[i][j];
                t->tetromino[i][j] = NULL;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->next_tetromino[i][j]) {
                t->tetromino[i][j] = t->next_tetromino[i][j];
                t->next_tetromino[i][j] = NULL;
            }
        }
    }
    t->size = t->next_size;
    t->y = 0;
    t->x = (10 - t->size) / 2;
}

bool game_move_shape(struct Tetrominos *t, struct Board *b, int move_x, int move_y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->tetromino[i][j]) {
                if (i + t->y + move_y >= 20 || j + t->x + move_x < 0 || j + t->x + move_x >= 10) {
                    return true;
                } else if (b->board[i + t->y + move_y][j + t->x + move_x]) {
                    return true;
                }
            }
        }
    }
    t->x += move_x;
    t->y += move_y;
    return false;
}

void game_rotate_shape(struct Tetrominos *t, struct Board *b) {
    if (t->size == 2) {
        return;
    }
    SDL_Texture *temp[t->size][t->size];
    for (int i = 0; i < t->size; i++) {
        for (int j = 0; j < t->size; j++) {
            if (t->tetromino[i][j]) {
                temp[j][t->size - 1 - i] = t->tetromino[i][j];
            } else {
                temp[j][t->size - 1 - i] = NULL;
            }
        }
    }
    for (int i = 0; i < t->size; i++) {
        for (int j = 0; j < t->size; j++) {
            if (temp[i][j]) {
                if (i + t->y >= 20 || j + t->x < 0 || j + t->x >= 10) {
                    return;
                } else if (b->board[i + t->y][j + t->x]) {
                    return;
                }
            }
        }
    }
    for (int i = 0; i < t->size; i++) {
        for (int j = 0; j < t->size; j++) {
            if (temp[i][j]) {
                t->tetromino[i][j] = temp[i][j];
            } else {
                t->tetromino[i][j] = NULL;
            }
        }
    }
}

bool game_reset(struct Game *g) {
    if (score_reset(g->score)) {
        return true;
    }

    if (level_reset(g->level)) {
        return true;
    }

    if (lines_reset(g->lines)) {
        return true;
    }

    game_transfer_shape(g->tetrominos, g->board);
    if (tetrominos_generate_shape(g->tetrominos)) {
        return true;
    }

    game_level_setup(g);

    g->playing = true;

    return false;
}

void game_level_setup(struct Game *g) {
    board_reset(g->board);

    if (g->level->level < 10) {
        g->target_count = 53 - (g->level->level * 4);
    } else {
        g->target_count = 23 - g->level->level;
    }
}

bool game_update(struct Game *g) {
    g->counter++;
    if (g->counter < g->target_count) {
        return false;
    } else {
        g->counter = 0;
    }

    if (game_move_shape(g->tetrominos, g->board, 0, 1)) {
        if (g->tetrominos->y == 0) {
            g->playing = false;
            return false;
        }
        game_transfer_shape(g->tetrominos, g->board);
        if (tetrominos_generate_shape(g->tetrominos)) {
            return true;
        }
        Uint32 new_lines = board_check_lines(g->board);
        if (new_lines) {
            if (score_increment(g->score, g->level->level, new_lines)) {
                return true;
            }

            if (lines_increment(g->lines, new_lines)) {
                return true;
            }
            if (g->lines->lines > (g->level->level * 10 + 9)) {
                if (level_increment(g->level)) {
                    return true;
                }
                game_level_setup(g);
            }
        }
    }

    return false;
}


bool game_run(struct Game *g) {

    while (true) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
                case SDL_QUIT:
                    return false;
                    break;
                case SDL_KEYDOWN:
                    switch (g->event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            if (g->playing) {
                                g->playing = false;
                            } else {
                                return false;
                            }
                            break;
                        case SDL_SCANCODE_SPACE:
                            if (!g->playing) {
                                if (game_reset(g)) {
                                    return true;
                                }
                            }
                            break;
                        case SDL_SCANCODE_LEFT:
                            game_move_shape(g->tetrominos, g->board, -1, 0);
                            break;
                        case SDL_SCANCODE_RIGHT:
                            game_move_shape(g->tetrominos, g->board, 1, 0);
                            break;
                        case SDL_SCANCODE_UP:
                            game_rotate_shape(g->tetrominos, g->board);
                            break;
                        case SDL_SCANCODE_DOWN:
                            game_move_shape(g->tetrominos, g->board, 0, 1);
                            break;
                        case SDL_SCANCODE_F:
                            fps_toggle_display(g->fps);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        
        if (g->playing) {
            if (game_update(g)) {
                return true;
            }
        }

        if (SDL_RenderClear(g->renderer)) {
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            return true;
        }

        if (background_draw(g->background)) {
            return true;
        }

        if (score_draw(g->score)) {
            return true;
        }

        if (level_draw(g->level)) {
            return true;
        }

        if (lines_draw(g->lines)) {
            return true;
        }

        if (board_draw(g->board)) {
            return true;
        }
        
        if (tetrominos_draw(g->tetrominos)) {
            return true; 
        }

        if (!g->playing) {
            if (message_draw(g->message)) {
                return true;
            }
        }

        SDL_RenderPresent(g->renderer);

        g->delta_time = fps_update(g->fps);
    }
    
    return false;
}
