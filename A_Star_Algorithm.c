//
//  A_Star_Algorithm.c
//  
//
//  Created by Felix Waldbach on 28.12.18.
//

#include "A_Star_Algorithm.h"
#include <stdio.h>

/* A* Search Algorithm
1.  Initialize the open list
2.  Initialize the closed list
put the starting node on the open
list (you can leave its f at zero)

3.  while the open list is not empty
a) find the node with the least f on
the open list, call it "q"

b) pop q off the open list

c) generate q's 8 successors and set their
parents to q

d) for each successor
i) if successor is the goal, stop search
successor.g = q.g + distance between
successor and q
successor.h = distance from goal to
successor (This can be done using many
           ways, we will discuss three heuristics-
           Manhattan, Diagonal and Euclidean
           Heuristics)

successor.f = successor.g + successor.h

ii) if a node with the same position as
successor is in the OPEN list which has a
lower f than successor, skip this successor

iii) if a node with the same position as
successor  is in the CLOSED list which has
a lower f than successor, skip this successor
otherwise, add  the node to the open list
end (for loop)

e) push q on the closed list
end (while loop)
*/

int main(int argc, char *argv[]){
    FILE *in = stdin;
    Lab_p lab;
    
    if(argc > 2)
    {
        fprintf(stderr, "Error! Usage: %s [<file>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(argc == 2)
    {
        in = fopen(argv[1], "r");
        if(!in)
        {
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    
    lab = generateLab(in);
    printLab(lab);
    exit(EXIT_SUCCESS);
}

Lab_p generateLab(FILE* in)
{
    Lab_p elem = (LAB*) malloc(sizeof(LAB));
    elem->maxrow = 0;
    elem->maxcol = 0;
    int row = 0;
    int col = 0;
    int handle_max_col = 0;
    int empty_line = 0;
    
    char c;
    
    do {
        c = (char)fgetc(in);
        if(c == '\n') {
            empty_line++;
            printf("\n");
            row++;
            col=0;
            if(handle_max_col > elem->maxcol) elem->maxcol = handle_max_col;
            handle_max_col = 0;
            elem->maxrow++;
        } else if (strchr(BF_VALID, c)) {
            empty_line = 0;
            printf("%c", c);
            NODE handle;
            handle.type = c;
            handle.x = row;
            handle.y = col;
            switch(c) {
                case '0':
                    handle.distance = TYPE_COSTS_0;
                    break;
                case '1':
                    handle.distance = TYPE_COSTS_1;
                    break;
                case '2':
                    handle.distance = TYPE_COSTS_2;
                    break;
                case '3':
                    handle.distance = TYPE_COSTS_3;
                    break;
                case '4':
                    handle.distance = TYPE_COSTS_4;
                    break;
                case '5':
                    handle.distance = TYPE_COSTS_5;
                    break;
                default:
                    break;
            }
            elem->lab[row][col++] = handle;
            handle_max_col++;
        }
    } while(c != EOF && elem->maxcol<=MAXCOLS && elem->maxrow<=MAXROWS && empty_line<2);
    return elem;
}


void printLab(Lab_p lab)
{
    system("clear");
    int i, j;
    printf("Lab size: %d x %d\n", lab->maxrow, lab->maxcol);
    printf("LAB TYPES:----------------\n");
    for(i=0; i<=lab->maxrow; i++)
    {
        for(j=0; j<lab->maxcol; j++) {
            printf("[%d, %d: %c] ", lab->lab[i][j].x, lab->lab[i][j].y, lab->lab[i][j].type);
        }
        printf("|\n");
    }
    printf("LAB COSTS:----------------\n");
    for(i=0; i<=lab->maxrow; i++)
    {
        printf("|");
        for(j=0; j<lab->maxcol; j++) {
            if(lab->lab[i][j].distance > 9) printf("%d ", lab->lab[i][j].distance);
            else printf("0%d ", lab->lab[i][j].distance);
        }
        printf("|\n");
    }
    delay(100);
}

void delay(long milliseconds) {
    long pause;
    clock_t now,then;
    
    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

