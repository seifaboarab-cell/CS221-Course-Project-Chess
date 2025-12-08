#include "board.h"

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

typedef struct Position{
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
}Position;

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

bool commit_position(){
    Position *temp = malloc(sizeof(Position));
    if (temp == NULL)
        return false;
    temp->previous_position = current_position;
    current_position = temp;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            current_position->board[i][j] = board[i][j];
    for (int i = 0; i < 2; i++){
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

void reset_postion(){
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            board[i][j] = current_position->board[i][j];
    for (int i = 0; i < 2; i++){
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
