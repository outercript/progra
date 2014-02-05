#include <stdio.h>
#include <math.h>

void buscarPrimoCercano(void)
{
    int input, resultado;
    double square, delta;
    printf("Introduce un numero positivo: ");
    scanf("%d",&input);

    square = sqrt(input);
    delta = fabs(square - (int) square);

    //printf("La raiz de este numero es: %.4f\n", square);
    //printf("La delta de este numero es: %.4f\n", delta);

    if (delta >= 0.5)
        resultado = (int)square + 1;
    else
        resultado = (int)square;

    resultado *= resultado;
    printf("El cuadrado mas cercano es: %d\n", resultado);
}

int main(void)
{
    int n,i;
    int d2, count; // Typo tenia coma en vez de punto y coma
    double d1;

    while(1)
    buscarPrimoCercano();

    while (1)
    {
        printf("Introduce un numero (0 para salir): ");
        scanf("%d",&n);
        if (n == 0)
            break;

        count=0;

        // Division sobre zero porque i = 0 en la primera iteracion, incrementar i y cambiar el limite del for.
        for (i=1; i<=n; i++)
        {
            d1=(double)n/(double)i;
            d2=n/i;

            if (fabs(d1-(double)d2) < 0.00001)
                count++;
        }

    if (count == 2)
        printf("%d es primo\n",n);

    else
        printf("%d no es primo\n",n);
    }


    return 0; // Falta el return value
}
