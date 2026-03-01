/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Scanner header
 *
 * @author Jakub Jeřábek (xjerab28)
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


#include "debug.h"
#include "string.h"

typedef enum {
    TOK_add,
    TOK_sub,
    TOK_arrow,
    TOK_mul,
    TOK_div,
    TOK_less,
    TOK_lessEq,
    TOK_great,
    TOK_greatEq,
    TOK_equal,
    TOK_compEq,
    TOK_doubleQuest,
    TOK_exclam, // !
    TOK_notEq,
    TOK_keyword,
    TOK_identifier,
    TOK_int,
    TOK_float,
    TOK_string,
    TOK_colon,
    TOK_openRoundBracket,
    TOK_closeRoundBracket,
    TOK_openCurlyBracket,
    TOK_closeCurlyBracket,
    TOK_comma,
    TOK_underScore,
    TOK_lexError,
    TOK_EOF,
    TOK_EOL
} TokenType;

typedef enum {
    KW_double,
    KW_int,
    KW_string,
    KW_doubleQ,
    KW_intQ,
    KW_stringQ,
    KW_else,
    KW_func,
    KW_if,
    KW_let,
    KW_nil,
    KW_return,
    KW_var,
    KW_while,
    KW_COUNT
} Keyword;


extern const char* tokenTypeStrings[];
extern const char *keywords[];

typedef struct {
    char *string;
    int num;
    double numFloat;
    Keyword keyword;
} TokenValue;
typedef struct {
    TokenType tokenType;
    TokenValue tokenValue;
} Token;


/**
 * @brief funkce scanneru pro ukonceni a uvolneni pameti
 * @param errorCode koncovy error codde
 * @param string dynamicky string
*/
int handleError(int errorCode, String *string);

/**
 * @brief procesovani celeho cisla
 * @param string dynamicky string
 * @param token token pro vraceni
*/
int processInt(String *string, Token *token);

/**
 * @brief procesovani desetinneho cisla
 * @param string dynamicky string
 * @param token token pro vraceni
*/
int processFloat(String *string, Token *token);

/**
 * @brief procesovani znaku "?" za datovym typem
 * @param token token pro vraceni
 * @param swiftFile vstup
 * 
*/
int processQueryKeyword(Token *token, FILE *swiftFile);

/**
 * @brief procesovani identifikatoru
 * @param string dynamicky string
 * @param token token pro vraceni
 * @param swiftFile vstup
*/
int processId(String *string, Token *token, FILE *swiftFile);


/**
 * @brief procesovani stringu
 * @param string dynamicky string
 * @param token token pro vraceni
 * @param swiftFile vstup
 * @param isMultiLine 1 pokud se jedna o vice-radkovy retezec
*/
int processString(String *string, Token *token, FILE *swiftFile, bool isMultiLine);

/**
 * @brief procesovani vice-radkoveho komentare
 * @param swiftFile vstup
 * @param commentDepth pocitadlo zanoreni
*/
int processMultiComm(FILE *swiftFile, int *commentDepth);

/**
 * @brief procesovani radkoveho komentare
 * @param swiftFile vstup
*/
int processLineComm(FILE *swiftFile);


/**
 * @brief hlavni funkce scanneru
 * @param token token pro vraceni
 * @param swiftFile vstup
 * @returns nacteny token
*/
int getToken(Token *token, FILE *swiftFile);

#endif