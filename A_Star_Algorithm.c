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

    /*
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
    */
    printList(open_actual, open_start);

    aStarRun(lab, open_start);

    exit(EXIT_SUCCESS);
}

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

bool aStarRun(Lab_p lab, NODE *current_node) {
    xmalloc_open();
    printList(open_actual, open_start);
    int i;
    int f;
    while (open_start) {
        current_node->successors[0] = &lab->lab[current_node->x - 1][current_node->y]; // north
        current_node->successors[0]->parent = current_node;
        current_node->successors[1] = &lab->lab[current_node->x][current_node->y + 1]; // east
        current_node->successors[1]->parent = current_node;
        current_node->successors[2] = &lab->lab[current_node->x + 1][current_node->y]; // south
        current_node->successors[2]->parent = current_node;
        current_node->successors[3] = &lab->lab[current_node->x][current_node->y - 1]; // west
        current_node->successors[3]->parent = current_node;
        for (i = 0; i < 4; i++) {
            printf("\nSUCCESSOR %d: X: %d, Y: %d\n", i, current_node->successors[i]->x, current_node->successors[i]->y);
            if (isDestination(current_node->successors[i]->x, current_node->successors[i]->y)) {
                printf("Found! Path: blabla, costs: bla");
                return true;
            }
            current_node->successors[i]->g =
                    current_node->successors[i]->parent->g + current_node->successors[i]->distance;
            current_node->successors[i]->h = getManhattanDistance(*current_node->successors[i]);
            current_node->successors[i]->f = current_node->successors[i]->g + current_node->successors[i]->h;
            printf("F: %d\n", current_node->successors[i]->f);
            if (!isInList(&closed_start, current_node->successors[i])) {
                printf("Inside outer if\n");
                NODE *handle;
                handle = isInList(&open_start, current_node->successors[i]);
                printf("Handle f: %d", handle->f);
                if (handle->f >= current_node->successors[i]->f) {
                    printf("Inside inner if\n");
                    deleteList(&open_start, handle);
                    addList(&open_start, current_node->successors[i]);
                }
                printf("Survived inner if\n");
                if (!isInList(&open_start, current_node->successors[i])) {
                    printf("Inside second inner if");
                    addList(&open_start, current_node->successors[i]);
                }
                printf("Survived if\n");
            } else {
                printf("Is in the closed list\n");
            }
        }
        addList(&closed_start, current_node);
        //aStarRun(lab, findCheapestFNode());
    }
    return false;
}

bool isDestination(int x, int y) {
    if (x == goalX && y == goalY) {
        return true;
    } else {
        return false;
    }
}

NODE *findCheapestFNode() {
    NODE *handle = open_start;
    NODE *previousHandle = NULL;
    int cheapestF = 0;
    NODE *cheapestNode;
    while (handle) {
        if (handle->f <= cheapestF) {
            cheapestF = handle->f;
            cheapestNode = handle;
        } else {
            printf("Not found");
        }
        previousHandle = handle;
        handle = handle->next;
    }
    printf("\nCheapest Node: f %d\nX %d \nY %d",
           cheapestNode->f, cheapestNode->x, cheapestNode->y);
    return cheapestNode;
}

bool *isInList(NODE **list_start, NODE *node) {
    NODE *handle = *list_start;
    NODE *previousHandle = NULL;
    while (handle) {
        if (handle == node) {
            if (previousHandle == NULL) {
                *list_start = handle->next;
            } else {
                previousHandle->next = handle->next;
            }
            printf("Handle isInList X: %d\n", handle->x);
            return true;
        } else {
            printf("Not found in list\n");
        }
        previousHandle = handle;
        handle = handle->next;
        printf("Iterating \n");
    }
    printf("Returning NULL");
    return false;
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
        //printf("Semi counter: %d\n", semi_ctr);
        c = (char) fgetc(in);
        if (c == '\n') {
            row++;
            col = 0;
            if (handle_max_col > elem->maxcol) elem->maxcol = handle_max_col;
            handle_max_col = 0;
            elem->maxrow++;
        } else if (c == ';') {
            semi_ctr++;
            //printf("Semi increasing: %d\n", semi_ctr);
        } else if (strchr(BF_VALID, c)) {
            //printf("Character: %c\n", c);
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
    //open_start = elem->lab[startX][startY];
    addList(&open_start, &elem->lab[startX][startY]);
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
    new_node->next = *list_start;
    *list_start = new_node;
    return new_node;
}

void deleteList(NODE **list_start, NODE *delete_node) {
    NODE *handle = *list_start;
    NODE *previousHandle = NULL;
    while (handle) {
        if (handle == delete_node) {
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

void printList(NODE *list_actual, NODE *list_start) {
    printf("\nList:\n");
    /* Zeiger auf erstem Element positionieren */
    list_actual = list_start;
    /* Solange es Elemente gibt, also bis momentan==NULL */
    while (list_actual) {
        printf("\nDistance %d, X %d, Y %d\n",
               list_actual->distance, list_actual->x, list_actual->y);
        /* Zeiger auf nächstem Element positionieren */
        list_actual = list_actual->next;
    }
    printf("Print list done");
}

void delay(long milliseconds) {
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

