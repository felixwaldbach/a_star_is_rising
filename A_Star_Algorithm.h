//
//  A_Star_Algorithm.h
//  
//
//  Created by Felix Waldbach on 28.12.18.
//

#ifndef A_Star_Algorithm_h
#define A_Star_Algorithm_h
#define MAXCOLS  15
#define MAXROWS  15
#define BF_VALID "012345"
#define TYPE_COSTS_0 3
#define TYPE_COSTS_1 5
#define TYPE_COSTS_2 1
#define TYPE_COSTS_3 4
#define TYPE_COSTS_4 2
#define TYPE_COSTS_5 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct Node {
    struct Node *parent;
    struct Node *successors[4];
    struct Node *next;
    double distance, g, f, h;
    int x, y;
    char type;
} NODE;


/* Data type: Labyrinth - ASCII map, costs, direction */

typedef struct labyrinth {
    NODE lab[MAXCOLS][MAXROWS];
    int maxrow;
    int maxcol;
} LAB, *Lab_p;

NODE *momentan, *open_actual, *open_handle, *open_start, *open_end;
NODE *closed_actual, *closed_handle, *closed_start, *closed_end;
int startX, startY, goalX, goalY;
bool boat;
double boatWeight;

bool isDestination(int x, int y);

Lab_p generateLab(FILE *in);

void printLab(Lab_p lab);

//void fill_list(NODE list);
void print_list(NODE list, NODE actual, NODE start, NODE end);

int getManhattanDistance(NODE currentNode);

void printList(NODE *list_actual, NODE *list_start);

bool aStarRun(Lab_p lab, NODE *current_node);

bool isDestination(int x, int y);

bool isInList(NODE **start, int x, int y);

NODE *findCheapestFNode(NODE **start);

void aStar2();

bool cheaperNode(NODE **list_start, int x, int y, double f);

NODE *findLeastF();

void printNode(NODE *print_node);

void liste_einfuegen_anfang(NODE **start, double distance, double g, double f, double h, int x, int y, char type);

int liste_loeschen_anfang(NODE **start);

int liste_loeschen_wert(NODE **start, int x, int y);

void printPath(NODE *goal);

#endif /* A_Star_Algorithm_h */
