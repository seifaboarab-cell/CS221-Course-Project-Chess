#include "bishop.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
bool can_move_bishop(int y1, int x1, int y2, int x2)
{
    if (!(abs(y2 - y1) == abs(x2 - x1)))
    {
        return false;
    }
    else if (y2 < y1)
    {
        if (x2 < x1)
        {
            for (int i = x1 - 1, z = y1 - 1; i > x2 && z > y2; i--, z--)
            {
                if (isalpha(board[z][i]))
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = x1 + 1, z = y1 - 1; i < x2 && z > y2; i++, z--)
            {
                if (isalpha(board[z][i]))
                {
                    return false;
                }
            }
        }
    }
    else if (y2 > y1)
    {
        if (x2 < x1)
        {
            for (int i = x1 - 1, z = y1 + 1; i > x2 && z < y2; i--, z++)
            {
                if (isalpha(board[z][i]))
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = x1 + 1, z = y1 + 1; i < x2 && z < y2; i++, z++)
            {
                if (isalpha(board[z][i]))
                {
                    return false;
                }
            }
        }
    }

    return true;
}
bool bishop_has_legal_move(int y1, int x1, bool is_black)
{
    int bishop_moves[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 8; j++)
        {
            int moved_y = y1 + bishop_moves[i][0] * j, moved_x = x1 + bishop_moves[i][1] * j;
            if (moved_y >= 0 && moved_y < 8 && moved_x >= 0 && moved_x < 8)
            {
                if (is_black)
                {
                    if (!(isalpha(board[moved_y][moved_x])))
                    {
                        board[moved_y][moved_x] = 'B';
                        set_square_color(y1, x1);
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
                    else
                    {
                        if (isupper(board[moved_y][moved_x]))
                        {
                            break;
                        }
                        else
                        {
                            board[moved_y][moved_x] = 'B';
                            set_square_color(y1, x1);
                            if (!(is_in_check(is_black)))
                            {
                                reset_position();
                                return true;
                            }
                            else
                            {
                                reset_position();
                                break;
                            }
                        }
                    }
                }

                else
                {
                    if (!(isalpha(board[moved_y][moved_x])))
                    {
                        board[moved_y][moved_x] = 'b';
                        set_square_color(y1, x1);
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
                    else
                    {
                        if (islower(board[moved_y][moved_x]))
                        {
                            break;
                        }
                        else
                        {
                            board[moved_y][moved_x] = 'b';
                            set_square_color(y1, x1);
                            if (!(is_in_check(is_black)))
                            {
                                reset_position();
                                return true;
                            }
                            else
                            {
                                reset_position();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}