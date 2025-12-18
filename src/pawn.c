#include "pawn.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

bool can_move_pawn(int y1, int x1, int y2, int x2, bool is_black, char promotion_piece)
{
    int promotion_row = is_black ? 0 : 7;
    if ((promotion_piece == '\0' && y2 == promotion_row) || (promotion_piece != '\0' && y2 != promotion_row))
        return false;
    if (promotion_piece != 'q' && promotion_piece != 'r' && promotion_piece != 'b' && promotion_piece != 'n' && promotion_piece != '\0')
        return false;
    if (promotion_piece != '\0')
        board[y1][x1] = is_black ? toupper(promotion_piece) : promotion_piece;
    if (y1 == 1 && y2 == 3 && !is_black && x1 == x2 && !isalpha(board[y1 + 1][x1]) && !isupper(board[y1 + 2][x1]))
    {
        en_passant_flags[is_black][x1] = half_turn;
        return true;
    }
    else if (y1 == 6 && y2 == 4 && is_black && x1 == x2 && !isalpha(board[y1 - 1][x1]) && !islower(board[y1 - 2][x1]))
    {
        en_passant_flags[is_black][x1] = half_turn;
        return true;
    }
    else if ((y2 - y1 == 1) && !is_black && x2 == x1 && !isupper(board[y2][x2]))
    {
        return true;
    }
    else if ((y2 - y1 == 1) && !is_black && (abs(x2 - x1) == 1) && isalpha(board[y2][x2]))
    {
        return true;
    }

    else if ((y2 - y1 == -1) && is_black && x2 == x1 && !islower(board[y2][x2]))
    {
        return true;
    }
    else if ((y2 - y1 == -1) && is_black && (abs(x2 - x1) == 1) && isalpha(board[y2][x2]))
    {
        return true;
    }

    board[y1][x1] = is_black ? 'P' : 'p';
    return false;
}

bool en_passant(int y1, int x1, int y2, int x2, bool is_black)
{
    int row1 = is_black ? 3 : 4, row2 = is_black ? 2 : 5;
    if (y1 != row1 || y2 != row2 || abs(x1 - x2) != 1)
        return false;
    if (half_turn - en_passant_flags[!is_black][x2] != 1)
        return false;
    board[y2][x2] = board[y1][x1];
    capture[!is_black][num_capture[!is_black]++] = board[y1][x2];
    set_square_color(y1, x2);
    set_square_color(y1, x1);
    if (is_in_check(is_black))
    {
        reset_position();
        return false;
    }
    return true;
}
bool pawn_has_legal_move(int y1, int x1, bool is_black)
{

    if (is_black)
    {
        if ((en_passant(y1, x1, y1 - 1, x1 - 1, is_black)) || (en_passant(y1, x1, y1 - 1, x1 + 1, is_black)))
        {
            reset_position();
            return true;
        }
    }
    else
    {
        if ((en_passant(y1, x1, y1 + 1, x1 - 1, is_black)) || (en_passant(y1, x1, y1 + 1, x1 + 1, is_black)))
        {
            reset_position();
            return true;
        }
    }
    int y_check = is_black ? y1 - 1 : y1 + 1;
    if ((!is_black && !(isalpha(board[y_check][x1]))) || (is_black && !(isalpha(board[y_check][x1]))))
    {
        board[y_check][x1] = is_black ? 'P' : 'p';
        set_square_color(y1, x1);
        if (!is_in_check(is_black))
        {
            reset_position();
            return true;
        }
        else
        {
            reset_position();
        }
    }
    if (1)
    {
        int y_check = is_black ? y1 - 2 : y1 + 2;
        int y_check2 = is_black ? y1 - 1 : y1 + 1;
        if ((y1 == 1 && !is_black && !(isalpha(board[y_check][x1])) && !isalpha(board[y_check2][x1])) || (y1 == 6 && is_black && !(isalpha(board[y_check][x1])) && !isalpha(board[y_check2][x1])))
        {
            board[y_check][x1] = is_black ? 'P' : 'p';
            set_square_color(y1, x1);
            if (!is_in_check(is_black))
            {
                reset_position();
                return true;
            }
            else
            {
                reset_position();
            }
        }
    }
    if (!is_black)
    {
        if (isupper(board[y1 + 1][x1 - 1]))
        {
            board[y1 + 1][x1 - 1] = 'p';
            set_square_color(y1, x1);
            if (!is_in_check(is_black))
            {
                reset_position();
                return true;
            }
            else
            {
                reset_position();
            }
        }
        if (isupper(board[y1 + 1][x1 + 1]))
        {
            board[y1 + 1][x1 + 1] = 'p';
            set_square_color(y1, x1);
            if (!is_in_check(is_black))
            {
                reset_position();
                return true;
            }
            else
            {
                reset_position();
            }
        }
    }
    else
    {
        if (islower(board[y1 - 1][x1 - 1]))
        {
            board[y1 - 1][x1 - 1] = 'P';
            set_square_color(y1, x1);
            if (!is_in_check(is_black))
            {
                reset_position();
                return true;
            }
            else
            {
                reset_position();
            }
        }
        if (islower(board[y1 - 1][x1 + 1]))
        {
            board[y1 - 1][x1 + 1] = 'P';
            set_square_color(y1, x1);
            if (!is_in_check(is_black))
            {
                reset_position();
                return true;
            }
            else
            {
                reset_position();
            }
        }
    }
    return false;
}
