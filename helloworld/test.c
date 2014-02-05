#include <stdio.h>

void main() {
    int c;
    printf("%d", EOF);

    while ( c = getchar() ){

        if (c == '\n')
            break;

        putchar(c);
    }

    printf("\nTermine!\r\n");
    return 0;
}
