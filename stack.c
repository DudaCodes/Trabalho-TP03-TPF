#include "stack.h"
#include <stdio.h> // Para perror, fprintf

Stack* createStack(int capacity) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        perror("Failed to allocate stack");
        exit(EXIT_FAILURE);
    }
    s->capacity = capacity;
    s->top = -1;
    s->items = (StackElement*)malloc(sizeof(StackElement) * capacity);
    if (!s->items) {
        perror("Failed to allocate stack items");
        free(s);
        exit(EXIT_FAILURE);
    }
    return s;
}

void destroyStack(Stack* s) {
    if (s) {
        // Libera strings se houverem elementos do tipo string na pilha
        for (int i = 0; i <= s->top; i++) {
            if (s->items[i].type == TYPE_STRING && s->items[i].data.string_val) {
                free(s->items[i].data.string_val);
                s->items[i].data.string_val = NULL;
            }
        }
        free(s->items);
        s->items = NULL;
        free(s);
        s = NULL;
    }
}

bool isEmpty(Stack* s) {
    return s->top == -1;
}

bool isFull(Stack* s) {
    return s->top == s->capacity - 1;
}

void push(Stack* s, StackElement item) {
    if (isFull(s)) {
        // Redimensionar a pilha se estiver cheia
        s->capacity *= 2;
        s->items = (StackElement*)realloc(s->items, sizeof(StackElement) * s->capacity);
        if (!s->items) {
            perror("Failed to reallocate stack");
            destroyStack(s); // Tentativa de liberar recursos alocados
            exit(EXIT_FAILURE);
        }
    }
    s->items[++(s->top)] = item;
}

StackElement pop(Stack* s) {
    if (isEmpty(s)) {
        fprintf(stderr, "Stack Underflow! Attempted to pop from an empty stack.\n");
        // Retornar um elemento inválido ou causar falha.
        // Neste contexto, falha crítica é aceitável, pois indica um erro lógico na expressão.
        exit(EXIT_FAILURE);
    }
    return s->items[(s->top)--];
}

StackElement peek(Stack* s) {
    if (isEmpty(s)) {
        fprintf(stderr, "Stack is empty, cannot peek.\n");
        // Retornar um elemento "nulo" ou tratar de forma apropriada
        StackElement empty_element;
        empty_element.type = TYPE_CHAR;
        empty_element.data.char_val = '\0';
        return empty_element;
    }
    return s->items[s->top];
}
