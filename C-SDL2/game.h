#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "bubble_text.h"
#include "background.h"
#include "score.h"
#include "level.h"
#include "lines.h"
#include "tetrominos.h"
#include "board.h"
#include "message.h"
#include "fps.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct Background *background;
    struct Score *score;
    struct Level *level;
    struct Lines *lines;
    struct Tetrominos *tetrominos;
    struct Board *board;
    struct Message *message;
    struct Fps *fps;
    bool playing;
    double delta_time;
    int counter;
    double target_count;
    double current_count;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_transfer_shape(struct Tetrominos *t, struct Board *b);
bool game_reset(struct Game *g);
void game_level_setup(struct Game *g);
bool game_update(struct Game *g);
bool game_move_shape(struct Tetrominos *t, struct Board *b, int move_x, int move_y);
void game_rotate_shape(struct Tetrominos *t, struct Board *b);
bool game_run(struct Game *g);

#endif
