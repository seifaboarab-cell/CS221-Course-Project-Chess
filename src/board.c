#include "board.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'-', ' ', '-', ' ', '-', ' ', '-', ' '},
    {' ', '-', ' ', '-', ' ', '-', ' ', '-'},
    {'-', ' ', '-', ' ', '-', ' ', '-', ' '},
    {' ', '-', ' ', '-', ' ', '-', ' ', '-'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
};
char capture[2][15];
int num_capture[2] = {0, 0};
int king_location[2][2] = {{0, 4}, {7, 4}};
bool king_moved[2] = {false, false};
bool a_rook_moved[2] = {false, false};
bool h_rook_moved[2] = {false, false};
int en_passant_flags[2][8] = {{-1, -1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1, -1}};

int half_turn = 0;
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
    int half_turn;
    int turn;
    int player_number;
    struct Position *previous_position;
} Position;

Position *current_position = NULL;

void display_board()
{
    int i, j, k;

    printf("captured white piece {");
    for (int z = 0; z < num_capture[0]; z++)
    {
        printf(" %c ,", capture[0][z]);
    }
    printf("}\n\n");

    printf(" ");
    for (i = 7; i >= 0; i--)
        printf("    %c", (char)(i + 'A'));
    printf("\n");

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

    printf("  ");
    for (i = 0; i < 42; i++)
    {
        printf("-");
    }
    printf("\n");

    printf(" ");
    for (i = 7; i >= 0; i--)
        printf("    %c", (char)(i + 'A'));
    printf("\n\n");
    printf("captured black piece {");
    for (int z = 0; z < num_capture[1]; z++)
    {
        printf(" %c ,", capture[1][z]);
    }
    printf("}\n\n\n");
}

void set_square_color(int y, int x)
{
    if (((x + y) & 1) == 0)
    {
        board[y][x] = '-';
    }
    else
    {
        board[y][x] = ' ';
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
    current_position->half_turn = half_turn;
    current_position->turn = turn;
    current_position->player_number = player_number;
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
}
