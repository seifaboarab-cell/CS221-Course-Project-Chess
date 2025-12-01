#include "board.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"

#include <ctype.h>
#include <stdio.h>

int main(){
    char *players[] = {"White", "Black"};
    for (int i = 0;;){
        int turn = i / 2 + 1, player_number = i % 2;
        char input[10];
        display();
        printf("%d-%s: ", turn, players[player_number]);
        scanf("%s", input);
        int x1 = toupper(input[0]) - 'A', y1 = input[1] - '1', x2 = toupper(input[2]) - 'A', y2 = input[3] - '1';
        //if (!move(x1, y1, x2, y2, player_number))
            //continue;
        i++;
    }
    return 0;
}