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

// Converte uma expressão infixa para posfixa 
char *getFormaPosFixa(char *StrInFixa){
    static char posFixa[MAX];
    char token[MAX];
    int i = 0, j = 0;
    PilhaChar operadores;
    inicializaPilhaChar(&operadores);
    
    strcpy(posFixa, "");

    while (StrInFixa[i] != '\0') {
        // Ignora espaços
        if (StrInFixa[i] == ' ') {
            i++;
            continue;
        }
        
        // Se é dígito ou ponto decimal (número)
        if (isdigit(StrInFixa[i]) || StrInFixa[i] == '.') {
            j = 0;
            while (isdigit(StrInFixa[i]) || StrInFixa[i] == '.') {
                token[j++] = StrInFixa[i++];
            }
            token[j] = '\0';
            strcat(posFixa, token);
            strcat(posFixa, " ");
        }
        // Se é uma letra (início de função)
        else if (isalpha(StrInFixa[i])) {
            j = 0;
            while (isalpha(StrInFixa[i])) {
                token[j++] = StrInFixa[i++];
            }
            token[j] = '\0';
            
            // Se é uma função conhecida
            if (ehFuncao(token)) {
                pushChar(&operadores, 'f'); // Marcador de função
                // Empilha os caracteres da função (ao contrário para desempilhar corretamente)
                for (int k = strlen(token) - 1; k >= 0; k--) {
                    pushChar(&operadores, token[k]);
                }
            }
        }
        // Se é parêntese de abertura
        else if (StrInFixa[i] == '(') {
            pushChar(&operadores, StrInFixa[i]);
            i++;
        }
        // Se é parêntese de fechamento
        else if (StrInFixa[i] == ')') {
            while (!pilhaVaziaChar(&operadores) && topoChar(&operadores) != '(') {
                char op = popChar(&operadores);
                if (op == 'f') {
                    // Reconstrói a função
                    char funcao[10];
                    int k = 0;
                    while (!pilhaVaziaChar(&operadores) && topoChar(&operadores) != '(') {
                        funcao[k++] = popChar(&operadores);
                    }
                    funcao[k] = '\0';
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
            }
            i++;
        }
        // Se é operador
        else {
            while (!pilhaVaziaChar(&operadores) && 
                   topoChar(&operadores) != '(' &&
                   prioridade(topoChar(&operadores)) >= prioridade(StrInFixa[i])) {
                char op = popChar(&operadores);
                char temp[2] = {op, '\0'};
                strcat(posFixa, temp);
                strcat(posFixa, " ");
            }
            pushChar(&operadores, StrInFixa[i]);
            i++;
        }
    }
    
    // Desempilha operadores restantes
    while (!pilhaVaziaChar(&operadores)) {
        char op = popChar(&operadores);
        if (op != '(' && op != ')') {
            char temp[2] = {op, '\0'};
            strcat(posFixa, temp);
            strcat(posFixa, " ");
        }
    }
    
    return posFixa;
}

// Calcula o valor de uma expressão pós-fixada
float getValorPosFixa(char *StrPosFixa){
    PilhaFloat pilha;
    inicializaPilhaFloat(&pilha);
    
    char *token = strtok(StrPosFixa, " ");
    
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
    char temp[MAX];
    strcpy(temp, posFixa); // Cria uma cópia pois strtok modifica a string
    return getValorPosFixa(temp);
}