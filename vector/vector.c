#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    v->lastElem = 0;
    v->elemSize = elemSize;
    v->initialLength = initialAllocation;
    v->currentLength = initialAllocation;
    v->freefn = freeFn;
    v->elements = malloc(v->elemSize * v->initialLength);
    assert(v->elements != NULL);
}

void VectorDispose(vector *v)
{
    int i;
    void *elemAddr;
    if(v->freefn != NULL)
    {
        for(i=0; i < v->lastElem; i++)
        {
            elemAddr = (char *)v->elements + (i * v->elemSize);
            v->freefn(elemAddr);
        }
    }
    free(v->elements);
    v->elements = NULL;
}

int VectorLength(const vector *v)
{
    return v->lastElem;
}

void *VectorNth(const vector *v, int position)
{
    assert(position >= 0 && position <= v->lastElem);
    return (char *)v->elements + (position * v->elemSize);
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    void *destAddr;
    destAddr = VectorNth(v, position);
    memcpy(destAddr, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{

}

void VectorAppend(vector *v, const void *elemAddr)
{
    // Increase vector length if maximum has been reached
    if (v->lastElem == v->currentLength)
    {
        VectorExpand(v);
    }

    // Copy the object and increase count
    VectorReplace(v, elemAddr, v->lastElem);
    v->lastElem++;
}

void VectorDelete(vector *v, int position)
{
    void *elemAddr;
    elemAddr = VectorNth(v, position);
}

void VectorSort(vector *v, VectorCompareFunction compare)
{}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    return -1;
}

void VectorExpand(vector *v)
{
    printf(" Expanding vector from %d to %d\n",v->currentLength, v->currentLength+v->initialLength);
    v->currentLength += v->initialLength;
    v->elements = realloc(v->elements, v->currentLength * v->elemSize);
    assert(v->elements != NULL);
}

void main(){}
