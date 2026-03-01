/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Zásobník na zpracování výrazů
 *
 * @author Vojtech Teichmann ()
 */

#include "stack.h"

// Funkce pro inicializaci zásobníku
void stackInit(Stack *s) {
    s->top = NULL;
}

// Funkce pro zjištění, zda je zásobník prázdný
int stackIsEmpty(Stack *s) {
    return s->top == NULL;
}


// Funkce pro vložení prvku na zásobník
void stackPush(Stack *s, Token token) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        // Chybová manipulace, nedostatek paměti
        exit(1);
    }
    newNode->token = token;
    newNode->next = s->top;
    s->top = newNode;
}

// Funkce pro odebrání prvku ze zásobníku
Token stackPop(Stack *s) {
    if (stackIsEmpty(s)) {
        // Chybová manipulace, zásobník je prázdný
        fprintf(stderr, "Chyba: Chybí operand\n");
        exit(2);
    }
    StackNode *temp = s->top;
    Token token = temp->token;
    s->top = temp->next;
    free(temp);
    return token;
}

Token stackTop(Stack *s) {
    if (s->top == NULL) {
        // Chybová manipulace, zásobník je prázdný
        exit(2); // Nebo lepší formu chybového řízení
    }
    return s->top->token;  // Vrátí token z vrcholu zásobníku bez jeho odebrání
}

void stackDestroy(Stack *s) {
    while (!stackIsEmpty(s)) {
        // Odebereme prvek ze zásobníku a uvolníme jeho paměť
        StackNode *temp = s->top;
        s->top = s->top->next;
        free(temp);
    }
}
