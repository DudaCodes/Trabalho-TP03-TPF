#include <stdio.h>
#include "expressao.h"
#include "expressao.c"

int main() {
    char infixa[] = "3 + 4 * 2";
    printf("Pos-fixa: %s\n", getFormaPosFixa(infixa));

    char posfixa[] = "3 4 2 * +";
    printf("Valor: %.2f\n", getValorPosFixa(posfixa));

    return 0;
}
