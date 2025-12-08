#include "knight.h"
#include <stdbool.h>
#include <stdlib.h>
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