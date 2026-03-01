/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symstack implementation
 *
 * @author Monika Záhradníková (xzahra33)
 */

#include "symstack.h"
#include "symtable.h"

Tsymstack* symstackInit() {
    Tsymstack *stack = (Tsymstack*)malloc(sizeof(Tsymstack));
    if (stack != NULL) {
        stack->stackArray = NULL;
        stack->topIndex = -1;
        return stack;
    }
    return NULL;
}


void symstackPush(Tsymstack *stack, Tnode *tree) {
    if (stack != NULL) {
        stack->topIndex++;
        stack->stackArray = (Tnode **)realloc(stack->stackArray, (stack->topIndex + 1) * sizeof(Tnode));
        if (stack->stackArray != NULL) {
            stack->stackArray[stack->topIndex] = tree;
        } else {
            fprintf(stderr, "symstackPush nealokoval pamäť\n");
            exit(99);
        }
    }
}


void symstackPop(Tsymstack *stack) {
    if (stack != NULL && stack->topIndex >= 0) {
        symtableDispose(&(stack->stackArray[stack->topIndex]));
        free(stack->stackArray[stack->topIndex]);
        stack->stackArray[stack->topIndex] = NULL;
        stack->topIndex--;
    }
}

Tnode* symstackTop(Tsymstack *stack){
    if (stack != NULL && stack->topIndex >= 0) {
        Tnode *top = stack->stackArray[stack->topIndex];
        if (top == NULL){
            stack->topIndex--;
        }
        return top;
    }

    return NULL;
}


Tdata* symstackSearchiId(Tsymstack *stack, char *idName) {
    if (stack != NULL && idName != NULL){
        if (stack->topIndex >= 0) {
            int i = stack->topIndex;
            while (i >= 0){
                Tnode *tree = stack->stackArray[i];
                Tsymsearch searchRet = symtableSearch(&tree, idName);
                if (searchRet.found == true) {

                    return searchRet.found_data;
                }
                else {
                    i--;
                }
            }
        }
    }
    return NULL;
}


void symstackDispose(Tsymstack *stack) {
    if (stack != NULL) {
        if (stack->topIndex == 0) {
            free(stack->stackArray[stack->topIndex]->attributes);  //Uvoľnenie pamäte pre Tdata
            free(stack->stackArray[stack->topIndex]->key);  // Uvoľnenie pamäte pre kľúč.
            free(stack->stackArray[stack->topIndex]);
            stack->stackArray[stack->topIndex] = NULL;
            stack->topIndex--;
        }
        else {
            while (stack->topIndex >= 0) {
                symtableDispose(&(stack->stackArray[stack->topIndex]));
                free(stack->stackArray[stack->topIndex]);
                stack->stackArray[stack->topIndex] = NULL;
                stack->topIndex--;
            }
        }

        free(stack->stackArray);
        stack->stackArray = NULL;
        free(stack);
        stack = NULL;
    }
}
