#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

/* Return 1 for a complete line, 0 at end of input, or -1 for a long line. */
static int read_line(char buffer[256]) {
    if (fgets(buffer, 256, stdin) == NULL)
        return 0;
    if (strchr(buffer, '\n') != NULL || feof(stdin))
        return 1;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
    return -1;
}

static bool parse_numbers(const char *line, int *values, int count) {
    for (int i = 0; i < count; ++i) {
        char *end;
        errno = 0;
        long value = strtol(line, &end, 10);
        if (end == line || errno == ERANGE || value < INT_MIN || value > INT_MAX ||
            (*end != '\0' && !isspace((unsigned char)*end)))
            return false;
        values[i] = (int)value;
        line = end;
    }
    while (isspace((unsigned char)*line))
        ++line;
    return *line == '\0';
}

int get_menu_choice(void) {
    char line[256];
    int status;
    while ((status = read_line(line)) != 0) {
        int choice;
        if (status == 1 && parse_numbers(line, &choice, 1) && choice >= 1 && choice <= 3)
            return choice;
        printf("Invalid choice. Please enter 1, 2, or 3: ");
        fflush(stdout);
    }
    return 0;
}

bool get_valid_move(const Board *b, char player, int *r1, int *c1, int *r2, int *c2) {
    char line[256];
    for (;;) {
        printf("Player %c's turn. Enter r1 c1 r2 c2:\n", player);
        fflush(stdout);
        int status = read_line(line);
        int coords[4];
        if (status == 0)
            return false;
        if (status < 0 || !parse_numbers(line, coords, 4)) {
            puts("Invalid input format. Enter exactly 4 integers.");
            continue;
        }
        Move move = {coords[0], coords[1], coords[2], coords[3]};
        int edge = move_to_edge(move);
        if (edge < 0) {
            puts("Invalid move. Dots must be in bounds and directly adjacent.");
            continue;
        }
        if (board_has_edge(b, edge)) {
            puts("That line is already taken! Choose another edge.");
            continue;
        }
        *r1 = move.r1;
        *c1 = move.c1;
        *r2 = move.r2;
        *c2 = move.c2;
        return true;
    }
}
