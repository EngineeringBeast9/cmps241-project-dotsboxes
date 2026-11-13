#ifndef DAB_BOARD_H
#define DAB_BOARD_H

#include <stdbool.h>
#include <stdint.h>

#define DOT_ROWS 5
#define DOT_COLS 6
#define BOX_ROWS (DOT_ROWS - 1)
#define BOX_COLS (DOT_COLS - 1)
#define BOX_COUNT (BOX_ROWS * BOX_COLS)
#define EDGE_COUNT (DOT_ROWS * BOX_COLS + BOX_ROWS * DOT_COLS)

typedef struct { int r1, c1, r2, c2; } Move;
typedef struct {
    uint64_t edges;                 /* bits 0..24 horizontal, 25..48 vertical */
    char owner[BOX_ROWS][BOX_COLS];
    int score[2];
    int current;                   /* 0 = A, 1 = B */
    int lines_left;
} Board;

void board_init(Board *board);
int move_to_edge(Move move);        /* -1 for invalid coordinates */
Move edge_to_move(int edge);
bool board_has_edge(const Board *board, int edge);
bool board_valid_move(const Board *board, Move move);
int board_box_sides(const Board *board, int row, int col);
int board_apply(Board *board, Move move); /* -1 invalid, else 0..2 captures */
int board_moves(const Board *board, Move moves[EDGE_COUNT]);
bool board_finished(const Board *board);
bool board_invariant(const Board *board);
int board_winner(const Board *board); /* -1 tie, 0 A, 1 B */

#endif
