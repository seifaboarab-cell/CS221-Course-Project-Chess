#ifndef BOARD_H
#define BOARD_H

extern char board[8][8];
extern char capture[2][15];
extern int num_capture[2];

void display_board();

void set_square_color(int y, int x);

#endif