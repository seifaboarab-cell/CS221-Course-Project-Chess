#include "board.h"

#include <stdio.h>

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

void display()
{
    int i, j, k;

    printf(" ");
    for (i = 0; i < 8; i++)
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

        for (j = 0; j < 8; j++)
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
    for (i = 0; i < 8; i++)
        printf("    %c", (char)(i + 'A'));
    printf("\n");
}

void white_black(int x, int y)
{
    if ((x + y) & 1 == 0)
    {
        board[x][y] == '-';
    }
    else
    {
        board[x][y] == ' ';
    }
}