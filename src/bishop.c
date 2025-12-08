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
            for (int i = x1 + 1, z = y1 - 1; i<x2 && z> y2; i++, z--)
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