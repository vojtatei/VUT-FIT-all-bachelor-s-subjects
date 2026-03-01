/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief String implementation
 *
 * @author Jakub Jeřábek (xjerab28)
 */



#include "string.h"



int initStr(String *newString) {
    if (newString == NULL) return STR_ALLOC_ERR;

    newString->string = (char *)malloc(STR_LEN*sizeof(char));

    if (newString->string == NULL) return STR_ALLOC_ERR;

    newString->allocSize = STR_LEN;
    newString->len = 0;
    newString->string[0] = '\0';
    
    return STR_OK;
}

void freeStr(String *string) {
    free(string->string);
    string->string = NULL;
    free(string);
    string = NULL;
}



int addCharStr(char currSymbol, String *string) {
    if (string == NULL) return STR_ALLOC_ERR;

    if (string->len + 1 < string->allocSize) {
        string->string[string->len] = currSymbol;
        string->len++;
        string->string[string->len] = '\0';
        return STR_OK;
    }

    int newSize = string->allocSize + STR_LEN;
    string->string = (char *)realloc(string->string, newSize);

    if (string->string == NULL) return STR_ALLOC_ERR;

    string->string[string->len++] = currSymbol;
    string->string[string->len] = '\0';
    string->allocSize = newSize;

    return STR_OK;
}

int delLastCharStr(String *string) {
    if (string == NULL) return STR_ALLOC_ERR;

    if (string->len == 0) return STR_ZERO_LEN_ERR;

    string->string[--string->len] = '\0';
    
    return STR_OK;
}

int addStrStr(char *newStr, String *string) {
    if (string == NULL) return STR_ALLOC_ERR;

    if ((string->len + strlen(newStr) + 1) > string->allocSize) {
        unsigned newSize = string->allocSize;

        while (newSize < (string->len + strlen(newStr) + 1)) {
            newSize += STR_LEN;
            string->string = (char *)realloc(string->string, newSize);
            if (string->string == NULL) return STR_ALLOC_ERR;
        }
        string->allocSize = newSize;
    }

    for (int i = 0; newStr[i] != '\0'; i++) {
        string->string[string->len++] = newStr[i];
    }
    string->string[string->len] = '\0';  

    return STR_OK;
}

int reallocStr(unsigned int newSize, String *string) {
    if (newSize < string->allocSize) {
        return STR_ALLOC_ERR;
    }

    string->string = (char *) realloc(string->string, newSize);
    if (string->string == NULL) return STR_ALLOC_ERR;  
    string->allocSize = newSize;

    return 0;
}

void clearStr(String *string) {
    if (string->string == NULL) {
        return;
    }
    memset(string->string, 0, string->allocSize);
    string->len = 0;
    string->string[0] = '\0';
}

