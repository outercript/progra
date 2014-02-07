#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "stack.h"

void intStackNew(intStack *s)
{
    s->position = 0;
    s->maxSize = 4;
    s->elements = (int *)malloc(s->maxSize * sizeof(int));
    assert(s->elements != NULL);
}

void intStackDispose(intStack *s)
{
    free(s->elements);
}

void intStackPush(intStack *s, int value)
{
    if (s->position == s->maxSize)
    {
        s->maxSize *= 2;
        s->elements = (int *)realloc(s->elements, s->maxSize*sizeof(int));
    }
    s->elements[s->position] = value;
    s->position++;
}

int intStackPop(intStack *s)
{
    assert(s->position > 0);
    s->position--;
    return s->elements[s->position];
}




void StackNew(Stack *s, int block_size, void (*dispose_fn)(void *))
{
    s->position = 0;
    s->maxSize = 4;
    s->elemSize = block_size;
    s->elements = malloc(s->maxSize * s->elemSize);
    s->freefn = dispose_fn;
    assert(s->elements != NULL);
}

void StackDispose(Stack *s)
{
    int i;
    void *address;
    if(s->freefn != NULL)
    {
        for(i=0; i < s->position; i++)
        {
            address = (char *)s->elements + (s->position * s->elemSize);
            s->freefn(address);
        }
    }
    free(s->elements);
}

void StackPush(Stack *s, void *value)
{
    void *address;
    if (s->position == s->maxSize)
    {
        s->maxSize *= 2;
        s->elements = realloc(s->elements, s->maxSize*s->elemSize);
    }
    address = (char *)s->elements + (s->position * s->elemSize);
    memcpy(address, value, s->elemSize);
    s->position++;
}

void StackPop(Stack *s, void *dest)
{
    void *address;
    assert(s->position > 0);
    s->position--;

    address = (char *)s->elements + (s->position * s->elemSize);
    memcpy(dest, address, s->elemSize);
}


void dispose_string(void *str)
{
    free(*(char **)str);
}

int main(void)
{
    int i;
    char *cadenas[] = {"hola", "mundo", "caca"};
    char *p;

    Stack test;
    StackNew(&test, sizeof(char *), dispose_string);

    for(i=0; i<3; i++)
    {
        p = strdup(cadenas[i]);
        StackPush(&test, &p);
    }

    for(i=0; i<3; i++)
    {
        StackPop(&test, &p);
        printf("%s ", p);
    }

    printf("\nComplete");
    return 0;
}
