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
    if(v->freefn != NULL) {
        for(i=0; i < v->lastElem; i++) {
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
    if (v->lastElem == v->currentLength) {
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

    if (isSorted == true) {
        elemAddr = bsearch(key, baseAddr, v->lastElem, v->elemSize, searchFn);
    }

    if (elemAddr != NULL) {
        return ((char *)elemAddr - (char *)v->elements) / v->elemSize;
    }
    return kNotFound;
}

void VectorExpand(vector *v)
{
    printf(" Expanding vector from %d to %d\n",v->currentLength, v->currentLength+v->initialLength);
    v->currentLength += v->initialLength;
    v->elements = realloc(v->elements, v->currentLength * v->elemSize);
    assert(v->elements != NULL);
}




int compare_double(void *a, void *b)
{
    double *x = a;
    double *y = b;
    return *x - *y;
}

void print_double(void *elemAddr, void *auxData)
{
    printf("%.2f, ", *(double *)elemAddr);
}

int main()
{
    int i, length;
    double d;
    void *p;

    length = 5;
    vector test;
    VectorNew(&test, sizeof(double), NULL, length);

    // Test filling up the Vector
    printf("Test Fill Vector\n");
    for (i=0; i<length; i++) {
        d = 1.5*i;
        VectorAppend(&test, &d);
    }
    VectorMap(&test, (VectorMapFunction)print_double, NULL);

    // Replace test
    printf("\n\nTest Replace Element\n");
    d = 7.23;
    VectorReplace(&test, &d, 3);
    VectorMap(&test, (VectorMapFunction)print_double, NULL);

    // Append when full test
    printf("\n\nTest Appending to a full vector\n");
    printf(" (Before) Vector.length = %d\n", VectorLength(&test));
    d = 9.43;
    VectorAppend(&test, &d);
    printf(" (After) Vector.lenght = %d\n", VectorLength(&test));

    // Test printing the vector
    VectorMap(&test, (VectorMapFunction)print_double, NULL);

    // Check sorted
    printf("\n\nSorting the vector\n");
    VectorSort(&test, (VectorCompareFunction)compare_double);
    VectorMap(&test, (VectorMapFunction)print_double, NULL);

    // Test performing binary search on vector
    printf("\n\nSearching value in vector(sorted)\n");
    d = 6.0;
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 0, true);
    if (i != kNotFound)
        printf("Success: Item %.2f was found in position %d\n", d, i);
    else
        printf("Fail: Item %.2f was not found\n", d);

    printf("\nSearching value in vector(sorted) w/ offset=2\n");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 2, true);
    if (i != kNotFound)
        printf("Success: Item %.2f was found in position %d\n", d, i);
    else
        printf("Fail: Item %.2f was not found\n", d);

    printf("\nSearching value in vector(sorted) w/ offset=4\n");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 4, true);
    if (i != kNotFound)
        printf("Fail: Item %.2f was found in position %d\n", d, i);
    else
        printf("Success: Item %.2f was not found\n", d);

    printf("\nSearching value not in vector(sorted)\n");
    d = 35.23;
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 0, true);
    if (i != kNotFound)
        printf("Fail: Item %.2f was found in position %d\n", d, i);
    else
        printf("Success: Item %.2f was not found\n", d);
    return 0;
}
