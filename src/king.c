#include "king.h"
#include <stdlib.h>

bool can_move_king(int y1, int x1, int y2, int x2){
    return abs(y2 - y1) <= 1 && abs(x2 - x1) <= 1;
}