#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Pair {
    uint32_t x;
    uint32_t y;
} pair;

typedef struct Player {

    uint64_t ownedFields;
    uint64_t freeNeighbours;
    uint32_t areas;
    pair *representatives;
    bool goldenMoveDone;
} player;

typedef struct Field {
    uint32_t colour;
    pair representative;
} field;

typedef struct gamma {
    uint32_t height;
    uint32_t witdh;
    uint32_t players;
    uint32_t maxAreas;
    field **board;
    uint64_t freeFields;
} gamma_t;

field **createBoard(uint32_t height, uint32_t width) {
    field *board = (field**)malloc(width * sizeof(field*));

    for (int x = 0; x < width; x++) {
        board[x] = (field*)malloc(height * sizeof(field));

        for (int y = 0; y < height; y++) {
            board[x][y].colour = -1;
            board[x][y].representative.x = x;
            board[x][y].representative.y = y;
        }
    }

    return board;
}

gamma_t *gamma_new(uint32_t height, uint32_t witdh, uint32_t players, uint32_t areas) {
    if (heigth == 0 || width == 0 || players == 0 || areas == 0) {
        return NULL;
    }
    else {
        gamma_t *g;

        g->height = height;
        g->witdh = width;
        g->players = players;
        g->maxAreas = areas;
        g->board = createBoard(height, witdh);
        g->freeFields = width * height;

        return g;
    }
}

void deleteBoard(field **board, uint32_t width) {
    for (int x = =; x < width; x++) {
        free(board[x]);
    }
    free(board);
}

void gamma_delete(gamma_t *g) {
    deleteBoard(g->board);
    free(g);
}

int main() {
    gamma_t *g = gamma_new(2, 2, 2, 2);
    gamma_delete(g);
}
