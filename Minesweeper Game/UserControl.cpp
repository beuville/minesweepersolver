#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <iostream>
#include "UserControl.h"
#include "Gameboard.h"
using namespace std;
/*
* constructor
*/
UserControl::UserControl(){

    int x, y, z;
    
    printf("board height: ");
    fgets(input, 1024, stdin);
    x = atoi(input);
    printf("board width: ");
    fgets(input, 1024, stdin);
    y = atoi(input);
    
    printf("num mines: ");
    fgets(input, 1024, stdin);
    z = atoi(input);
    
    Gameboard *board = new Gameboard(x,y,z);
    //board->generate();
    

    while(1){
        printf(":>: ");
        fgets(input, 1024, stdin);
        command = strtok(input, " \t\n");
        if(strcmp(command, "printP") == 0)
            board->print('p');
        else if(strcmp(command, "printM") == 0)
            board->print('m');
        else if(strcmp(command, "guess") == 0){
            printf("row: ");
            fgets(input, 1024, stdin);
            x = atoi(input);
            printf("column: ");
            fgets(input, 1024, stdin);
            y = atoi(input);
            //board->pickSquare(x, y);
            board->pickSquare(x, y);
        }
        else if(strcmp(command, "pprint") == 0){
            for (int i=0; i<4; i++)
                printf("%d\n", board->getCoordinate('m',i,i));
        }
        else
            printf("nope\n");
    }
}
