#include "board.h"

#include <string.h>

void board_init(Board *board)
{
    memset(board, 0, sizeof(*board));
    memset(board->owner, ' ', sizeof(board->owner));
    board->lines_left = EDGE_COUNT;
}

int move_to_edge(Move move)
{
    if (move.r1 < 0 || move.r1 >= DOT_ROWS || move.r2 < 0 || move.r2 >= DOT_ROWS ||
        move.c1 < 0 || move.c1 >= DOT_COLS || move.c2 < 0 || move.c2 >= DOT_COLS)
        return -1;
    if (move.r1 == move.r2 && (move.c1 - move.c2 == 1 || move.c2 - move.c1 == 1))
        return move.r1 * BOX_COLS + (move.c1 < move.c2 ? move.c1 : move.c2);
    if (move.c1 == move.c2 && (move.r1 - move.r2 == 1 || move.r2 - move.r1 == 1))
        return DOT_ROWS * BOX_COLS + (move.r1 < move.r2 ? move.r1 : move.r2) * DOT_COLS + move.c1;
    return -1;
}

Move edge_to_move(int edge)
{
    Move move = {-1, -1, -1, -1};
    if (edge >= 0 && edge < DOT_ROWS * BOX_COLS) {
        move.r1 = move.r2 = edge / BOX_COLS;
        move.c1 = edge % BOX_COLS;
        move.c2 = move.c1 + 1;
    } else if (edge >= DOT_ROWS * BOX_COLS && edge < EDGE_COUNT) {
        edge -= DOT_ROWS * BOX_COLS;
        move.r1 = edge / DOT_COLS;
        move.r2 = move.r1 + 1;
        move.c1 = move.c2 = edge % DOT_COLS;
    }
    return move;
}

bool board_has_edge(const Board *board, int edge)
{
    return edge >= 0 && edge < EDGE_COUNT && (board->edges & (UINT64_C(1) << edge)) != 0;
}

bool board_valid_move(const Board *board, Move move)
{
    int edge = move_to_edge(move);
    return edge >= 0 && !board_has_edge(board, edge);
}

int board_box_sides(const Board *board, int row, int col)
{
    if (row < 0 || row >= BOX_ROWS || col < 0 || col >= BOX_COLS)
        return -1;
    return (int)board_has_edge(board, row * BOX_COLS + col)
         + (int)board_has_edge(board, (row + 1) * BOX_COLS + col)
         + (int)board_has_edge(board, DOT_ROWS * BOX_COLS + row * DOT_COLS + col)
         + (int)board_has_edge(board, DOT_ROWS * BOX_COLS + row * DOT_COLS + col + 1);
}

static int claim(Board *board, int row, int col)
{
    if (row < 0 || row >= BOX_ROWS || col < 0 || col >= BOX_COLS ||
        board->owner[row][col] != ' ' || board_box_sides(board, row, col) != 4)
        return 0;
    board->owner[row][col] = (char)('A' + board->current);
    ++board->score[board->current];
    return 1;
}

int board_apply(Board *board, Move move)
{
    int edge = move_to_edge(move);
    int captured;
    if (edge < 0 || board_has_edge(board, edge))
        return -1;
    board->edges |= UINT64_C(1) << edge;
    --board->lines_left;
    if (edge < DOT_ROWS * BOX_COLS) {
        int row = edge / BOX_COLS, col = edge % BOX_COLS;
        captured = claim(board, row - 1, col) + claim(board, row, col);
    } else {
        int row = (edge - DOT_ROWS * BOX_COLS) / DOT_COLS;
        int col = (edge - DOT_ROWS * BOX_COLS) % DOT_COLS;
        captured = claim(board, row, col - 1) + claim(board, row, col);
    }
    if (captured == 0)
        board->current = 1 - board->current;
    return captured;
}

int board_moves(const Board *board, Move moves[EDGE_COUNT])
{
    int count = 0;
    for (int edge = 0; edge < EDGE_COUNT; ++edge)
        if (!board_has_edge(board, edge))
            moves[count++] = edge_to_move(edge);
    return count;
}

bool board_finished(const Board *board)
{
    return board->lines_left == 0;
}

int board_winner(const Board *board)
{
    if (board->score[0] == board->score[1])
        return -1;
    return board->score[0] > board->score[1] ? 0 : 1;
}

bool board_invariant(const Board *board)
{
    int used = 0, scores[2] = {0, 0};
    if (board->current < 0 || board->current > 1 || (board->edges >> EDGE_COUNT) != 0)
        return false;
    for (int edge = 0; edge < EDGE_COUNT; ++edge)
        used += (int)board_has_edge(board, edge);
    for (int r = 0; r < BOX_ROWS; ++r) {
        for (int c = 0; c < BOX_COLS; ++c) {
            char owner = board->owner[r][c];
            if (owner != ' ' && owner != 'A' && owner != 'B')
                return false;
            if ((owner != ' ') != (board_box_sides(board, r, c) == 4))
                return false;
            if (owner != ' ')
                ++scores[owner - 'A'];
        }
    }
    return used + board->lines_left == EDGE_COUNT &&
           board->score[0] == scores[0] && board->score[1] == scores[1];
}
