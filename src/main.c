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
#include <string.h>

#define MAX_INPUT_SIZE 5

bool move(int y1, int x1, int y2, int x2, bool is_black, char promotion_piece);
// Note: Parameters are ordered as (y, x) because the board is accessed as board[y][x].
// We should keep this ordering consistent throughout the code.
bool has_legal_move(bool is_black);
bool read_input(char s[]);

int main()
{
    char *players[] = {"White", "Black"};
    commit_position();
    for (;;)
    {
        turn = half_turn / 2 + 1;
        player_number = half_turn % 2;
        display_board();
        bool in_check = is_in_check(player_number), can_move = has_legal_move(player_number);
        // if (!can_move)
        // {
        //     if(in_check)
        //         printf("Checkmate!\n%s won.", players[!player_number]);
        //     else
        //         printf("Draw by stalemate.\n");
        //     free_game();
        //     return 0;
        // }
        if (in_check)
            printf("Check!\n");
        printf("%d-%s: ", turn, players[player_number]);        
        char input[MAX_INPUT_SIZE + 1] = "";

        if (!read_input(input))
        {
            printf("Invalid input!\n");
        }
        else if (!strcmp(input, "undo") || !strcmp(input, "u"))
        {
            if (!undo())
                printf("Undo is not available right now.\n");

        }else if (!strcmp(input, "redo") || !strcmp(input, "r"))
        {
            if (!redo())
                printf("Redo is not available right now.\n");    
        }else if (!strcmp(input, "quit") || !strcmp(input, "q"))
        {
            //TODO
            break;
        }else if (strlen(input) < 4)
        {
            printf("Invalid input!\n");
        }else
        {
            int x1 = input[0] - 'a', y1 = input[1] - '1', x2 = input[2] - 'a', y2 = input[3] - '1';
            if(!move(y1, x1, y2, x2, player_number, input[4])){
                // I changed move() function to make it return flase only when memory allocation fails.
                // Otherwise it will return true and commit the move if it's legal or else reset the position.
                printf("ERROR: failed to allocate memory\n");
                free_game();
                return 1;
            }  
        }
    }
    free_game();
    return 0;
}

bool move(int y1, int x1, int y2, int x2, bool is_black, char promotion_piece)
{
    if (x1 > 7 || x1 < 0 || x2 > 7 || x2 < 0 || y1 > 7 || y1 < 0 || y2 > 7 || y2 < 0)
    {
        printf("Invalid input!\n");
        return true;
    }
    if ((!islower(board[y1][x1]) && !is_black) || (!isupper(board[y1][x1]) && is_black))
    {
        printf("You don't have a piece on this square.\n");
        return true;
    }
    if ((islower(board[y2][x2]) && !is_black) || (isupper(board[y2][x2]) && is_black))
    {
        printf("You cannot capture one of your own pieces.\n");
        return true;
    }
    if (promotion_piece != '\0' && tolower(board[y1][x1]) != 'p')
    {
        printf("Illegal move!\n");
        return true;;
    }
    bool can_move = true;
    switch (board[y1][x1])
    {
    case 'k':
    case 'K':
        if (castle(y1, x1, y2, x2, is_black))
        {
            // Unlike other move functions, castle() and en_passant() implicitly perform
            // the move on the board and verify that the king is not left in check
            // and if the king is in check, they reset the position.
            // That is why, if they return true, the function can return true directly.
            if(!commit_position())
                return false;
            return true;
        }
        can_move = can_move_king(y1, x1, y2, x2);
        if (can_move)
        {
            king_location[is_black][0] = y2;
            king_location[is_black][1] = x2;
            king_moved[is_black] = true;
        }
        break;
    case 'q':
    case 'Q':
        can_move = can_move_queen(y1, x1, y2, x2);
        break;
    case 'r':
    case 'R':
        can_move = can_move_rook(y1, x1, y2, x2);
        if (can_move && y1 == 0 + is_black * 7)
        {
            if (x1 == 0)
                a_rook_moved[is_black] = true;
            else if (x1 == 7)
                h_rook_moved[is_black] = true;
        }
        break;
    case 'b':
    case 'B':
        can_move = can_move_bishop(y1, x1, y2, x2);
        break;
    case 'n':
    case 'N':
        can_move = can_move_knight(y1, x1, y2, x2);
        break;
    case 'p':
    case 'P':
        if (en_passant(y1, x1, y2, x2, is_black))
        {
            // Unlike other move functions, castle() and en_passant() implicitly perform
            // the move on the board and verify that the king is not left in check
            // and if the king is in check, they reset the position.
            // That is why, if they return true, the function can return true directly.
            if(!commit_position())
                return false;
            return true;
        }
        can_move = can_move_pawn(y1, x1, y2, x2, is_black, promotion_piece);
        break;
    }
    if (!can_move)
    {
        printf("Illegal move!\n");
        return true;
    }
    if (isalpha(board[y2][x2]))
        capture[!is_black][num_capture[!is_black]++] = board[y2][x2];
    board[y2][x2] = board[y1][x1];
    set_square_color(y1, x1);

    if (is_in_check(is_black))
    {
        printf("Illegal move: you cannot move your king into check or leave him in check.\n");
        reset_position();
        return true;
    }
    if(!commit_position())
        return false;
    return true;
}

bool has_legal_move(bool is_black)
{
    bool has_legal_move = false;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((islower(board[i][j]) && !is_black) || (isupper(board[i][j]) && is_black))
            {
                switch (board[i][j])
                {
                case 'k':
                case 'K':
                    //has_legal_move = king_has_legal_move(i, j, is_black);
                    break;
                case 'q':
                case 'Q':
                    //has_legal_move = queen_has_legal_move(i, j, is_black);
                    break;
                case 'r':
                case 'R':
                    //has_legal_move = rook_has_legal_move(i, j, is_black);

                    break;
                case 'b':
                case 'B':
                    //has_legal_move = bishop_has_legal_move(i, j, is_black);
                    break;
                case 'n':
                case 'N':
                    //has_legal_move = knight_has_legal_move(i, j, is_black);
                    break;
                case 'p':
                case 'P':
                    //has_legal_move = pawn_has_legal_move(i, j, is_black);
                    break;
                }
            }
            if (has_legal_move)
            {
                return true;
            }
        }
    }
    return false;
}

bool read_input(char s[])
{
    int c = getchar(), i = 0;
    bool valid = true;
    while (c !='\n' && c != EOF)
    {
        if(i < MAX_INPUT_SIZE)
            s[i++] = tolower((unsigned char) c);
        else
            valid = false;
        c = getchar();
    }
    s[i] = '\0';
    return valid;
}