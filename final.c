#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX 512
#define PI 3.14159265359

typedef struct {
    float itens[MAX];
    int topo;
} PilhaFloat;

void inicializaPilhaFloat(PilhaFloat *pilha){
    pilha->topo = -1;
}

int pilhaVaziaFloat(PilhaFloat *pilha){
    return pilha->topo == -1;
}

void pushFloat(PilhaFloat *pilha, float valor){
    if (pilha->topo < MAX - 1) {
        pilha->itens[++pilha->topo] = valor;
    } else {
        fprintf(stderr, "Pilha cheia!\n");
    }
}

float popFloat(PilhaFloat *pilha){
    if(!pilhaVaziaFloat(pilha)){
        return pilha->itens[pilha->topo--];
    } else {
        fprintf(stderr, "Pilha vazia!\n");
        return 0.0f;
    }
}

typedef struct {
    char itens[MAX];
    int topo;
} PilhaChar;

void inicializaPilhaChar(PilhaChar *pilha){
    pilha->topo = -1;
}

int pilhaVaziaChar(PilhaChar *pilha){
    return pilha->topo == -1;
}

void pushChar(PilhaChar *pilha, char valor){
    if (pilha->topo < MAX - 1) {
        pilha->itens[++pilha->topo] = valor;
    } else {
        fprintf(stderr, "Pilha cheia!\n");
    }
}

char popChar(PilhaChar *pilha){
    if(!pilhaVaziaChar(pilha)){
        return pilha->itens[pilha->topo--];
    } else {
        fprintf(stderr, "Pilha vazia!\n");
        return '\0';
    }
}

char topoChar(PilhaChar *pilha){
    if(!pilhaVaziaChar(pilha)){
        return pilha->itens[pilha->topo];
    } else {
        return '\0';
    }
}

// Função auxiliar: converte graus para radianos
float grausParaRadianos(float graus) {
    return graus * PI / 180.0;
}

// Função auxiliar: prioridade dos operadores
int prioridade(char operador) {
    switch (operador) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// Função auxiliar: verifica se é uma função matemática
int ehFuncao(char *token) {
    return (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
            strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 ||
            strcmp(token, "raiz") == 0);
}
// verificar se um caractere é um operador binário
int ehOperadorBinario(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

// Converte uma expressão infixa para posfixa
char *getFormaPosFixa(char *StrInFixa){
    static char posFixa[MAX];
    char token[MAX];
    int i = 0, j = 0;
    PilhaChar operadores;
    inicializaPilhaChar(&operadores);

    strcpy(posFixa, "");

    // Adicionado: Cria uma cópia temporária e adiciona espaço no final para facilitar o parsing.
    char temp_infixa[MAX + 2];
    strcpy(temp_infixa, StrInFixa);
    strcat(temp_infixa, " ");

    while (temp_infixa[i] != '\0') { // Usa temp_infixa para iteração.
        // Ignora espaços
        if (temp_infixa[i] == ' ') {
            i++;
            continue;
        }

        // Se é dígito ou ponto decimal (número)
        if (isdigit(temp_infixa[i]) || temp_infixa[i] == '.') {
            j = 0;
            while (isdigit(temp_infixa[i]) || temp_infixa[i] == '.') {
                token[j++] = temp_infixa[i++]; // Consome caracteres de temp_infixa.
            }
            token[j] = '\0';
            strcat(posFixa, token);
            strcat(posFixa, " ");
        }
        // Se é uma letra (início de função)
        else if (isalpha(temp_infixa[i])) { // Verifica em temp_infixa.
            j = 0;
            while (isalpha(temp_infixa[i])) {
                token[j++] = temp_infixa[i++]; // Consome caracteres de temp_infixa.
            }
            token[j] = '\0';

            // Se é uma função conhecida
            if (ehFuncao(token)) {
                pushChar(&operadores, 'f'); // Marcador de função.
                // Empilha os caracteres da função (ao contrário para desempilhar corretamente)
                for (int k = strlen(token) - 1; k >= 0; k--) {
                    pushChar(&operadores, token[k]);
                }
            } else {
                // Adicionado: Tratamento de erro para função desconhecida.
                fprintf(stderr, "Erro: Função desconhecida '%s' na expressão infixa.\n", token);
                strcpy(posFixa, "ERRO"); // Sinaliza erro na string de saída.
                return posFixa;
            }
        }
        // Se é parêntese de abertura
        else if (temp_infixa[i] == '(') { // Verifica em temp_infixa.
            pushChar(&operadores, temp_infixa[i]);
            i++;
        }
        // Se é parêntese de fechamento
        else if (temp_infixa[i] == ')') { // Verifica em temp_infixa.
            while (!pilhaVaziaChar(&operadores) && topoChar(&operadores) != '(') {
                char op = popChar(&operadores);
                if (op == 'f') {
                    // Reconstrói a função
                    char funcao[10];
                    int k = 0;
                    // Modificado: Adicionado `&& topoChar(&operadores) != 'f'` para parar no marcador de função.
                    while (!pilhaVaziaChar(&operadores) && topoChar(&operadores) != '(' && topoChar(&operadores) != 'f') {
                        funcao[k++] = popChar(&operadores);
                    }
                    funcao[k] = '\0';
           //inverte a string da função, pois foi empilhada ao contrário.
          for (int start = 0, end = k - 1; start < end; start++, end--) {
            char temp = funcao[start];
            funcao[start] = funcao[end];
            funcao[end] = temp;
          }
             strcat(posFixa, funcao);
             strcat(posFixa, " ");

                } else {
                    char temp[2] = {op, '\0'};
                    strcat(posFixa, temp);
                    strcat(posFixa, " ");
                }
            }
            if (!pilhaVaziaChar(&operadores)) {
                popChar(&operadores); // Remove o '('
            }else {
             // Adicionado: Tratamento de erro para parênteses não balanceados.
                fprintf(stderr, "Erro: Parênteses não balanceados (falta '(') na expressão infixa.\n");
              strcpy(posFixa, "ERRO");
                 return posFixa;
            }

            i++;
        }
        // Se é operador
        else {
             // Lógica para desempilhar operadores de maior ou igual prioridade.
             while (!pilhaVaziaChar(&operadores) &&
                    topoChar(&operadores) != '(' &&
                    prioridade(topoChar(&operadores)) >= prioridade(temp_infixa[i])) { // Usa temp_infixa.
                 char op = popChar(&operadores);
                 // Modificado: Lógica para marcador de função 'f' ao desempilhar operadores.
                 if (op == 'f') {
                     char funcao[10];
                     int k = 0;
                     while (!pilhaVaziaChar(&operadores) && topoChar(&operadores) != '(' && topoChar(&operadores) != 'f') {
                         funcao[k++] = popChar(&operadores);                     }
                     funcao[k] = '\0';
                     for (int start = 0, end = k - 1; start < end; start++, end--) {
                         char temp = funcao[start];
                         funcao[start] = funcao[end];
                         funcao[end] = temp;
                     }
                     strcat(posFixa, funcao);
                     strcat(posFixa, " ");
                 } else {
                    char temp[2] = {op, '\0'};
                     strcat(posFixa, temp);
                     strcat(posFixa, " ");
                 }
            }
             // Adicionado: Se o caractere não for um operador binário conhecido, é um erro.
             if (!ehOperadorBinario(temp_infixa[i])) { // Usa temp_infixa
                 fprintf (stderr, "Erro: Caractere inválido na expressão infixa: '%c' (posição %d).\n", temp_infixa[i], i);
                 strcpy(posFixa, "ERRO");
                 return posFixa;
             }
            pushChar(&operadores, temp_infixa[i]);
            i++;
        }
    }

    // Desempilha operadores restantes
    while (!pilhaVaziaChar(&operadores)) {
     char op = popChar(&operadores);
    if (op == '(' || op == ')') {
    // Adicionado: Tratamento de erro para parênteses não balanceados.
             fprintf(stderr, "Erro: Parênteses não balanceados (falta ')') na expressão infixa.\n");
             strcpy(posFixa, "ERRO");
             return posFixa;
         }
    // Modificado: Lógica para marcador de função 'f' ao desempilhar operadores restantes.
         if (op == 'f') {
             char funcao[10];
             int k = 0;
             while (!pilhaVaziaChar(&operadores) && topoChar(&operadores) != '(' && topoChar(&operadores) != 'f') {
                 funcao[k++] = popChar(&operadores);
             }
             funcao[k] = '\0';
             for (int start = 0, end = k - 1; start < end; start++, end--) {
                 char temp = funcao[start];
                 funcao[start] = funcao[end];
                 funcao[end] = temp;
             }
             strcat(posFixa, funcao);
             strcat(posFixa, " ");
         } else {
             char temp[2] = {op, '\0'};
           strcat(posFixa, temp);
             strcat(posFixa, " ");
         }
    }

     return posFixa;
 }

// Adicionado: Converte uma expressão pós-fixa para infixa
char *getFormaInFixa(char *StrPosFixa) {
    static char inFixaResult[MAX]; // Usamos static, com a mesma ressalva de reentrância
    PilhaChar pilhaOperandos; // Pilha para armazenar substrings infixas
    inicializaPilhaChar(&pilhaOperandos);

    // Cria uma cópia da string, pois strtok a modifica
    char tempPosFixa[MAX];
    strcpy(tempPosFixa, StrPosFixa);

    char *token = strtok(tempPosFixa, " ");

    while (token != NULL) {
        // Se é um número
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])) || token[0] == '.') {
            // Empilha o número completo
            for(int i = 0; i < strlen(token); i++) {
                pushChar(&pilhaOperandos, token[i]);
            }
            pushChar(&pilhaOperandos, '|'); // Delimitador especial para separar operandos na pilha
        }
        // Se é operador binário
        else if (ehOperadorBinario(token[0]) && strlen(token) == 1) {
            if (pilhaVaziaChar(&pilhaOperandos)) {
                fprintf(stderr, "Erro: Expressão posfixa inválida, falta operando para '%c'.\n", token[0]);
                strcpy(inFixaResult, "ERRO");
                return inFixaResult;
            }
            // Desempilha o segundo operando (B)
            char opB_buff[MAX]; int k = 0;
            while (!pilhaVaziaChar(&pilhaOperandos) && topoChar(&pilhaOperandos) != '|') {
                opB_buff[k++] = popChar(&pilhaOperandos);
            }
            if(pilhaVaziaChar(&pilhaOperandos)) { // Verifica se o delimitador foi encontrado
                fprintf(stderr, "Erro: Expressão posfixa inválida, delimitador ausente para operando B.\n");
                strcpy(inFixaResult, "ERRO");
                return inFixaResult;
            }
            popChar(&pilhaOperandos); // Remove o '|'
            opB_buff[k] = '\0';
            // Inverte opB (pois foi desempilhado em ordem reversa)
            for (int start = 0, end = k - 1; start < end; start++, end--) {
                char temp = opB_buff[start];
                opB_buff[start] = opB_buff[end];
                opB_buff[end] = temp;
            }

            if (pilhaVaziaChar(&pilhaOperandos)) {
                fprintf(stderr, "Erro: Expressão posfixa inválida, falta operando para '%c'.\n", token[0]);
                strcpy(inFixaResult, "ERRO");
                return inFixaResult;
            }
            // Desempilha o primeiro operando (A)
            char opA_buff[MAX]; k = 0;
            while (!pilhaVaziaChar(&pilhaOperandos) && topoChar(&pilhaOperandos) != '|') {
                opA_buff[k++] = popChar(&pilhaOperandos);
            }
            if(pilhaVaziaChar(&pilhaOperandos)) { // Verifica se o delimitador foi encontrado
                fprintf(stderr, "Erro: Expressão posfixa inválida, delimitador ausente para operando A.\n");
                strcpy(inFixaResult, "ERRO");
                return inFixaResult;
            }
            popChar(&pilhaOperandos); // Remove o '|'
            opA_buff[k] = '\0';
            // Inverte opA
            for (int start = 0, end = k - 1; start < end; start++, end--) {
                char temp = opA_buff[start];
                opA_buff[start] = opA_buff[end];
                opA_buff[end] = temp;
            }

            // Constrói a subexpressão infixa: (A operador B)
            char subExpressao[MAX];
            strcpy(subExpressao, "(");
            strcat(subExpressao, opA_buff);
            strcat(subExpressao, token); // Adiciona o operador
            strcat(subExpressao, opB_buff);
            strcat(subExpressao, ")");

            // Empilha a subexpressão de volta na pilha
            for(int i = 0; i < strlen(subExpressao); i++) {
                pushChar(&pilhaOperandos, subExpressao[i]);
            }
            pushChar(&pilhaOperandos, '|'); // Delimitador
        }
        // Se é função unária
        else if (ehFuncao(token)) {
            if (pilhaVaziaChar(&pilhaOperandos)) {
                fprintf(stderr, "Erro: Expressão posfixa inválida, falta operando para função '%s'.\n", token);
                strcpy(inFixaResult, "ERRO");
                return inFixaResult;
            }
            // Desempilha o operando
            char op_buff[MAX]; int k = 0;
            while (!pilhaVaziaChar(&pilhaOperandos) && topoChar(&pilhaOperandos) != '|') {
                op_buff[k++] = popChar(&pilhaOperandos);
            }
            if(pilhaVaziaChar(&pilhaOperandos)) { // Verifica se o delimitador foi encontrado
                fprintf(stderr, "Erro: Expressão posfixa inválida, delimitador ausente para operando da função.\n");
                strcpy(inFixaResult, "ERRO");
                return inFixaResult;
            }
            popChar(&pilhaOperandos); // Remove o '|'
            op_buff[k] = '\0';
            // Inverte op_buff
            for (int start = 0, end = k - 1; start < end; start++, end--) {
                char temp = op_buff[start];
                op_buff[start] = op_buff[end];
                op_buff[end] = temp;
            }

            // Constrói a subexpressão: função(operando)
            char subExpressao[MAX];
            strcpy(subExpressao, token); // Nome da função
            strcat(subExpressao, "(");
            strcat(subExpressao, op_buff);
            strcat(subExpressao, ")");

            // Empilha a subexpressão
            for(int i = 0; i < strlen(subExpressao); i++) {
                pushChar(&pilhaOperandos, subExpressao[i]);
            }
            pushChar(&pilhaOperandos, '|'); // Delimitador
        } else {
            fprintf(stderr, "Erro: Token inválido na expressão posfixa: '%s'.\n", token);
            strcpy(inFixaResult, "ERRO");
            return inFixaResult;
        }
        token = strtok(NULL, " ");
    }

    // O resultado final deve ser o único item na pilha
    if (pilhaVaziaChar(&pilhaOperandos)) {
        fprintf(stderr, "Erro: Expressão posfixa vazia ou inválida (nenhum resultado).\n");
        strcpy(inFixaResult, "ERRO");
        return inFixaResult;
    }

    char final_buff[MAX]; int k = 0;
    while (!pilhaVaziaChar(&pilhaOperandos) && topoChar(&pilhaOperandos) != '|') {
        final_buff[k++] = popChar(&pilhaOperandos);
    }
    if(!pilhaVaziaChar(&pilhaOperandos) && topoChar(&pilhaOperandos) == '|') {
        popChar(&pilhaOperandos); // Remove o '|' final
    }

    final_buff[k] = '\0';
    // Inverte o buffer final para obter a string infixa correta
    for (int start = 0, end = k - 1; start < end; start++, end--) {
        char temp = final_buff[start];
        final_buff[start] = final_buff[end];
        final_buff[end] = temp;
    }

    if (!pilhaVaziaChar(&pilhaOperandos)) {
        fprintf(stderr, "Erro: Expressão posfixa inválida, operandos restantes inesperadamente na pilha.\n");
        strcpy(inFixaResult, "ERRO");
        return inFixaResult;
    }

    strcpy(inFixaResult, final_buff);
    return inFixaResult;
}

// Calcula o valor de uma expressão pós-fixada
float getValorPosFixa(char *StrPosFixa){
    PilhaFloat pilha;
    inicializaPilhaFloat(&pilha);

    // Cria uma cópia da string, pois strtok a modifica
    char temp_posFixa[MAX];
    strcpy(temp_posFixa, StrPosFixa);

    char *token = strtok(temp_posFixa, " "); // Usa temp_posFixa aqui

    while (token != NULL) {
        // Se é número
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])) || token[0] == '.') {
            pushFloat(&pilha, atof(token));
        }


        // Se é operador binário
        else if (strlen(token) == 1 && strchr("+-*/%^", token[0])) {
            if (pilha.topo < 1) {
                fprintf(stderr, "Erro: Expressão inválida\n");
                return 0.0f;
            }

            float b = popFloat(&pilha);
            float a = popFloat(&pilha);
            float resultado = 0.0f;

            switch (token[0]) {
                case '+':
                    resultado = a + b;
                    break;
                case '-':
                    resultado = a - b;
                    break;
                case '*':
                    resultado = a * b;
                    break;
                case '/':
                    if (b != 0) {
                        resultado = a / b;
                    } else {
                        fprintf(stderr, "Erro: Divisão por zero.\n");
                        return 0.0f;
                    }
                    break;
                case '%':
                    resultado = fmod(a, b);
                    break;
                case '^':
                    resultado = pow(a, b);
                    break;
            }
            pushFloat(&pilha, resultado);
        }
        // Se é função unária
        else if (ehFuncao(token)) {
            if (pilha.topo < 0) {
                fprintf(stderr, "Erro: Expressão inválida\n");
                return 0.0f;
            }

            float a = popFloat(&pilha);
            float resultado = 0.0f;

            if (strcmp(token, "sen") == 0) {
                resultado = sin(grausParaRadianos(a));
            } else if (strcmp(token, "cos") == 0) {
                resultado = cos(grausParaRadianos(a));
            } else if (strcmp(token, "tg") == 0) {
                // Adicionado: Verificação para tangente de 90, 270, etc. (indefinida)
                float angle_mod_180 = fmod(fabs(a), 180.0f);
                if (fabs(angle_mod_180 - 90.0f) < 0.0001f || fabs(angle_mod_180 - 270.0f) < 0.0001f) {
                    fprintf(stderr, "Erro: Tangente de ângulo inválido (múltiplo de 90/270).\n");
                    return 0.0f;
                }
                resultado = tan(grausParaRadianos(a));
            } else if (strcmp(token, "log") == 0) {
                if (a > 0) {
                    resultado = log10(a);
                } else {
                    fprintf(stderr, "Erro: Logaritmo de número não positivo.\n");
                    return 0.0f;
                }
            } else if (strcmp(token, "raiz") == 0) {
                if (a >= 0) {
                    resultado = sqrt(a);
                } else {
                    fprintf(stderr, "Erro: Raiz quadrada de número negativo.\n");
                    return 0.0f;
                }
            }
            pushFloat(&pilha, resultado);
        }

        token = strtok(NULL, " ");
    }

    if (pilha.topo == 0) {
        return popFloat(&pilha);
    } else {
        fprintf(stderr, "Erro: Expressão inválida\n");
        return 0.0f;
    }
}

// Calcula o valor de uma expressão infixa
float getValorInFixa(char *StrInFixa) {
    char *posFixa = getFormaPosFixa(StrInFixa);
    // Verificação de erro na conversão para pós-fixa.
    if (strcmp(posFixa, "ERRO") == 0) {
        return 0.0f;
    }
    char temp[MAX];
    strcpy(temp, posFixa); // Cria uma cópia pois strtok modifica a string
    return getValorPosFixa(temp);
}
