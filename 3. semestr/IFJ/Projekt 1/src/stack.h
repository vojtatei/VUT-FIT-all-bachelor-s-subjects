/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Hlavičkový soubor pro zásobník na zpracování výrazů
 *
 * @author Vojtech Teichmann ()
 */


#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h" 


// Element zásobníku
typedef struct StackNode {
    Token token;         // Token získaný ze scanneru
    struct StackNode *next;
} StackNode;

// Struktura pro zásobník
typedef struct {
    StackNode *top;
} Stack;

void stackInit(Stack *s);

int stackIsEmpty(Stack *s);

void stackPush(Stack *s, Token token);

Token stackPop(Stack *s);

Token stackTop(Stack *s);

void stackDestroy(Stack *s);

#endif