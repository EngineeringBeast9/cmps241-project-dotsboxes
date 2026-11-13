/*
 * Unit tests for the Board module: coordinate mapping, move validation,
 * box capture, scoring, turn rules and the winner.
 *
 * Build and run:
 *   gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -Iinclude \
 *       -o build/test_board tests/test_board.c src/board.c && ./build/test_board
 */
#include "board.h"

#include <stdio.h>
#include <string.h>

static int checks = 0;
static int failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        ++checks;                                                       \
        if (!(cond)) {                                                  \
            ++failures;                                                 \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        }                                                               \
    } while (0)

static void test_init(void)
{
    Board board;
    board_init(&board);
    CHECK(board.lines_left == 49);
    CHECK(EDGE_COUNT == 49 && BOX_COUNT == 20);
    CHECK(board.current == 0);
    CHECK(board.score[0] == 0 && board.score[1] == 0);
    CHECK(board_invariant(&board));
    CHECK(!board_finished(&board));
}

static void test_coordinates(void)
{
    Board board;
    board_init(&board);
    for (int edge = 0; edge < EDGE_COUNT; ++edge) {
        Move move = edge_to_move(edge);
        Move reverse = {move.r2, move.c2, move.r1, move.c1};
        CHECK(move_to_edge(move) == edge);
        CHECK(move_to_edge(reverse) == edge);
        CHECK(board_valid_move(&board, move));
    }
    CHECK(move_to_edge((Move){-1, 0, 0, 0}) == -1);   /* off board */
    CHECK(move_to_edge((Move){0, 6, 1, 6}) == -1);    /* column 6 */
    CHECK(move_to_edge((Move){0, 0, 1, 1}) == -1);    /* diagonal */
    CHECK(move_to_edge((Move){0, 0, 0, 2}) == -1);    /* not adjacent */
    CHECK(move_to_edge((Move){0, 0, 0, 0}) == -1);    /* same dot */
    CHECK(board_box_sides(&board, -1, 0) == -1);
}

static void test_invalid_moves_leave_board_unchanged(void)
{
    Board board;
    board_init(&board);
    CHECK(board_apply(&board, (Move){0, 0, 0, 1}) == 0);
    Board before = board;
    CHECK(board_apply(&board, (Move){0, 0, 1, 1}) == -1);   /* diagonal */
    CHECK(board_apply(&board, (Move){0, 0, 0, 1}) == -1);   /* already drawn */
    CHECK(board_apply(&board, (Move){0, 1, 0, 0}) == -1);   /* drawn, reversed */
    CHECK(memcmp(&board, &before, sizeof board) == 0);
}

static void test_turn_passes_without_capture(void)
{
    Board board;
    board_init(&board);
    CHECK(board_apply(&board, (Move){0, 0, 0, 1}) == 0);
    CHECK(board.current == 1);
    CHECK(board_apply(&board, (Move){0, 1, 0, 2}) == 0);
    CHECK(board.current == 0);
}

static void test_single_capture_gives_extra_turn(void)
{
    Board board;
    board_init(&board);
    const Move sides[] = {{0,0,0,1}, {1,0,1,1}, {0,0,1,0}};
    for (int i = 0; i < 3; ++i)
        CHECK(board_apply(&board, sides[i]) == 0);
    CHECK(board.current == 1);                      /* B to move */
    CHECK(board_apply(&board, (Move){0,1,1,1}) == 1);
    CHECK(board.owner[0][0] == 'B');
    CHECK(board.score[1] == 1 && board.score[0] == 0);
    CHECK(board.current == 1);                      /* B moves again */
    CHECK(board_invariant(&board));
}

static void test_double_capture(void)
{
    Board board;
    board_init(&board);
    const Move sides[] = {{0,0,0,1}, {1,0,1,1}, {0,0,1,0},
                          {0,1,0,2}, {1,1,1,2}, {0,2,1,2}};
    for (int i = 0; i < 6; ++i)
        CHECK(board_apply(&board, sides[i]) == 0);
    CHECK(board.current == 0);
    CHECK(board_apply(&board, (Move){0,1,1,1}) == 2);
    CHECK(board.owner[0][0] == 'A' && board.owner[0][1] == 'A');
    CHECK(board.score[0] == 2 && board.current == 0);
    CHECK(board_invariant(&board));
}

/* Deterministic pseudo-random games: every game must end with a valid,
 * consistent board and a winner that matches the scores. */
static void test_random_games(void)
{
    unsigned state = 241U;
    for (int game = 0; game < 200; ++game) {
        Board board;
        board_init(&board);
        int moves_made = 0;
        while (!board_finished(&board)) {
            Move legal[EDGE_COUNT];
            int count = board_moves(&board, legal);
            CHECK(count == board.lines_left);
            state = state * 1664525U + 1013904223U;
            Move pick = legal[(state >> 8) % (unsigned)count];
            CHECK(board_apply(&board, pick) >= 0);
            ++moves_made;
            CHECK(board_invariant(&board));
        }
        CHECK(moves_made == EDGE_COUNT);
        CHECK(board.score[0] + board.score[1] == BOX_COUNT);
        int winner = board_winner(&board);
        if (board.score[0] > board.score[1])
            CHECK(winner == 0);
        else if (board.score[1] > board.score[0])
            CHECK(winner == 1);
        else
            CHECK(winner == -1);
    }
}

int main(void)
{
    test_init();
    test_coordinates();
    test_invalid_moves_leave_board_unchanged();
    test_turn_passes_without_capture();
    test_single_capture_gives_extra_turn();
    test_double_capture();
    test_random_games();

    printf("%d checks, %d failures\n", checks, failures);
    if (failures == 0) {
        puts("ALL BOARD TESTS PASSED");
        return 0;
    }
    return 1;
}
