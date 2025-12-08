#ifndef QUEEN_H
#define QUEEN_H

#include "board.h"
#include "bishop.h"
#include "rook.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool can_move_queen(int y1, int x1, int y2, int x2);

#endif