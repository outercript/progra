#include <stdio.h>
#include <string.h>

void * lsearch(void *, void *, int, int, int (*cmp)(void *, void *));
int compare_int(void *, void *);
int compare_string(void *, void *);

int main(void){
    char *a[] = {"test", "me", "hello", "suro"};
    char b[] = "hello";
    char x[40];

    void *ppos;
    int i, arr_size;
    arr_size = sizeof(a)/sizeof(char *);

    for (i=0; i < arr_size; i++){
        ppos = (char *)a + i*sizeof(char *);
        printf("%d: %p = %p\n", i, a, ppos);
        printf("%d: %p = %p\n", i, a[i], ((char)ppos));

        strcpy(x, ppos);
        printf("%d: %s = %s\n", i, a[i], x);
    }

    return 0;

    ppos = lsearch(&b, a, arr_size, sizeof(char *), compare_string);
    if (ppos != NULL)
        printf("El valor \"%s\" se encuentra en la direccion: %p\n", b, (char *)ppos);
    else
        printf("El valor no se encuentra en el arreglo\n");

    return 0;
}


void * lsearch
(void *key, void *arr, int lenght, int block_size, int (*cmp)(void *, void *)){
    int i;
    void *itemAddr;
    for (i=0; i < lenght; i++){
        itemAddr = (char *)arr + i*block_size;
        if (cmp(key, itemAddr) == 0)
            return itemAddr;
    }
    return NULL;
}

int compare_string
(void *a, void *b){
    char *x = a;
    char *y = b;

    if ( a == NULL || b == NULL ) return -1;
    printf("%s = %s ?\n", x, y);

    while(1){
        if (x == y){
            if ( x == "\0")
                return 0;

            x++;
            y++;
        }
        else{
            break;
        }

    }
    return -1;
}

int compare_int
(void *a, void *b){
    int *x = a;
    int *y = b;
    return *x - *y;
}
