#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "solver.h"
#include "stack.h"

int main(int argc, char * argv[]) {
    if(argc != 2) {
        printf("Usage: ./solver filename.sudoku");
        return(0);
    }

    FILE * input = fopen(argv[1], "r");
    srand(time(NULL));
    int ** grid = createGridFromFile(input);
    fclose(input);

    if(!grid) {
        printf("Error parsing the input file. Exiting.\n");
        return(0);
    }

    printf("\n\nOriginal:\n");
    printBoard(grid);

    solver(grid);

    return(1);
}

/*
 * Function:  solver
 * --------------------
 *  The function that actually solves the sudoku puzzle.  The algorithm is as follows:
 *      1. If there are any spots that have only 1 possible number that can fit, 
 *           fill it in go back to step 1
 *      2. If there is a spot that has 2 possible numbers that fit, 
 *           pick a random number and go back to step 1.
 *       3. Either the board is solved is solved or there is a spot that no numbers can fit it.
 *           Backtrack to the last time we picked a random number and try again. 
 *
 *  returns: no return, but it prints the solved puzzle at the end
 */
void solver(int **puzzle) {
    Stack stack;
    StackNode * current = NULL;
    int rowBits, colBits, gridBits, spots;
    int nx, ny;

    createStack(&stack, 500);
    current = createNode(puzzle);

    int randX = 0, randY = 0, found=0;

    /* step 1 as outlined in the algorithm, find all spots on the board with only 1 possible value and fill them in */
    STEP1: {
        if(isSolved(current->grid)) {
            goto CLEANUP;
        }
        /* reset the 2-spot values */
        randX=0, randY=0, found=0;
        for(nx=0; nx<DIMENSION; nx++) {
            for(ny=0; ny<DIMENSION; ny++) {
                if(!current->grid[nx][ny]) {

                    /* potential numbers that can go in each cell based on rows, column, and the current grid of the cell */
                    rowBits = getRowVector(current->grid, nx);
                    colBits = getColVector(current->grid, ny);
                    gridBits = getGridVector(current->grid, ny, nx);

                    spots = getPossibleValueCount(rowBits, colBits, gridBits);
                        
                    /* cell with only potential of only 1 number that cna go there */
                    if(spots == 1) {
                        current->grid[nx][ny] = getValueFromSingleBit(rowBits, colBits, gridBits);
                    } else if(spots == 0) {
                        /* we found a spot with no possible values! This means we made an incorrect choice somewhere else, so we need to backtrack */
                        freeStackNode(current);
                        current = stackPop(&stack);
                        if(!current) {
                            /* if the stack was empty, reset to the begining and try again */
                            current = createNode(puzzle);
                        }
                        
                    } else if(spots == 2) {
                        /* we find some spots with 2 possible values, 
                        take note of them as we might need to use them later */
                        randX = nx;
                        randY = ny;
                        found = 1;
                    }

                    /* the puzzle is solved! */
                    if(spots == 0 || spots == 1) {
                        if(nx == 8 && ny == 8) {
                            goto CLEANUP;
                        }
                        goto STEP1;
                    }
                }
            }
        }
    }

    /* if there were no spots with 2 potential numbers, pick a spot at random */
    if(!found) {
        do {
            randX = rand() % DIMENSION;
            randY = rand() % DIMENSION;
        } while(current->grid[randX][randY] != 0);

        /* push the current state onto the stack since we are making a random choice that COULD be wrong */
        stackPush(&stack, *current);
        current = createNode(current->grid);
    }
    
    /* calculate the potential numbers that could go in this position */
    rowBits = getRowVector(current->grid, randX);
    colBits = getColVector(current->grid, randY);
    gridBits = getGridVector(current->grid, randY, randX);

    spots = getPossibleValueCount(rowBits, colBits, gridBits);

    /* put a number in the cell and go back to step 1 as outlined by the algorithm */
    if(spots >= 1) {
        int randNum = getRandomValueFromBits(rowBits, colBits, gridBits);
        current->grid[randX][randY] = randNum;
    } else if(spots == 0) {
        if(current) {
            freeStackNode(current);
        }
        current = stackPop(&stack);        
    }
    
    goto STEP1;

    CLEANUP: {
        printf("\n\nSolved:\n");
        printBoard(current->grid);
        freeStack(&stack);
    }
}
/*
 * Function:  copyGrid
 * --------------------
 *  Create a copy of this grid represented by a 2d array
 *
 *  returns: a duplicate 2d array with the exact same entries as the parameter puzzle
 */
int ** copyGrid(int ** puzzle) {
    int i, j;
    int ** copy = malloc(10 * sizeof(int*));
    for(i=0; i<DIMENSION; i++) {
        copy[i] = malloc(sizeof(int)*10);
    }

    for(i=0; i<DIMENSION; i++) {
        for(j=0; j<DIMENSION; j++) {
            copy[i][j] = puzzle[i][j];
        }
    }
    return(copy);
}

/*
 * Function:  createGridFromFile
 * --------------------
 *  Creates a grid represented by a 2d array from a given file with a specified format
 *
 *  returns: a 2d array representing the grid
 */
int ** createGridFromFile(FILE * input) {
    if(!input) {
        printf("Couldn't open file. Exiting.\n");
        return(NULL);
    }

    int i;
    int ** board = malloc(10 * sizeof(int*));

    if(!board) return NULL;

    for(i=0; i<DIMENSION; i++) {
        board[i] = malloc(sizeof(int)*10);
        if(!board[i]) return NULL;
    }

    for(i=0; i<DIMENSION; i++) {
        if(!fscanf(input, "%d %d %d %d %d %d %d %d %d", &board[i][0], &board[i][1], &board[i][2],
              &board[i][3], &board[i][4], &board[i][5], &board[i][6], &board[i][7], &board[i][8])) {
            return(NULL);
        }
    }

    return(board);
}

/*
 * Function:  isSolved
 * --------------------
 *  Check if the grid is solved by making sure there is a non-zero number in every spot
 *
 *  returns: a 1 if the puzzle is solved, 0 if not
 */
int isSolved(int **puzzle) {
    for(int i=0; i<DIMENSION; i++) {
        for(int j=0; j<DIMENSION; j++) {
            if(!puzzle[i][j]) {
                return(0);
            }
        }
    }
    return(1);
}

/*
 * Function:  getRandomValueFromBits
 * --------------------
 *  Selects a random bit from the three other vectors that identifies 
 *      which possible numbers can go in that position
 *
 *  returns: a non-zero integer representing the bit position if there is more than 0 possible numbers to select,
 *           or 0 if there are no possible values to use.
 */
int getRandomValueFromBits(int rowBits, int colBits, int gridBits) {
    int count = getPossibleValueCount(rowBits, colBits, gridBits);
    int total = (rowBits | colBits | gridBits);

    int randPos = rand() % count;

    int * binary = getBinaryArray(total);

    for(int i=1; i<10; i++) {
        if(!binary[i]) {
            if(!randPos) {
                return(i);
            } else {
                randPos--;
            }
        }
    }
    return(0);
}

/*
 * Function:  getRandomValueFromBits
 * --------------------
 *  Counts the potential number of values that be placed into a given cell on the board 
 *       from an algorithm that uses bitwise vectors to represent if each number (1-9) is
 *       present in the respective row, column, and grid (3x3 square in which the cell resides)
 *
 *        ex: row vector: [1 0 1 1 0 1 1 0 1]
 *            col vector: [1 0 1 1 1 1 1 1 0]
 *           grid vector: [1 0 1 1 1 1 1 1 1]
 *
 *        resultant vector from OR operation:
 *                        [1 0 1 1 1 1 1 1 1]
 *        
 *           We can see that there is only 1 0 in the final vector, therefore only 1 number can be placed in the given cell
 *
 *  returns: the number of possible values that can be placed into the given cell
 */
int getPossibleValueCount(int rowBits, int colBits, int gridBits) {
    int total = (rowBits | colBits | gridBits);
    int * binary = getBinaryArray(total);
    int count = 0;

    for(int i=1; i<10; i++) {
        if(!binary[i]) count++;
    }
    return(count);
}

/*
 * Function:  getValueFromSingleBit
 * --------------------
 *  Assuming there is only 1 potential number that can be placed into a given cell, get it's value 
 *
 *  returns: the number that can be placed in that position. 0 if none exist
 */
int getValueFromSingleBit(int rowBits, int colBits, int gridBits) {
    int total = (rowBits | colBits | gridBits);

    int * binary = getBinaryArray(total);

    for(int i=1; i<10; i++) {
        if(!binary[i]) return(i);
    }
    return(0);
}

/*
 * Function:  getGridVector
 * --------------------
 *  Create the grid bit-vector of potential values of a given cell. Calculates which 
 *       grid the cell is in and uses the rows and columns in the correct grid.
 *    
 *   It calculates the grid/column start row by: row - row % 3 and same for column
 *   Then it can easily create the total vector by repeatedly using the OR operator 
 *      on a mask containing each number. This process is similar to the colVector 
 *      and rowVector functions but they are simpler
 *
 *  returns: the grid vector as an integer 
 */
int getGridVector(int ** board, int col, int row) {
    int gridBits=0;
    int gridStartRow = row - row % 3;
    int gridStartCol = col - col % 3;

    for(int i=gridStartRow; i<gridStartRow+3; i++) {
        for(int j=gridStartCol; j<gridStartCol+3; j++) {
            if(board[i][j]) {
                gridBits |= CREATE_MASK(board[i][j]-1);
            }
        }
    }
    return(gridBits);
}

/*
 * Function:  getColVector
 * --------------------
 *  create a bitwise vector of a given column 
 *
 *  returns: the column vector as an integer 
 */
int getColVector(int ** board, int col) {
    int colBits = 0;

    for(int i=0; i<DIMENSION; i++) {
        if(board[i][col]) {
            colBits |= CREATE_MASK(board[i][col]-1);
        }
    }
    return(colBits);
}

/*
 * Function:  getRowVector
 * --------------------
 *  create a bitwise vector of a given row 
 *
 *  returns: the row vector as an integer 
 */
int getRowVector(int ** board, int row) {
    int rowBits=0;

    for(int i=0; i<DIMENSION; i++) {
        if(board[row][i]) {
            rowBits |= CREATE_MASK(board[row][i]-1);
        }
    }
    return(rowBits);
}

/*
 * Function:  getBinaryArray
 * --------------------
 *  convert the an integer representing a bitwise vector into a binary array
 *
 *  returns: the bitwise vector as an array of 9 numbers
 */
int * getBinaryArray(int num) {
    int * binaryArray, i=1;
    binaryArray = malloc(sizeof(int)*10);

    while(num) {
         binaryArray[i++]= num % 2;
         num /= 2;
    }
    return(binaryArray);
}

/*
 * Function:  printBoard
 * --------------------
 *  Print the sudoku board with some nice dividers so the user can see it
 *
 *  returns: no return but prints the board to stdout
 */
void printBoard(int ** board) {
    for(int i=0; i<DIMENSION; i++) {
        for(int j=0; j<DIMENSION; j++) {
            if(j != 0 && j % 3 == 0) printf("| ");
            printf("%d ", board[i][j]);
        }
        if(i % 3 == 2 && i != DIMENSION-1) {
            printf("\n----------------------\n");
        } else {
            printf("\n");
        }
    }
}

