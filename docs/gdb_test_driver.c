#include <stdio.h>
#include "board.h"
int main(void)
{
    Board board;
    board_init(&board);
    board_apply(&board, (Move){0, 0, 0, 1});   /* top of box (0,0) */
    board_apply(&board, (Move){1, 0, 1, 1});   /* bottom of box (0,0) */
    board_apply(&board, (Move){0, 0, 1, 0});   /* left of box (0,0) */
    board_apply(&board, (Move){0, 1, 1, 1});   /* right: completes box (0,0) */
    printf("A=%d B=%d owner=%c\n", board.score[0], board.score[1], board.owner[0][0]);
    return 0;
}
