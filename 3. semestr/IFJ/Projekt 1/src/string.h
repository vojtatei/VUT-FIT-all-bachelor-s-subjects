/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief String header
 *
 * @author Jakub Jeřábek (xjerab28)
 */

#ifndef STRING_H
#define STRING_H

#define STR_LEN 10

#define STR_OK 0
#define STR_ALLOC_ERR 1
#define STR_ZERO_LEN_ERR 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char *string;
    unsigned allocSize;
    unsigned len;
} String;

int initStr(String *newString);

void freeStr(String *string);

int addCharStr(char currSymbol, String *string);

int delLastCharStr(String *string);

int addStrStr(char *newStr, String *string);

void clearStr(String *string);

int reallocStr(unsigned int newSize, String *string);

#endif