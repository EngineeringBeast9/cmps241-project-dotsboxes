#include <stdio.h>
#include "board.h"
#include "display.h"
#include "input.h"

static void run_game(void) {
    Board board;
    board_init(&board);

    int r1, c1, r2, c2;

    while (!board_finished(&board)) {
        print_board(&board);
        if (!get_valid_move(&board, (char)('A' + board.current), &r1, &c1, &r2, &c2))
            return;
        board_apply(&board, (Move){r1, c1, r2, c2});
        print_scores(&board);
    }

    print_board(&board);
    print_winner(&board);
}

int main(void) {
    while (1) {
        show_menu();
        fflush(stdout);
        int choice = get_menu_choice();

        if (choice == 1) {
            run_game();
        } else if (choice == 2) {
            printf("\nRules: Connect two adjacent dots by entering their row and column coordinates.\n");
            printf("Completing the 4th side of a box claims it with your initial and grants an extra turn!\n\n");
        } else if (choice == 3 || choice == 0) {
            printf("Exiting Dots and Boxes. Goodbye!\n");
            break;
        }
    }
    return 0;
}
