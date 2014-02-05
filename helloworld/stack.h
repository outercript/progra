#ifndef __STACK__
#define __STACK__

typedef struct{
    int *elements;
    int position;
    int maxSize;
} intStack;

void intStackNew(intStack *s);
void intStackDispose(intStack *s);
void intStackPush(intStack *s, int value);
int intStackPop(intStack *s);



typedef struct{
    void *elements;
    int position;
    int maxSize;
    int elemSize;
} Stack;


void StackNew(Stack *s, int block_size);
void StackDispose(Stack *s);
void StackPush(Stack *s, void *value);
void StackPop(Stack *s, void *address);

#endif
