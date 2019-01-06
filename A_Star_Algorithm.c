//
//  A_Star_Algorithm.c
//  
//
//  Created by Felix Waldbach on 28.12.18.
//

#include "A_Star_Algorithm.h"
#include <stdio.h>

Lab_p lab;

int main(int argc, char *argv[]) {
    FILE *in = stdin;

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
    aStar2();

    //aStarRun(lab, open_start);
    //aStar2();

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

void aStar2() {
    printf("Running A Star\n");
    while (open_start) {
        int i;
        //Find node with least f on the open list
        NODE *leastF;
        leastF = malloc(sizeof(NODE));
        do {
            leastF = findCheapestFNode(&open_start);
            leastF->distance = leastF->distance * boatWeight;
            printf("X: %d, Y: %d, Distance: %lf\n", leastF->x, leastF->y, leastF->distance);
            //Remove it from the open list
            liste_loeschen_wert(&open_start, leastF->x, leastF->y);
        } while (leastF->type == '1' && !boat);
        if (leastF->type == '1') {
            printf("River at %d, %d\n", leastF->x, leastF->y);
            boat = false;
            boatWeight = 0.9;
        }
        if (isDestination(leastF->x, leastF->y)) {
            printf("Found the goal!! %d, %d\n", leastF->x, leastF->y);
            printf("Boats left: %d\n", boat);
            printPath(leastF);
            return;
        }
        //Generate and set its successors
        if (leastF->x - 1 >= 0) {
            leastF->successors[0] = &lab->lab[leastF->x - 1][leastF->y]; // north
            leastF->successors[0]->parent = leastF;
        }
        if (leastF->y + 1 < lab->maxcol) {
            leastF->successors[1] = &lab->lab[leastF->x][leastF->y + 1]; // east
            leastF->successors[1]->parent = leastF;
        }
        if (leastF->x + 1 < lab->maxrow) {
            leastF->successors[2] = &lab->lab[leastF->x + 1][leastF->y]; // south
            leastF->successors[2]->parent = leastF;
        }
        if (leastF->y - 1 >= 0) {
            leastF->successors[3] = &lab->lab[leastF->x][leastF->y - 1]; // west
            leastF->successors[3]->parent = leastF;
        }
        for (i = 0; i < 4; i++) {
            NODE *handle;
            handle = leastF->successors[i];
            if (!handle) continue;
            handle->g = handle->parent->g + handle->distance;
            handle->h = getManhattanDistance(*handle);
            handle->f = handle->g + handle->h;
            if (cheaperNode(&open_start, handle->x, handle->y, handle->f) ||
                cheaperNode(&closed_start, handle->x, handle->y, handle->f)) {
                continue;
            } else {
                liste_einfuegen_anfang(&open_start, handle->distance, handle->g, handle->f, handle->h, handle->x,
                                       handle->y, handle->type);
            }

        }
        liste_einfuegen_anfang(&closed_start, leastF->distance, leastF->g, leastF->f, leastF->h, leastF->x, leastF->y,
                               leastF->type);
    }
}

void printPath(NODE *goal) {
    if (!goal) {
        printf("Grandparent reached\n");
        return;
    } else {
        printNode(goal);
        printPath(goal->parent);
    }

}

bool cheaperNode(NODE **list_start, int x, int y, double f) {
    NODE *handle, *previousHandle;
    handle = *list_start;
    previousHandle = NULL;
    while (handle) {
        if (handle->x == x && handle->y == y && handle->f <= f) {
            return true;
        }
        previousHandle = handle;
        handle = handle->next;
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

NODE *findCheapestFNode(NODE **start) {
    NODE *davor;
    double f;
    NODE *returnNode;

    if (*start == NULL) {
        return NULL;
    }

    returnNode = *start;
    f = (*start)->f;

    davor = momentan = (*start)->next;
    while (momentan) {
        if (momentan->f < f) {
            f = momentan->f;
            returnNode = momentan;
        }
        davor = momentan;
        momentan = momentan->next;
    }
    return returnNode;
}

bool isInList(NODE **start, int x, int y) {
    NODE *davor;

    if (*start == NULL) {
        return -1;
    }

    if ((*start)->x == x && (*start)->y == y) {
        return true;
    }

    davor = momentan = (*start)->next;
    while (momentan) {
        if (momentan->x == x && momentan->y == y) {
            return true;
        }
        davor = momentan;
        momentan = momentan->next;
    }
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
            NODE *handle = malloc(sizeof(NODE));
            handle->type = c;
            handle->x = row;
            handle->y = col;
            switch (c) {
                case '0':
                    handle->distance = TYPE_COSTS_0;
                    break;
                case '1':
                    handle->distance = TYPE_COSTS_1;
                    break;
                case '2':
                    handle->distance = TYPE_COSTS_2;
                    break;
                case '3':
                    handle->distance = TYPE_COSTS_3;
                    break;
                case '4':
                    handle->distance = TYPE_COSTS_4;
                    break;
                case '5':
                    handle->distance = TYPE_COSTS_5;
                    break;
                default:
                    break;
            }
            elem->lab[row][col++] = *handle;
            handle_max_col++;
        }
    } while (c != EOF && elem->maxcol <= MAXCOLS && elem->maxrow <= MAXROWS && semi_ctr != 14);
    double g = 0.0;
    double h = getManhattanDistance(elem->lab[startX][startY]);
    double f = h;
    printf("Adding start to list\n");
    liste_einfuegen_anfang(&open_start, elem->lab[startX][startY].distance, g, f, h, elem->lab[startX][startY].x,
                           elem->lab[startX][startY].y, elem->lab[startX][startY].type);
    boat = true;
    boatWeight = 1.0;
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
            if (lab->lab[i][j].distance > 9) printf("%lf ", lab->lab[i][j].distance);
            else printf("%lf ", lab->lab[i][j].distance);
        }
        printf("|\n");
    }
}

int getManhattanDistance(NODE currentNode) {
    return abs(currentNode.x - goalX) + abs(currentNode.y - goalY);
}

void printList(NODE *list_actual, NODE *list_start) {
    list_actual = list_start;
    printf("Printing list...\n");
    while (list_actual) {
        printNode(list_actual);
        list_actual = list_actual->next;
    }
}

void printNode(NODE *print_node) {
    printf("Printing Node: --Distance: %lf, g: %lf, f: %lf, h: %lf, x: %d, y: %d--\n",
           print_node->distance, print_node->g, print_node->f, print_node->h, print_node->x,
           print_node->y);
}

void liste_einfuegen_anfang(NODE **start, double distance, double g, double f, double h, int x, int y, char type) {
    NODE *new_node;
    new_node = malloc(sizeof(NODE));

    new_node->distance = distance;
    new_node->g = g;
    new_node->f = f;
    new_node->h = h;
    new_node->x = x;
    new_node->y = y;
    new_node->type = type;
    new_node->next = *start;
    *start = new_node;
}

int liste_loeschen_anfang(NODE **start) {
    int retval = -1;
    NODE *next_node = NULL;

    if (*start == NULL) {
        return -1;
    }

    next_node = (*start)->next;
    retval = (*start)->x;
    //free(*start);
    *start = next_node;

    return retval;
}

int liste_loeschen_wert(NODE **start, int x, int y) {
    NODE *davor;

    if (*start == NULL) {
        return -1;
    }

    if ((*start)->x == x && (*start)->y == y) {
        return liste_loeschen_anfang(start);
    }

    davor = momentan = (*start)->next;
    while (momentan) {
        if (momentan->x == x && momentan->y == y) {
            davor->next = momentan->next;
            //free(momentan);
            return x;
        }


        davor = momentan;
        momentan = momentan->next;
    }
    return -1;
}

