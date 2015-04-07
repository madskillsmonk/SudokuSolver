#include <stdlib.h>
#include <stdio.h>
#include "solver.h"
#include "stack.h"

/*
 * Function:  freeStackNode
 * --------------------
 *  Frees memory of a StackNode Struct iteratively
 *
 *  returns: no return
 */
void freeStackNode(StackNode * node) {
    if(!node || !node->grid) return;
    for(int i=0; i<DIMENSION; i++) {
        if(node->grid[i])
        free(node->grid[i]);
    }
    free(node->grid);
    free(node);
}

void freeStack(Stack * stack) {
    int size = stack->top;
    int i;
    if(size > 0) {
        for(i=0; i<size; i++) {
            freeStackNode(stackPop(stack));
        }
    }
}

/*
 * Function:  createNode
 * --------------------
 *  Creates a StackNode Struct from a given 2d array of integers
 *
 *  returns: the dynamically created StackNode struct
 */
StackNode * createNode(int ** grid) {
    StackNode * newNode = malloc(sizeof(StackNode));
    newNode->grid = copyGrid(grid);
    return(newNode);
}

/*
 * Function:  createStack
 * --------------------
 *  Creates a Stack Struct with a static size. No error checking is done here on the size
 *      so it is expected that the size is reasonable 
 *
 *  returns: no return, sets the stack being passed in
 */
void createStack(Stack *stack, int size) {
    StackNode * nodes = NULL;

    nodes = (StackNode*)malloc(sizeof(StackNode) * size);

    /* memory allocation failed... uh oh. */
    if(!nodes) {
        exit(1);
    }

    stack->items = nodes;
    stack->size = size;
    stack->top = -1;
}

/*
 * Function:  stackIsEmpty
 * --------------------
 *  Checks if the stack parameter has 0 items in it
 *
 *  returns: 1 if it is empty, 0 if not
 */
int stackIsEmpty(Stack * stack) {
    return(stack->top < 0);
}

/*
 * Function:  stackIsFull
 * --------------------
 *  Checks if the stack parameter is full
 *
 *  returns: 1 if it is full, 0 if not
 */
int stackIsFull(Stack * stack) {
    return(stack->top >= stack->size - 1);
}

/*
 * Function:  stackPush
 * --------------------
 *  Adds a new node to the stock of the stack if there is room
 *
 *  returns: no return
 */
void stackPush(Stack * stack, StackNode node) {
    if(stackIsFull(stack)) {
        printf("Stack full!\n");
        exit(1);
    }
    stack->items[++stack->top] = node;
}

/*
 * Function:  stackPop
 * --------------------
 *  Removes a node from the stack if possible
 *
 *  returns: returns the removed node
 */
StackNode * stackPop(Stack * stack) {
    if(stackIsEmpty(stack)) {
        return(NULL);
    }
    return(&(stack->items[stack->top--]));
}

