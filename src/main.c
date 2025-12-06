#include "board.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

bool move(int y1, int x1, int y2, int x2, bool is_black);
// Note: Parameters are ordered as (y, x) because the board is accessed as board[y][x].
// We should keep this ordering consistent throughout the code.
bool promotion(int y1, int x1, int y2, int x2, bool is_black);
// this funcion start after move function and validility function of (king)
int main()
{
    char *players[] = {"White", "Black"};
    for (int i = 0;;)
    {
        int turn = i / 2 + 1, player_number = i % 2;
        char input[10];
        display_board();
        printf("%d-%s: ", turn, players[player_number]);
        scanf("%s", input);
        int x1 = toupper(input[0]) - 'A', y1 = input[1] - '1', x2 = toupper(input[2]) - 'A', y2 = input[3] - '1';
        if (!move(y1, x1, y2, x2, player_number))
            continue;

        i++;
    }
    return 0;
}

bool move(int y1, int x1, int y2, int x2, bool is_black)
{
    if (x1 > 7 || x1 < 0 || x2 > 7 || x2 < 0 || y1 > 7 || y1 < 0 || y2 > 7 || y2 < 0)
    {
        printf("Invalid input!\n");
        return false;
    }
    if (!islower(board[y1][x1]) && !is_black || !isupper(board[y1][x1]) && is_black)
    {
        printf("You don't have a piece on this square.\n");
        return false;
    }
    if (islower(board[y2][x2]) && !is_black || isupper(board[y2][x2]) && is_black)
    {
        printf("You cannot capture one of your own pieces.\n");
        return false;
    }
    bool can_move = true;
    switch (board[y1][x1])
    {
    case 'k':
    case 'K':
        // can_move = can_move_king(y1, x1, y2, x2);
        break;
    case 'q':
    case 'Q':
        // can_move = can_move_queen(y1, x1, y2, x2);
        break;
    case 'r':
    case 'R':
        // can_move = can_move_rook(y1, x1, y2, x2);
        break;
    case 'b':
    case 'B':
        // can_move = can_move_bishop(y1, x1, y2, x2);
        break;
    case 'n':
    case 'N':
        // can_move = can_move_knight(y1, x1, y2, x2);
        break;
    case 'p':
    case 'P':
        // can_move = can_move_pawn(y1, x1, y2, x2, is_black);
        break;
    }
    if (!can_move)
    {
        printf("Illegal move!\n");
        return false;
    }
    board[y2][x2] = board[y1][x1];
    set_square_color(y1, x1);
    return true;
}
