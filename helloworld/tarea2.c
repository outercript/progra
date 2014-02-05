#include <stdio.h>

int main(void)
{
    unsigned char index = 0;
    char input[11];
    char c;

    do
    {
        c = getchar();
        input[index] = c;
        index++;

    } while (c != '\n');

    printf("%s", input);

    return 0;
}
