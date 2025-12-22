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

#define MAX_INPUT_SIZE 10

bool move(int y1, int x1, int y2, int x2, bool is_black, char promotion_piece);
// Note: Parameters are ordered as (y, x) because the board is accessed as board[y][x].
// We should keep this ordering consistent throughout the code.
bool has_legal_move(bool is_black);
bool read_input(char s[]);
int save_game();
void load_game(char index[]);
bool draw();

int main()
{
    char *players[] = {"White", "Black"};
    char input[MAX_INPUT_SIZE + 1] = "";
    printf("Load a saved game or start a new one? (load/new)\n");
    if (read_input(input))
    {
        if (!strcmp(input, "load") || !strcmp(input, "l"))
        {
            printf("Enter the game number: ");
            read_input(input);
            load_game(input);
        }
    }
    if (!commit_position())
    {
        printf("ERROR: failed to allocate memory\n");
        free_game();
        return 1;
    }
    for (;;)
    {
        turn = half_turn / 2 + 1;
        player_number = half_turn % 2;
        display_board(player_number);
        bool in_check = is_in_check(player_number);
        bool can_move = has_legal_move(player_number);
        if (!can_move)
        {
            if (in_check)
                printf("Checkmate!\n%s won.", players[!player_number]);
            else
                printf("Draw!\n");
            break;
        }
        if (draw())
        {
            printf("Draw!\n");
            break;
        }
        if (in_check)
            printf("Check!\n");
        printf("%d-%s: ", turn, players[player_number]);

        if (!read_input(input))
        {
            printf("Invalid input!\n");
        }
        else if (!strcmp(input, "undo") || !strcmp(input, "u"))
        {
            if (!undo())
                printf("Undo is not available right now.\n");
        }
        else if (!strcmp(input, "redo") || !strcmp(input, "r"))
        {
            if (!redo())
                printf("Redo is not available right now.\n");
        }
        else if (!strcmp(input, "quit") || !strcmp(input, "q"))
        {
            int index = save_game();
            if (index == -1)
                printf("Could not save your game.\n");
            else
                printf("Your game number is: %d\n", index);
            break;
        }
        else if (strlen(input) < 4 || strlen(input) > 5)
        {
            printf("Invalid input!\n");
        }
        else
        {
            int x1 = input[0] - 'a', y1 = input[1] - '1', x2 = input[2] - 'a', y2 = input[3] - '1';
            if (!move(y1, x1, y2, x2, player_number, input[4]))
            {
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
        return true;
        ;
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
            if (!commit_position())
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
            draw_flag = -1;
            if (!commit_position())
                return false;
            return true;
        }
        can_move = can_move_pawn(y1, x1, y2, x2, is_black, promotion_piece);
        if (can_move)
            draw_flag = -1;
        break;
    }
    if (!can_move)
    {
        printf("Illegal move!\n");
        return true;
    }
    if (isalpha(board[y2][x2]))
    {
        capture[!is_black][num_capture[!is_black]++] = board[y2][x2];
        draw_flag = -1;
    }
    board[y2][x2] = board[y1][x1];
    set_square_color(y1, x1);

    if (is_in_check(is_black))
    {
        printf("Illegal move: you cannot move your king into check or leave him in check.\n");
        reset_position();
        return true;
    }
    if (!commit_position())
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
                    has_legal_move = king_has_legal_move(i, j, is_black);
                    break;
                case 'q':
                case 'Q':
                    has_legal_move = queen_has_legal_move(i, j, is_black);
                    break;
                case 'r':
                case 'R':
                    has_legal_move = rook_has_legal_move(i, j, is_black);

                    break;
                case 'b':
                case 'B':
                    has_legal_move = bishop_has_legal_move(i, j, is_black);
                    break;
                case 'n':
                case 'N':
                    has_legal_move = knight_has_legal_move(i, j, is_black);
                    break;
                case 'p':
                case 'P':
                    has_legal_move = pawn_has_legal_move(i, j, is_black);
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
    while (c != '\n' && c != EOF)
    {
        if (i < MAX_INPUT_SIZE)
            s[i++] = tolower((unsigned char)c);
        else
            valid = false;
        c = getchar();
    }
    s[i] = '\0';
    return valid;
}

int save_game()
{
    FILE *index_file = fopen("saves/Index", "r");
    if (index_file == NULL)
    {
        perror("failed to save file");
        return -1;
    }
    int index = 0;
    fread(&index, sizeof(int), 1, index_file);
    fclose(index_file);

    char file_name[20];
    sprintf(file_name, "saves/%d.txt", index);
    FILE *file_ptr = fopen(file_name, "w");

    if (file_ptr == NULL)
    {
        perror("failed to save file");
        return -1;
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            fprintf(file_ptr, "%c", board[i][j]);
        }
        fprintf(file_ptr, "\n");
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            fprintf(file_ptr, "%c", capture[i][j]);
        }
        fprintf(file_ptr, "\n");
    }
    fprintf(file_ptr, "%d %d\n", num_capture[0], num_capture[1]);
    for (int i = 0; i < 2; i++)
    {
        fprintf(file_ptr, "%d %d", king_location[i][0], king_location[i][1]);
        fprintf(file_ptr, "\n");
    }
    fprintf(file_ptr, "%d %d\n", king_moved[0], king_moved[1]);
    fprintf(file_ptr, "%d %d\n", a_rook_moved[0], a_rook_moved[1]);
    fprintf(file_ptr, "%d %d\n", h_rook_moved[0], h_rook_moved[1]);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            fprintf(file_ptr, "%d ", en_passant_flags[i][j]);
        }
        fprintf(file_ptr, "\n");
    }
    fprintf(file_ptr, "%d\n", draw_flag - 1);
    fprintf(file_ptr, "%d\n", half_turn - 1);
    
    fclose(file_ptr);
    index++;
    index_file = fopen("saves/Index", "w");
    if (index_file == NULL)
    {
        perror("failed to save file");
        return -1;
    }
    fwrite(&index, sizeof(int), 1, index_file);
    fclose(index_file);
    return index - 1;
}

void load_game(char index[])
{
    char file_name[20];
    sprintf(file_name, "saves/%s.txt", index);
    FILE *file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL)
    {
        perror("there is not file with this name");
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            fscanf(file_ptr, " %c", &board[i][j]);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            fscanf(file_ptr, " %c", &capture[i][j]);
        }
    }
    fscanf(file_ptr, "%d %d", &num_capture[0], &num_capture[1]);
    for (int i = 0; i < 2; i++)
    {
        fscanf(file_ptr, "%d %d", &king_location[i][0], &king_location[i][1]);
    }
    fscanf(file_ptr, "%d %d", &king_moved[0], &king_moved[1]);
    fscanf(file_ptr, "%d %d", &a_rook_moved[0], &a_rook_moved[1]);
    fscanf(file_ptr, "%d %d", &h_rook_moved[0], &h_rook_moved[1]);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            fscanf(file_ptr, "%d ", &en_passant_flags[i][j]);
        }
    }
    fscanf(file_ptr, "%d", &draw_flag);
    fscanf(file_ptr, "%d", &half_turn);
    
    fclose(file_ptr);
}
bool draw()
{
    if (draw_flag >= 100)
        return true;
    int count_white = 0, count_black = 0;
    int num_knights[2] = {0, 0};
    int num_bishops[2] = {0, 0};
    char color_square[2][10] = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};
    if (num_capture[0] == 15 && num_capture[1] == 15)
    {
        return true;
    }
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int k = (islower(board[i][j])) ? 0 : 1;
            if (board[i][j] == 'r' || board[i][j] == 'R' || board[i][j] == 'q' || board[i][j] == 'Q' || board[i][j] == 'p' || board[i][j] == 'P')
            {
                return false;
            }
            else if (board[i][j] == 'b' || board[i][j] == 'B')
            {
                int l = (board[i][j] == 'b') ? num_bishops[0] : num_bishops[1];
                color_square[k][l] = ((i + j) % 2 == 0) ? '-' : '.';
                num_bishops[k] += 1;
                
            }
            else if (board[i][j] == 'n' || board[i][j] == 'N')
            {
                num_knights[k] += 1;
            }
        }
    }
    if ((num_knights[0] < 2 || (num_knights[1] < 2)) && num_bishops[0] == 0 && num_bishops[1] == 0)
    {
        return true;
    }
    else if ((num_bishops[0] > 0 || num_bishops[1] > 0) && num_knights[0] == 0 && num_knights[1] == 0)
    {
        bool different_square = false;
        for (int w = 0; w < num_bishops[0] - 1; w++)
        {
            if (color_square[0][w] != color_square[0][w + 1])
            {
                different_square = true;
                break;
            }
        }
        for (int w = 0; w < num_bishops[1] - 1; w++)
        {
            if (color_square[1][w] != color_square[1][w + 1])
            {
                different_square = true;
                break;
            }
        }
        if (different_square)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}