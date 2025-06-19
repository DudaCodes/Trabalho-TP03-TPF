#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"
#include "expressao.c"

int main() {
    char infixa[] = "(10 + 5) * 2";
    char posfixa[512];
    strcpy(posfixa, getFormaPosFixa(infixa));
    char copia[512];
    strcpy(copia, posfixa); 

    printf("Expressao Infixa: %s\n", infixa);
    printf("Expressao Posfixa: %s\n", posfixa);
    printf("Resultado: %.2f\n", getValorPosFixa(copia));

    return 0;
}
