#include "queen.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool can_move_queen(int y1, int x1, int y2, int x2)
{
    if ((can_move_bishop(y1, x1, y2, x2)) || (can_move_rook(y1, x1, y2, x2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool queen_has_legal_move(int y1, int x1, bool is_black)
{
    if ((bishop_has_legal_move(y1, x1, is_black)) || (rook_has_legal_move(y1, x1, is_black)))
    {
        return true;
    }
    return false;
}