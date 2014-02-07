#include <stdio.h>
#include <assert.h>

void * lsearch(void *, void *, int, int, int (*cmp)(void *, void *));
int compare_int(void *, void *);
int compare_string(void *, void *);

int main(void){
    char *a[] = {"test", "meme", "oscar", "suro", "me"};
    char *b = "me";

    void *ppos;
    int arr_size = sizeof(a)/sizeof(char *);

    ppos = lsearch(&b, a, arr_size, sizeof(char *), compare_string);
    if (ppos != NULL)
        printf("El valor \"%s\" se encuentra en la direccion: %p\n", *(char **)ppos, (char **)ppos);
    else
        printf("El valor no se encuentra en el arreglo\n");

    return 0;
}


void * lsearch(void *key, void *arr, int lenght, int block_size, int (*cmp)(void *, void *))
{
    int i;
    void *itemAddr;
    for (i=0; i < lenght; i++){
        itemAddr = (char *)arr + i*block_size;
        if (cmp(key, itemAddr) == 0)
            return itemAddr;
    }
    return NULL;
}

int compare_string(void *a, void *b)
{
    char *x = *(char **)a;
    char *y = *(char **)b;
    assert(a =! NULL && b != NULL);

    while(*x - *y == 0){
        if (*x == 0)
			return 0;
		x++;
		y++;
    }
    return *x - *y;
}

int compare_int(void *a, void *b)
{
    int *x = a;
    int *y = b;
    return *x - *y;
}
