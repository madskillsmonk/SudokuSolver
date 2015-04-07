#define DIMENSION 9
#define CREATE_MASK(n) (1 << (n))

void solver(int **puzzle);
int isSolved(int **puzzle);

int * getBinaryArray(int num);

int getRowVector(int **board, int row);
int getColVector(int **board, int col);
int getGridVector(int **board, int col, int row);

int getPossibleValueCount(int rowBits, int colBits, int gridBits);
int getValueFromSingleBit(int rowBits, int colBits, int gridBits);
int getRandomValueFromBits(int rowBits, int colBits, int gridBits);

int ** copyGrid(int **puzzle);
int ** createGridFromFile(FILE * file);

void printBoard(int **board);

