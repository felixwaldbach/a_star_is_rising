//
//  A_Star_Algorithm.c
//
//
//  Created by Felix Waldbach & Yunus Emre Besogul on 28.12.18.
//


#include "A_Star_Algorithm.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Read .csv-file and set source and destination
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

    // Generate the 2d array to hold the map
    lab = generateLab(in);

    // Run the A* search
    runAStar();

    exit(EXIT_SUCCESS);
}

void runAStar() {
    printf("Running A Star...\n");

    // While the OPEN list is not empty... search for the goal
    while (open_start) {
        int i;

        // Find the node with the least f in the OPEN list
        NODE *leastF;
        leastF = malloc(sizeof(NODE));
        // Check if boat is already used and if the next successor is a river
        // Skip this successor because boat is already used and not accessable anymore
        do {
            leastF = findCheapestFNode(&open_start);
            leastF->distance = leastF->distance * boatWeight;
            //printf("X: %d, Y: %d, Distance: %lf\n", leastF->x, leastF->y, leastF->distance);
            // Remove this node from the OPEN list due it will be processed
            removePositionFromList(&open_start, leastF->x, leastF->y);
        } while (leastF->type == '1' && !boat);

        // Check if node is river: Means that we will cross the river one time and then never again
        if (leastF->type == '1') {
            printf("River at %d, %d -> Using boat!\n", leastF->x, leastF->y);
            // Disable boat usement and lower the upcoming costs with 10%
            boat = false;
            boatWeight = 0.9;
        }

        // Check if current cell is destination
        // Stop search if successor is destination and print out the path and cost
        if (isDestination(leastF->x, leastF->y)) {
            printf("Found the goal -> %d, %d\n", leastF->x, leastF->y);
            printf("Printing path... \n");
            printPath(leastF);
            printf("Printing costs... \n");
            printf("%.2lf\n", leastF->g);
            return;
        }

        int cheksum;    // int variable to count the successors
        cheksum = 0;

        // Generate the 4 successors of this cell, only if successor is inside boundaries
        // Only 4 successors because diagonal is forbidden in this task
        // Set the parents

        if (leastF->x - 1 >= 0) {
            leastF->successors[0] = &lab->lab[leastF->x - 1][leastF->y]; // north
            leastF->successors[0]->parent = leastF;
            cheksum++;
        }
        if (leastF->y + 1 < lab->maxcol) {
            leastF->successors[1] = &lab->lab[leastF->x][leastF->y + 1]; // east
            leastF->successors[1]->parent = leastF;
            cheksum++;
        }
        if (leastF->x + 1 < lab->maxrow) {
            leastF->successors[2] = &lab->lab[leastF->x + 1][leastF->y]; // south
            leastF->successors[2]->parent = leastF;
            cheksum++;
        }
        if (leastF->y - 1 >= 0) {
            leastF->successors[3] = &lab->lab[leastF->x][leastF->y - 1]; // west
            leastF->successors[3]->parent = leastF;
            cheksum++;
        }

        // For each successor, calculate its g, h and f value and add it to the OPEN list
        // 1. If a node with the same position as the successor is in the OPEN list,
        // compare the f value, and continue with the node that has a lower f value
        // -> Delete from OPEN list and add to OPEN list
        // 2. If a node with the same position as the succesor is in the CLOSED list,
        // compare the f value and continue with the node that has a lower f value
        // -> Delete from CLOSED list and put to OPEN list

        for (i = 0; i < cheksum; i++) {
            NODE *handle;
            handle = leastF->successors[i];

            if (!handle) continue;
            handle->g = handle->parent->g + handle->distance;
            handle->h = getManhattanDistance(*handle);
            handle->f = handle->g + handle->h;

            // Skip successor if node is in OPEN list and has a lower f OR skip if node is in CLOSED list and has a lower f
            if (cheaperNode(&open_start, handle->x, handle->y, handle->f) ||
                cheaperNode(&closed_start, handle->x, handle->y, handle->f)) {
                continue;
            } else {
                // Successor is not in OPEN or CLOSED list or has a lower f...
                // Delete old one from OPEN list
                if (isInList(&open_start, handle->x, handle->y)) {
                    removePositionFromList(&open_start, handle->x, handle->y);
                }

                // Push the successors to the OPEN list
                addToList(&open_start, handle->distance, handle->g, handle->f, handle->h, handle->x,
                          handle->y, handle->type, handle->parent);
            }
        }

        // Push the processed node to the CLOSED list
        // Initialize the CLOSED list
        if (isInList(&closed_start, leastF->x, leastF->y)) {
            removePositionFromList(&closed_start, leastF->x, leastF->y);
        }
        addToList(&closed_start, leastF->distance, leastF->g, leastF->f, leastF->h, leastF->x, leastF->y,
                  leastF->type, leastF->parent);
    }
}

// Function to generate the 2d array dynamically
// X-Axis max is 14 (0...14)
// Y-Axis max is 14 (0...14)
// First 15 rows are map
// Then empty row
// Then the meanings about the map: Code, Designation, Costs
Lab_p generateLab(FILE *in) {
    Lab_p elem = (LAB *) malloc(sizeof(LAB));
    elem->maxrow = 0;
    elem->maxcol = 0;
    int row = 0;
    int col = 0;
    int handle_max_col = 0;
    int semi_ctr = 0;

    char c;

    // Analyze the map and read its values
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
            handle->x = col;
            handle->y = row;
            elem->lab[col++][row] = *handle;
            handle_max_col++;
        }
    } while (c != EOF && elem->maxcol <= MAXCOLS && elem->maxrow <= MAXROWS && row < MAXROWS);

    char read[512];
    char save[10][512];
    int line = 0, ctr, y;

    while (fgets(read, 512, in) != NULL) {
        strcpy(save[line], read);
        line++;
    }
    for (ctr = 2; ctr <= line; ctr++) {
        int k, semictr;
        for (k = 0; k < strlen(save[ctr]); k++) {
            if (save[ctr][k] == ';') {
                semictr++;
            }
            if (semictr == 2) {
                y = 0;
                while (save[ctr][k + 1] != ';') {
                    saveToCosts(save[ctr][k + 1], ctr, y);
                    //printf("%c\n", save[ctr][k + 1]);
                    y++;
                    k++;
                }
                break;
            }
        }
        semictr = 0;
    }

    addCostsToLab(elem);

    // Calculate g, h and f values for the starting cell
    double g = 0.0;
    double h = getManhattanDistance(elem->lab[startX - 1][startY - 1]);
    double f = h;

    // Initialize the OPEN list
    // Add the starting cell to the OPEN list
    addToList(&open_start, elem->lab[startX][startY].distance, g, f, h,
              elem->lab[startX][startY].x,
              elem->lab[startX][startY].y, elem->lab[startX][startY].type, NULL);

    // Enable the usement of the boat to cross the river and set the weight of the boat to 100% on the start
    boat = true;
    boatWeight = 1.0;
    return elem;
}

void saveToCosts(char c, int x, int y) {
    costs[x][y] = c;
}

// Function to print the map as a 2d array with its values
void addCostsToLab(Lab_p lab) {
    int i, j;
    for (i = 0; i < lab->maxcol; i++) {
        for (j = 0; j < lab->maxrow; j++) {

            // Skip index 0 and 1 because they are used for description and empty row
            int index, cost;
            switch (lab->lab[j][i].type) {
                case '0':
                    index = 2;
                    cost = atoi(costs[index]);
                    lab->lab[j][i].distance = cost;
                    break;
                case '1':
                    index = 3;
                    cost = atoi(costs[index]);
                    lab->lab[j][i].distance = cost;
                    break;
                case '2':
                    index = 4;
                    cost = atoi(costs[index]);
                    lab->lab[j][i].distance = cost;
                    break;
                case '3':
                    index = 5;
                    cost = atoi(costs[index]);
                    lab->lab[j][i].distance = cost;
                    break;
                case '4':
                    index = 6;
                    cost = atoi(costs[index]);
                    lab->lab[j][i].distance = cost;
                    break;
                case '5':
                    index = 7;
                    cost = atoi(costs[index]);
                    lab->lab[j][i].distance = cost;
                    break;
                default:
                    break;
            }
        }
    }
}

// Function to print the map as a 2d array with its values
void printLab(Lab_p lab) {
    system("clear");
    int i, j;
    printf("Lab size: %d x %d\n", lab->maxrow, lab->maxcol);
    printf("LAB TYPES:----------------\n");
    for (i = 0; i < lab->maxcol; i++) {
        for (j = 0; j < lab->maxrow; j++) {
            printf("[%d, %d: %c] ", lab->lab[j][i].x, lab->lab[j][i].y, lab->lab[j][i].type);
        }
        printf("|\n");
    }
    printf("LAB COSTS:----------------\n");
    for (i = 0; i < lab->maxcol; i++) {
        printf("|");
        for (j = 0; j < lab->maxrow; j++) {
            if (lab->lab[j][i].distance > 9) printf("%lf ", lab->lab[j][i].distance);
            else printf("%lf ", lab->lab[j][i].distance);
        }
        printf("|\n");
    }
}


// Function to print the path and cost after the A* algorithm has found the destination
void printPath(NODE *goal) {
    if (!goal) {
        printf("Start\n");
        return;
    } else {
        printf("X: %d, Y: %d <- ", goal->x, goal->y);
        printPath(goal->parent);
    }
}

// Function to check if there is a node with a cheaper f
bool cheaperNode(NODE **list_start, int x, int y, double f) {
    NODE *handle, *previousHandle;
    handle = *list_start;
    previousHandle = NULL;
    while (handle) {
        // If there is a cheaper f, return true
        if (handle->x == x && handle->y == y && handle->f <= f) {
            return true;
        }
        previousHandle = handle;
        handle = handle->next;
    }
    return false;
}

// Function to check if destination is reached
bool isDestination(int x, int y) {
    if (x == goalX && y == goalY) {
        return true;
    } else {
        return false;
    }
}

// Function to find and return the cheapest node in the OPEN list
NODE *findCheapestFNode(NODE **start) {
    NODE *previous;
    double f;
    NODE *returnNode;

    if (*start == NULL) {
        return NULL;
    }

    returnNode = *start;
    f = (*start)->f;

    previous = actual = (*start)->next;
    while (actual) {
        if (actual->f < f) {
            f = actual->f;
            returnNode = actual;
        }
        previous = actual;
        actual = actual->next;
    }
    return returnNode;
}

// Function to check if a node is in a  list
bool isInList(NODE **start, int x, int y) {
    NODE *previous;

    if (*start == NULL) {
        return -1;
    }

    if ((*start)->x == x && (*start)->y == y) {
        return true;
    }

    previous = actual = (*start)->next;
    while (actual) {
        if (actual->x == x && actual->y == y) {
            return true;
        }
        previous = actual;
        actual = actual->next;
    }
    return false;
}

// Approximation Heuristics calculation for h
int getManhattanDistance(NODE currentNode) {
    return abs(currentNode.x - goalX) + abs(currentNode.y - goalY);
}

// Function to print the values from the structure
void printNode(NODE *print_node) {
    printf("Printing Node: --Distance: %lf, g: %lf, f: %lf, h: %lf, x: %d, y: %d--\n",
           print_node->distance, print_node->g, print_node->f, print_node->h, print_node->x,
           print_node->y);
}

// Function to print a list
void printList(NODE *list_actual, NODE *list_start) {
    list_actual = list_start;
    printf("Printing list...\n");
    while (list_actual) {
        printNode(list_actual);
        list_actual = list_actual->next;
    }
}

// Function to add data to the beginning of a list
void addToList(NODE **start, double distance, double g, double f, double h, int x, int y, char type,
               NODE *parent) {
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
    new_node->parent = parent;
    *start = new_node;
}

// Function to delete the first input in a list
int removeStartFromList(NODE **start) {
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

// Function to delete a specific data in a list
int removePositionFromList(NODE **start, int x, int y) {
    NODE *previous;

    if (*start == NULL) {
        return -1;
    }

    if ((*start)->x == x && (*start)->y == y) {
        return removeStartFromList(start);
    }

    previous = actual = (*start)->next;
    while (actual) {
        if (actual->x == x && actual->y == y) {
            previous->next = actual->next;
            //free(momentan);
            return x;
        }
        previous = actual;
        actual = actual->next;
    }
    return -1;
}

