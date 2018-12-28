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

typedef struct Node {
    struct Node *parent;
    struct Node *successors[4];
    struct Node *next, *previous;
    int distance, g, f, h, x, y, type;
} NODE;


/* Data type: Labyrinth - ASCII map, costs, direction */

typedef struct labyrinth
{
    NODE lab[MAXCOLS][MAXROWS];
    int maxrow;
    int maxcol;
} LAB, *Lab_p;

NODE *open_actual, *open_handle, *open_start, *open_end;
NODE *closed_actual, *closed_handle, *closed_start, *closed_end;

Lab_p generateLab(FILE* in);
void printLab(Lab_p lab);
void xmalloc(void);
//void fill_list(NODE list);
void print_list(NODE list, NODE actual, NODE start, NODE end);
void delay(long milliseconds);

#endif /* A_Star_Algorithm_h */
