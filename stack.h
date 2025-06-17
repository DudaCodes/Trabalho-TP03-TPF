#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Enum para o tipo de elemento na pilha (para maior flexibilidade)
typedef enum {
    TYPE_CHAR,    // Para operadores
    TYPE_DOUBLE,  // Para valores numéricos
    TYPE_STRING   // Para expressões intermediárias (na conversão)
} StackElementType;

// Estrutura para um elemento da pilha, usando union para diferentes tipos de dados
typedef struct {
    StackElementType type;
    union {
        char char_val;
        double double_val;
        char* string_val;
    } data;
} StackElement;

// Estrutura da Pilha
typedef struct {
    StackElement* items;
    int top;
    int capacity;
} Stack;

// Protótipos das funções da Pilha
Stack* createStack(int capacity);
void destroyStack(Stack* s);
bool isEmpty(Stack* s);
bool isFull(Stack* s);
void push(Stack* s, StackElement item);
StackElement pop(Stack* s);
StackElement peek(Stack* s);

#endif // STACK_H
