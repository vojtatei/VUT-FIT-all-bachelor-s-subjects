/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Hlavičkový soubor pro funkci na zpracování výrazu
 *
 * @author Vojtech Teichmann ()
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "symstack.h"
#include "scanner.h"
#include "error.h"
#include "symtable.h"
#include "stack.h"
#include "string.h"


typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VOID
} TypeEnum;


typedef enum {
    SYMBOL_PLUS,         // +
    SYMBOL_MINUS,        // -
    SYMBOL_MULT,         // *
    SYMBOL_DIV,          // /
    SYMBOL_LTN,          // <
    SYMBOL_GTN,          // >
    SYMBOL_LEQ,          // <=
    SYMBOL_GEQ,          // >=
    SYMBOL_EQ,           // ==
    SYMBOL_NEQ,          // !=
    SYMBOL_NOT,          // !
    SYMBOL_DQUEST,       // ??
    SYMBOL_LEFT_BRACKET, // (
    SYMBOL_RIGHT_BRACKET, // )
    SYMBOL_ID,           // id
    SYMBOL_CEL_CIS,          // cel cislo
    SYMBOL_DES_CIS,      // des_cis
    SYMBOL_RET,          // ret
    SYMBOL_NIL,          // nil
    SYMBOL_DOLLAR       // $
} PrecTableSymbolEnum;

typedef struct
{
    int index;
    bool canIntDoubleCoexist;
    bool comparisonOperationFound;
}helpStruct;



typedef struct {
    TypeEnum type;        // INT, DOUBLE, STRING, etc. (tohle se nemění)
    Token *postFix;       // Pole tokenu postfixu, předtím to byly jen value
    int postFixLen;       // Délka postfixu, mohu uložit na konci funkce infixtopostfix
    bool allIdentifiersKnown;
    Tdata *variables[100];

} expressionStruct;

/**
 * @brief Vygenerovani vyrazu
 * @return expressionStruct se zpracovanym vyrazem
*/
expressionStruct createExpression(FILE *swiftFile, Tsymstack *symstack, Token *firstToken);

void convertPostfixToString(const Token *postfixExpression, unsigned length, expressionStruct *exprData) ;

void doOperation(Stack *stack, Token currentToken, Token *postfixExpression, unsigned *postfixExpressionLength);

Token* convertInfixToPostfix(FILE* swiftFile, unsigned* postfixExpressionLength, Tsymstack *symstack, expressionStruct *exprData, Token *firstToken, helpStruct *help);

void doReduce(Stack *stack, Token currentToken, Token *postfixExpression, unsigned *postfixExpressionLength);

void getSymbolFromToken(Token* token, helpStruct* mapping);

void checkPostfixDataTypes(const Token *postfixExpression, unsigned length, Tsymstack *symstack, helpStruct *help, expressionStruct *exprData);

TokenType convertDataTypeToTokenType(dataTypeT dataType);


#endif