#include "pawn.h"
#include <ctype.h>
#include <stdlib.h>

bool can_move_pawn(y1, x1, y2, x2, is_black)
{
    if (x1 == 1 && x2 == 3 && !is_black && y1 == y2 && !isupper(board[y1][x1 + 1]) && !islower(board[y1][x1 + 1]) && !isupper(board[y1][x1 + 2]))
    {
        return true;
    }
    else if (x1 == 6 && x2 == 4 && is_black && y1 == y2 && !isupper(board[y1][x1 - 1]) && !islower(board[y1][x1 - 1]) && !islower(board[y1][x1 - 2]))
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