#include <stdio.h>
#include "display.h"

static void print_divider(void) {
    printf("***************************************************************************\n");
}

void show_menu(void) {
    print_divider();
    printf("                         DOTS AND BOXES (4x5)                              \n");
    print_divider();
    printf("1. Start Two-Player Game (Player A vs Player B)\n");
    printf("2. How to Play\n");
    printf("3. Exit\n");
    printf("Select an option (1-3): ");
}

void print_board(const Board *b) {
    print_divider();
    // Column headers: 0 1 2 3 4 5
    printf("  ");
    for (int c = 0; c < DOT_COLS; c++) {
        printf("%d ", c);
    }
    printf("\n");

    for (int r = 0; r < DOT_ROWS; r++) {
        // Print dot row with horizontal lines
        printf("%d ", r);
        for (int c = 0; c < DOT_COLS; c++) {
            printf(".");
            if (c < BOX_COLS) {
                printf("%c", board_has_edge(b, move_to_edge((Move){r, c, r, c + 1})) ? '-' : ' ');
            }
        }
        printf("\n");

        // Print vertical lines and box owners between dot rows
        if (r < BOX_ROWS) {
            printf("  ");
            for (int c = 0; c < DOT_COLS; c++) {
                printf("%c", board_has_edge(b, move_to_edge((Move){r, c, r + 1, c})) ? '|' : ' ');
                if (c < BOX_COLS) {
                    printf("%c", b->owner[r][c]);
                }
            }
            printf("\n");
        }
    }
}

void print_scores(const Board *b) {
    print_divider();
    printf("Player A score: %d\n", b->score[0]);
    printf("Player B score: %d\n", b->score[1]);
}

void print_winner(const Board *b) {
    print_scores(b);
    print_divider();
    if (b->score[0] > b->score[1]) {
        printf("Game Over! Player A wins (%d to %d)!\n", b->score[0], b->score[1]);
    } else if (b->score[1] > b->score[0]) {
        printf("Game Over! Player B wins (%d to %d)!\n", b->score[1], b->score[0]);
    } else {
        printf("Game Over! It's a tie (%d to %d)!\n", b->score[0], b->score[1]);
    }
    print_divider();
}
