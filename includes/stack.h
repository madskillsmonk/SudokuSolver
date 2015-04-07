/* stack structures */
typedef struct StackNode {
    int ** grid;
} StackNode;

typedef struct Stack {
    StackNode * items;
    int top;
    int size;
} Stack;

/* stack operations */

int stackIsEmpty(Stack * stack);

void createStack(Stack *stack, int size);

int stackIsFull(Stack * stack);

void stackPush(Stack * stack, StackNode node);

StackNode * stackPop(Stack * stack);

StackNode * createNode(int ** grid);

void freeStackNode(StackNode * node);

void freeStack(Stack * stack);
