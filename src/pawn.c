#include "pawn.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

bool can_move_pawn(int y1, int x1, int y2, int x2, bool is_black)
{
    if (y1 == 1 && y2 == 3 && !is_black && x1 == x2 && !isalpha(board[y1 + 1][x1]) && !isupper(board[y1 + 2][x1]))
    {
        return true;
    }
    else if (y1 == 6 && y2 == 4 && is_black && x1 == x2 && !isalpha(board[y1 - 1][x1]) && !islower(board[y1 - 2][x1]))
    {
        return true;
    }
    else if ((y2 - y1 == 1) && !is_black && x2 == x1 && !isupper(board[y2][x2]))
    {
        return true;
    }
    else if ((y2 - y1 == 1) && !is_black && (abs(x2 - x1) == 1))
    {
        return true;
    }

    else if ((y2 - y1 == -1) && is_black && x2 == x1 && !islower(board[y2][x2]))
    {
        return true;
    }
    else if ((y2 - y1 == -1) && is_black && (abs(x2 - x1) == 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}