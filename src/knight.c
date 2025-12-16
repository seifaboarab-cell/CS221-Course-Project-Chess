#include "knight.h"
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
bool can_move_knight(int y1, int x1, int y2, int x2)
{
    if ((abs(y2 - y1) == 2) && (abs(x2 - x1) == 1))
    {
        return true;
    }
    else if ((abs(y2 - y1) == 1) && (abs(x2 - x1) == 2))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool knight_has_legal_move(int y1, int x1, bool is_black)
{
    int knight_moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};
    for (int i = 0; i < 8; i++)
    {
        int moved_y = y1 + knight_moves[i][0], moved_x = x1 + knight_moves[i][1];
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