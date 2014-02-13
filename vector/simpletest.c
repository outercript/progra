#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

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

int test_main()
{
    int i, length;
    double d;

    length = 5;
    vector test;
    VectorNew(&test, sizeof(double), NULL, length);

    printf("\n* Simple Vector Tests *\n");

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
    VectorMap(&test, (VectorMapFunction)print_double, NULL);


    // Test insert & deletion
    printf("\n\nTest Insert\n");
    d = 666.99;
    VectorInsert(&test, &d, 5);
    VectorMap(&test, (VectorMapFunction)print_double, NULL);

    printf("\n\nTest Delete\n");
    VectorDelete(&test, 5);
    VectorMap(&test, (VectorMapFunction)print_double, NULL);


    // Test linear search
    printf("\n\nSearching value in vector\n");
    d = 7.23;

    printf("offset=0: ");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 0, false);
    if (i != kNotFound)
        printf("Success: Item %.2f was found in position %d\n", d, i);
    else
        printf("Fail: Item %.2f was not found\n", d);

    printf("offset=2: ");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 2, false);
    if (i != kNotFound)
        printf("Success: Item %.2f was found in position %d\n", d, i);
    else
        printf("Fail: Item %.2f was not found\n", d);

    printf("offset=4: ");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 4, false);
    if (i != kNotFound)
        printf("Fail: Item %.2f was found in position %d\n", d, i);
    else
        printf("Success: Item %.2f was not found\n", d);

    printf("Value not in vector: ");
    d = 35.23;
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 0, false);
    if (i != kNotFound)
        printf("Fail: Item %.2f was found in position %d\n", d, i);
    else
        printf("Success: Item %.2f was not found\n", d);


    // Check sorted
    printf("\n\nSorting the vector\n");
    VectorSort(&test, (VectorCompareFunction)compare_double);
    VectorMap(&test, (VectorMapFunction)print_double, NULL);


    // Test binary search
    printf("\n\nSearching value in vector(sorted)\n");
    d = 6.0;

    printf("offset=0 :");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 0, true);
    if (i != kNotFound)
        printf("Success: Item %.2f was found in position %d\n", d, i);
    else
        printf("Fail: Item %.2f was not found\n", d);

    printf("offset=2 :");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 2, true);
    if (i != kNotFound)
        printf("Success: Item %.2f was found in position %d\n", d, i);
    else
        printf("Fail: Item %.2f was not found\n", d);

    printf("offset=4: ");
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 4, true);
    if (i != kNotFound)
        printf("Fail: Item %.2f was found in position %d\n", d, i);
    else
        printf("Success: Item %.2f was not found\n", d);

    printf("Value not in vector: ");
    d = 35.23;
    i = VectorSearch(&test, &d, (VectorCompareFunction)compare_double, 0, true);
    if (i != kNotFound)
        printf("Fail: Item %.2f was found in position %d\n", d, i);
    else
        printf("Success: Item %.2f was not found\n", d);


    VectorDispose(&test);
    return 0;
}

