//
//  A_Star_Algorithm.h
//  
//
//  Created by Felix Waldbach & Yunus Emre Besogul on 28.12.18.
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

typedef struct labyrinth {
    NODE lab[MAXCOLS][MAXROWS];
    int maxrow;
    int maxcol;
} LAB, *Lab_p;

Lab_p lab;

NODE *actual, *open_actual, *open_handle, *open_start;

NODE *closed_actual, *closed_handle, *closed_start;

int startX, startY, goalX, goalY;

bool boat;
double boatWeight;

void runAStar();

/* Lab functions */
Lab_p generateLab(FILE *in);

void printLab(Lab_p lab);

/* Node functions */
void printPath(NODE *goal);

bool cheaperNode(NODE **list_start, int x, int y, double f);

bool isDestination(int x, int y);

NODE *findCheapestFNode(NODE **start);

bool isInList(NODE **start, int x, int y);

int getManhattanDistance(NODE currentNode);

void printNode(NODE *print_node);

/* List functions */
void printList(NODE *list_actual, NODE *list_start);


void addToList(NODE **start, double distance, double g, double f, double h, int x, int y, char type,
               NODE *parent);

int removeStartFromList(NODE **start);

int removePositionFromList(NODE **start, int x, int y);

#endif /* A_Star_Algorithm_h */
