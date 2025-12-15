#include "king.h"

#include <ctype.h>
#include <stdlib.h>


bool can_move_king(int y1, int x1, int y2, int x2){
    return abs(y2 - y1) <= 1 && abs(x2 - x1) <= 1;
}

bool castle(int y1, int x1, int y2, int x2, bool is_black)
{
    int row = is_black? 7 : 0;
    char rook = is_black? 'R' : 'r', king = is_black? 'K' : 'k';
    if (y1 != row || y2 != row || x1 != 4 || abs(x1 - x2) != 2 || king_moved[is_black]) 
        return false;
    if (x2 == 2)
    {
        if (board[row][0] != rook || a_rook_moved[is_black] ||
             isalpha(board[row][3]) || isalpha(board[row][2]) || isalpha(board[row][1]))
            return false;
    }else
    {
        if (board[row][7] != rook || h_rook_moved[is_black] ||
             isalpha(board[row][5]) || isalpha(board[row][6]))
            return false;
    }
    bool legal = true;
    for (int i = 0, step = (x2 - x1) / 2; abs(i) <= 2; i += step)
    {
        set_square_color(row, king_location[is_black][1]);
        board[row][x1 + i] = king;
        king_location[is_black][1] = x1 + i;
        if (is_in_check(is_black))
        {
            legal = false;
            break;
        }
    }
    if (legal)
    {
        int rook_column = x2 == 2? 0 : 7;
        king_moved[is_black] = true;
        board[row][(x1 + x2) / 2] = rook;
        set_square_color(row, rook_column);
        return true;
    }
    board[row][x1] = king;
    set_square_color(row, king_location[is_black][1]);
    king_location[is_black][1] = x1;
    return false;
}