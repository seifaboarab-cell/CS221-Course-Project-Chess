#include "king.h"

#include <ctype.h>
#include <stdlib.h>

bool can_move_king(int y1, int x1, int y2, int x2)
{
    return abs(y2 - y1) <= 1 && abs(x2 - x1) <= 1;
}

bool castle(int y1, int x1, int y2, int x2, bool is_black)
{
    int row = is_black ? 7 : 0;
    char rook = is_black ? 'R' : 'r', king = is_black ? 'K' : 'k';
    if (y1 != row || y2 != row || x1 != 4 || abs(x1 - x2) != 2 || king_moved[is_black])
        return false;
    if (x2 == 2)
    {
        if (board[row][0] != rook || a_rook_moved[is_black] ||
            isalpha(board[row][3]) || isalpha(board[row][2]) || isalpha(board[row][1]))
            return false;
    }
    else
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
        int rook_column = x2 == 2 ? 0 : 7;
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
bool king_has_legal_move(int y1, int x1, bool is_black)
{
    int king_moves[8][2] = {{1, 1}, {1, 0}, {1, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {0, -1}};
    for (int i = 0; i < 8; i++)
    {
        int moved_y = y1 + king_moves[i][0], moved_x = x1 + king_moves[i][1];
        if (moved_y >= 0 && moved_y < 8 && moved_x >= 0 && moved_x < 8)
        {
            if (is_black)
            {
                if (!(isupper(board[moved_y][moved_x])))
                {
                    board[moved_y][moved_x] = 'K';
                    if (!(is_in_check(is_black)))
                    {
                        reset_position();
                        return true;
                    }
                    else
                    {
                        reset_position();
                        continue;
                    }
                }
            }
            else if (!is_black)
            {
                if (!(islower(board[moved_y][moved_x])))
                {
                    board[moved_y][moved_x] = 'k';
                    if (!(is_in_check(!is_black)))
                    {
                        reset_position();
                        return true;
                    }
                    else
                    {
                        reset_position();
                        continue;
                    }
                }
            }
        }
    }
    return false;
}