#include "rook.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
bool can_move_rook(int y1, int x1, int y2, int x2)
{
    if (!((y2 == y1 && x2 - x1 != 0) || (x2 == x1 && y2 - y1 != 0)))
    {
        return false;
    }
    else if ((y2 == y1 && x2 - x1 != 0))
    {
        if (x2 < x1)
        {
            for (int i = x1 - 1; i > x2; i--)
            {
                if (isalpha(board[y1][i]))
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = x1 + 1; i < x2; i++)
            {
                if (isalpha(board[y1][i]))
                {
                    return false;
                }
            }
        }
    }
    else if ((x2 == x1 && y2 - y1 != 0))
    {
        if (y2 < y1)
        {
            for (int i = y1 - 1; i > y2; i--)
            {
                if (isalpha(board[i][x1]))
                {
                    return false;
                }
            }
        }
        else
        {
            for (int i = y1 + 1; i < y2; i++)
            {
                if (isalpha(board[i][x1]))
                {
                    return false;
                }
            }
        }
    }
    return true;
}