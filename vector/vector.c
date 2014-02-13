#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

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
    if(v->freefn != NULL) {
        for(i=0; i < v->lastElem; i++) {
            elemAddr = VectorNth(v, i);
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
    void *currAddr = VectorNth(v, position);
    void *nextAddr = (char *)currAddr + v->elemSize;
    size_t offset = (v->lastElem - position + 1) * v->elemSize;

    if (v->lastElem == v->currentLength) {
        VectorExpand(v);
    }

    memmove(nextAddr, currAddr, offset);
    VectorReplace(v, elemAddr, position);
    v->lastElem++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    if (v->lastElem == v->currentLength) {
        VectorExpand(v);
    }

    VectorReplace(v, elemAddr, v->lastElem);
    v->lastElem++;
}

void VectorDelete(vector *v, int position)
{
    void *elemAddr = VectorNth(v, position);
    void *nextAddr = (char *)elemAddr + v->elemSize;
    size_t offset = (v->lastElem - position) * v->elemSize;

    if(v->freefn != NULL) {
        v->freefn(elemAddr);
    }

    memmove(elemAddr, nextAddr, offset);
    v->lastElem--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    qsort(v->elements, v->lastElem, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    int i;
    void *elemAddr;
    for (i=0; i < VectorLength(v); i++) {
        elemAddr = VectorNth(v, i);
        mapFn(elemAddr, auxData);
    }
}

int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{
    void *elemAddr = NULL;
    void *baseAddr = (char *)v->elements + (startIndex * v->elemSize);
    size_t elemLimit = v->lastElem - startIndex;

    if (isSorted == true) {
        elemAddr = bsearch(key, baseAddr, elemLimit, v->elemSize, searchFn);
    } else {
        elemAddr = lfind(key, baseAddr, &elemLimit, v->elemSize, searchFn);
    }

    if (elemAddr != NULL) {
        return ((char *)elemAddr - (char *)v->elements) / v->elemSize;
    }
    return kNotFound;
}

void VectorExpand(vector *v)
{
    v->currentLength += v->initialLength;
    v->elements = realloc(v->elements, v->currentLength * v->elemSize);
    assert(v->elements != NULL);
}
