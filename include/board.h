#ifndef BOARD_H
#define BOARD_H
#include <stdbool.h>

extern char board[8][8];
extern char capture[2][15];
extern int num_capture[2];
extern int king_location[2][2];
extern bool king_moved[2];
extern bool a_rook_moved[2];
extern bool h_rook_moved[2];
extern int en_passant_flags[2][8];
// for example: en_passant_flag[0][3] = 6 --> when the white pawn in the 4th column moved 2 squares forward, half_move was equal to 6
extern int draw_flag;
extern int half_turn;
extern int turn;
extern int player_number;

void display_board(bool is_black);

void set_square_color(int y, int x);

bool is_in_check(bool is_black);

bool commit_position();

void reset_position();

bool undo();

bool redo();

void free_game();

#endif