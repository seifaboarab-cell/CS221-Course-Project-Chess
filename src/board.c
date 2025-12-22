#include "board.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'-', '.', '-', '.', '-', '.', '-', '.'},
    {'.', '-', '.', '-', '.', '-', '.', '-'},
    {'-', '.', '-', '.', '-', '.', '-', '.'},
    {'.', '-', '.', '-', '.', '-', '.', '-'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
};
char capture[2][15] = {{'.', '.', '.', '.', '.', '.', '.', '.'}, {'-', '-', '-', '-', '-', '-', '-', '-'}};
int num_capture[2] = {0, 0};
int king_location[2][2] = {{0, 4}, {7, 4}};
bool king_moved[2] = {false, false};
bool a_rook_moved[2] = {false, false};
bool h_rook_moved[2] = {false, false};
int en_passant_flags[2][8] = {{-1, -1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1, -1}};
int draw_flag = -1;
int half_turn = -1;
int turn = 1;
int player_number = 0;

typedef struct Position
{
    char board[8][8];
    char capture[2][15];
    int num_capture[2];
    int king_location[2][2];
    bool king_moved[2];
    bool a_rook_moved[2];
    bool h_rook_moved[2];
    int en_passant_flags[2][8];
    int draw_flag;
    int half_turn;
    int turn;
    int player_number;
    struct Position *previous_position;
} Position;

Position *current_position = NULL;
Position *redo_stack = NULL;

void display_board(bool is_black)
{
    int i, j, k;
    if (is_black)
    {
        printf("captured black piece {");
        for (int z = 0; z < num_capture[is_black]; z++)
        {
            printf(" %c ,", capture[is_black][z]);
        }
    }
    else
    {
        printf("captured white piece {");
        for (int z = 0; z < num_capture[is_black]; z++)
        {
            printf(" %c ,", capture[is_black][z]);
        }
    }
    printf("}\n\n");
    printf(" ");
    for (i = 0; i <= 7; i++)
    {
        if (!is_black)
        {
            printf("    %c", (char)(i + 'A'));
        }
        else
        {
            printf("    %c", (char)(7 - i + 'A'));
        }
    }
    printf("\n");
    if (is_black)
    {
        for (k = 0; k < 8; k++)
        {
            printf("  ");
            for (i = 0; i < 42; i++)
            {
                printf("-");
            }
            printf("\n");
            printf("%d ", k + 1);

            for (j = 7; j >= 0; j--)
            {
                printf("|| %c ", board[k][j]);
            }

            printf("|| %d\n", k + 1);
        }
    }
    else
    {
        for (k = 0; k < 8; k++)
        {
            printf("  ");
            for (i = 0; i < 42; i++)
            {
                printf("-");
            }
            printf("\n");
            printf("%d ", 8 - k);

            for (j = 0; j <= 7; j++)
            {
                printf("|| %c ", board[7 - k][j]);
            }

            printf("|| %d\n", 8 - k);
        }
    }
    printf("  ");
    for (i = 0; i < 42; i++)
    {
        printf("-");
    }
    printf("\n");

    printf(" ");
    for (i = 0; i <= 7; i++)
    {
        if (!is_black)
        {
            printf("    %c", (char)(i + 'A'));
        }
        else
        {
            printf("    %c", (char)(7 - i + 'A'));
        }
    }
    printf("\n\n");
    if (is_black)
    {

        printf("captured white piece {");
        for (int z = 0; z < num_capture[!is_black]; z++)
        {
            printf(" %c ,", capture[!is_black][z]);
        }
        printf("}\n\n");
    }
    else
    {
        printf("captured black piece {");
        for (int z = 0; z < num_capture[!is_black]; z++)
        {
            printf(" %c ,", capture[!is_black][z]);
        }
        printf("}\n\n");
    }
    printf("\n\n\n");
}

void set_square_color(int y, int x)
{
    if (((x + y) & 1) == 0)
    {
        board[y][x] = '-';
    }
    else
    {
        board[y][x] = '.';
    }
}

bool is_in_check(bool is_black)
{
    int y = king_location[is_black][0], x = king_location[is_black][1];
    char queen = is_black ? 'q' : 'Q', rook = is_black ? 'r' : 'R', bishop = is_black ? 'b' : 'B',
         knight = is_black ? 'n' : 'N', pawn = is_black ? 'p' : 'P', king = is_black ? 'k' : 'K';
    int rook_moves[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int bishop_moves[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int knight_moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};
    int king_moves[8][2] = {{1, 1}, {1, 0}, {1, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {0, -1}};
    int pawn_moves[2][2] = {{1, 1}, {1, -1}};
    if (is_black)
    {
        pawn_moves[0][0] = -1;
        pawn_moves[1][0] = -1;
    }

    for (int i = 0; i < 8; i++)
    {
        int moved_y = y + knight_moves[i][0], moved_x = x + knight_moves[i][1];
        if (moved_y >= 0 && moved_y < 8 && moved_x >= 0 && moved_x < 8)
            if (board[moved_y][moved_x] == knight)
                return true;

        moved_y = y + king_moves[i][0], moved_x = x + king_moves[i][1];
        if (moved_y >= 0 && moved_y < 8 && moved_x >= 0 && moved_x < 8)
            if (board[moved_y][moved_x] == king)
                return true;
    }

    for (int i = 0; i < 2; i++)
    {
        int moved_y = y + pawn_moves[i][0], moved_x = x + pawn_moves[i][1];
        if (moved_y >= 0 && moved_y < 8 && moved_x >= 0 && moved_x < 8)
            if (board[moved_y][moved_x] == pawn)
                return true;
    }
    for (int i = 0; i < 4; i++)
    {

        for (int j = 1; j < 8; j++)
        {
            int moved_y = y + rook_moves[i][0] * j, moved_x = x + rook_moves[i][1] * j;
            if (moved_y < 0 || moved_y > 7 || moved_x < 0 || moved_x > 7)
                break;
            if (board[moved_y][moved_x] == rook || board[moved_y][moved_x] == queen)
                return true;
            if (isalpha(board[moved_y][moved_x]))
                break;
        }
        for (int j = 1; j < 8; j++)
        {
            int moved_y = y + bishop_moves[i][0] * j, moved_x = x + bishop_moves[i][1] * j;
            if (moved_y < 0 || moved_y > 7 || moved_x < 0 || moved_x * j > 7)
                break;
            if (board[moved_y][moved_x] == bishop || board[moved_y][moved_x] == queen)
                return true;
            if (isalpha(board[moved_y][moved_x]))
                break;
        }
    }
    return false;
}

void free_stack(Position *p)
{
    if (p == NULL)
        return;
    free_stack(p->previous_position);
    free(p);
}

bool commit_position()
{
    Position *temp = malloc(sizeof(Position));
    if (temp == NULL)
        return false;
    temp->previous_position = current_position;
    current_position = temp;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            current_position->board[i][j] = board[i][j];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
            current_position->en_passant_flags[i][j] = en_passant_flags[i][j];

        for (int j = 0; j < 2; j++)
            current_position->king_location[i][j] = king_location[i][j];
        current_position->king_moved[i] = king_moved[i];
        current_position->a_rook_moved[i] = a_rook_moved[i];
        current_position->h_rook_moved[i] = h_rook_moved[i];
        current_position->num_capture[i] = num_capture[i];

        for (int j = 0; j < num_capture[i]; j++)
            current_position->capture[i][j] = capture[i][j];
    }
    draw_flag++;
    half_turn++;
    current_position->draw_flag = draw_flag;
    current_position->half_turn = half_turn;
    current_position->turn = turn;
    current_position->player_number = player_number;
    free_stack(redo_stack);
    redo_stack = NULL;
    return true;
}

void reset_position()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = current_position->board[i][j];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 8; j++)
            en_passant_flags[i][j] = current_position->en_passant_flags[i][j];

        for (int j = 0; j < 2; j++)
            king_location[i][j] = current_position->king_location[i][j];
        king_moved[i] = current_position->king_moved[i];
        a_rook_moved[i] = current_position->a_rook_moved[i];
        h_rook_moved[i] = current_position->h_rook_moved[i];
        num_capture[i] = current_position->num_capture[i];

        for (int j = 0; j < num_capture[i]; j++)
            capture[i][j] = current_position->capture[i][j];
    }
    draw_flag = current_position->draw_flag;
    half_turn = current_position->half_turn;
}

bool undo()
{
    if (current_position->previous_position == NULL)
        return false;
    Position *temp = current_position;
    current_position = temp->previous_position;
    temp->previous_position = redo_stack;
    redo_stack = temp;
    reset_position();
    return true;
}
bool redo()
{
    if (redo_stack == NULL)
        return false;
    Position *temp = redo_stack;
    redo_stack = temp->previous_position;
    temp->previous_position = current_position;
    current_position = temp;
    reset_position();
    return true;
}

void free_game()
{
    free_stack(current_position);
    current_position = NULL;
    free_stack(redo_stack);
    redo_stack = NULL;
}
