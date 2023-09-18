#include "board.h"


bool board_new(struct Board **board, SDL_Renderer *renderer) {
    *board = calloc(1, sizeof(struct Board));
    if (!*board) {
        fprintf(stderr, "Error in calloc of board!\n");
        return true;
    }
    struct Board *b = *board;

    b->renderer = renderer;
    
    return false;
}

void board_free(struct Board **board) {
    if (*board) {
        for (Uint32 i = 0; i < 20; i++) {
            for (Uint32 j = 0; j < 10; j++) {
                SDL_DestroyTexture((*board)->board[i][j]);
                (*board)->board[i][j] = NULL;
            }
        }

        (*board)->renderer = NULL;

        free(*board);
        *board = NULL;
    }
}

void board_reset(struct Board *b) {
    if (b) {
        for (Uint32 i = 0; i < 20; i++) {
            for (Uint32 j = 0; j < 10; j++) {
                SDL_DestroyTexture(b->board[i][j]);
                b->board[i][j] = NULL;
            }
        }
    }
}

void board_remove_line(struct Board *b, int line) {
    for (int j = 0; j < 10; j++) {
        SDL_DestroyTexture(b->board[line][j]);
        b->board[line][j] = NULL;
        for (int i = line - 1; i >= 0; i--) {
            b->board[i + 1][j] = b->board[i][j];
        }
    }
}

Uint32 board_check_lines(struct Board *b) {
    bool full_line;
    Uint32 lines = 0;
    for (int i = 19; i >= 0; i-- ) {
        full_line = true;
        for (int j = 0; j < 10; j++) {
            if (!b->board[i][j]) {
                full_line = false;
                break;
            }
        }
        if (full_line) {
            lines++;
            board_remove_line(b, i);
            i = 20;
        }
    }
    return lines;
}

bool board_draw(struct Board *b) {
    b->rect.w = SIZE;
    b->rect.h = SIZE;
    for (int i = 0; i < 20; i++) {
        b->rect.y = i * SIZE;
        for (int j = 0; j < 10; j++) {
            b->rect.x = (j + 1) * SIZE;
            if (b->board[i][j]) {
                if (SDL_RenderCopy(b->renderer, b->board[i][j], NULL, &b->rect)) {
                    fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
                    return true;
                }
            }
        }
    }
    return false;
}
