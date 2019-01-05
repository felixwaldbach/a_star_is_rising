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

int nodeIndex = 0;

int main(int argc, char *argv[]) {
    FILE *in = stdin;
    Lab_p lab;

    if (argc > 6) {
        fprintf(stderr, "Error! Usage: %s [<file>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 6) {
        in = fopen(argv[1], "r");
        if (!in) {
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }
        startX = atoi(argv[2]);
        startY = atoi(argv[3]);
        goalX = atoi(argv[4]);
        goalY = atoi(argv[5]);
    }

    lab = generateLab(in);
    //int heuristicTest = getManhattanDistance(lab->lab[7][7]);
    //printLab(lab);
    //printf("\nHeuristic Test Value: %d", heuristicTest);

    NODE *test, *test2, *test3;
    test = fillList(1);
    test2 = fillList(2);
    test3 = fillList(3);
    addList(&open_start, test);
    addList(&open_start, test2);
    addList(&open_start, test3);
    printList(open_actual, open_start);
    printf("\n---------");
    deleteList(&open_start, test2);
    printList(open_actual, open_start);

    exit(EXIT_SUCCESS);
}

Lab_p generateLab(FILE *in) {
    Lab_p elem = (LAB *) malloc(sizeof(LAB));
    elem->maxrow = 0;
    elem->maxcol = 0;
    int row = 0;
    int col = 0;
    int handle_max_col = 0;
    int semi_ctr = 0;

    char c;

    do {
        printf("Semi counter: %d\n", semi_ctr);
        c = (char) fgetc(in);
        if (c == '\n') {
            row++;
            col = 0;
            if (handle_max_col > elem->maxcol) elem->maxcol = handle_max_col;
            handle_max_col = 0;
            elem->maxrow++;
        } else if(c == ';') {
            semi_ctr++;
            printf("Semi increasing: %d\n", semi_ctr);
        } else if (strchr(BF_VALID, c)) {
            printf("Character: %c\n", c);
            semi_ctr = 0;
            //printf("%c", c);
            NODE handle;
            handle.type = c;
            handle.x = row;
            handle.y = col;
            switch (c) {
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
    } while (c != EOF && elem->maxcol <= MAXCOLS && elem->maxrow <= MAXROWS && semi_ctr != 14);
    return elem;
}


void printLab(Lab_p lab) {
    system("clear");
    int i, j;
    printf("Lab size: %d x %d\n", lab->maxrow, lab->maxcol);
    printf("LAB TYPES:----------------\n");
    for (i = 0; i <= lab->maxrow; i++) {
        for (j = 0; j < lab->maxcol; j++) {
            printf("[%d, %d: %c] ", lab->lab[i][j].x, lab->lab[i][j].y, lab->lab[i][j].type);
        }
        printf("|\n");
    }
    printf("LAB COSTS:----------------\n");
    for (i = 0; i <= lab->maxrow; i++) {
        printf("|");
        for (j = 0; j < lab->maxcol; j++) {
            if (lab->lab[i][j].distance > 9) printf("%d ", lab->lab[i][j].distance);
            else printf("0%d ", lab->lab[i][j].distance);
        }
        printf("|\n");
    }
    delay(100);
}

int getManhattanDistance(NODE currentNode) {
    return abs(currentNode.x - goalX) + abs(currentNode.y - goalY);
}

void xmalloc_open(void) {
    /* Speicher anfordern und Zeiger per Typecast ändern */
    if ((open_actual = (NODE *) malloc(sizeof(NODE))) == NULL) {
        printf("\nNo memory available!\n");
        exit(8);
    }
    return;
}

void xmalloc_closed(void) {
    /* Speicher anfordern und Zeiger per Typecast ändern */
    if ((closed_actual = (NODE *) malloc(sizeof(NODE))) == NULL) {
        printf("\nNo memory available!\n");
        exit(8);
    }
    return;
}

NODE *addList(NODE **list_start, NODE *new_node) {
    new_node->index = nodeIndex;
    nodeIndex++;
    new_node->next = *list_start;
    *list_start = new_node;
    return new_node;
}

void deleteList(NODE **list_start, NODE *delete_node) {
    NODE *handle = *list_start;
    NODE *previousHandle = NULL;
    while (handle) {
        if (handle->index == delete_node->index) {
            if (previousHandle == NULL) {
                *list_start = handle->next;
            } else {
                previousHandle->next = handle->next;
            }
            free(handle);
            return;
        } else {
            printf("Not found");
        }
        previousHandle = handle;
        handle = handle->next;
    }
}

NODE *fillList(int i) {
    /* Strukturelemente mit Nutzdaten füllen */
    NODE *node = malloc(sizeof(NODE));
    node->distance = i;
    return node;
}

void printList(NODE* list_actual, NODE* list_start) {
    printf("\nListe vorwärts ausgeben\n");
    /* Zeiger auf erstem Element positionieren */
    list_actual = list_start;
    /* Solange es Elemente gibt, also bis momentan==NULL */
    while (list_actual) {
        printf("\nDistance %d\nIndex %d\n",
               list_actual->distance, list_actual->index);
        /* Zeiger auf nächstem Element positionieren */
        list_actual = list_actual->next;
    }
    return;
}

void delay(long milliseconds) {
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

