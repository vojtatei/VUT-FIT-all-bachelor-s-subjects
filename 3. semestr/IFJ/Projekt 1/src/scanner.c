/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Scanner main file
 *
 * @author Jakub Jeřábek (xjerab28)
 */


//#define DEBUG

#include "scanner.h"

#define LEX_OK 0
#define LEX_ERROR 1
#define LEX_END 2
#define KW_TABLE_SIZE_NOT_EQUAL 3
#define LEX_ERROR_INTERNAL 4
#define FILE_ERROR 5

#define S_STATE 100

#define NUM_STATE 101
#define FLOAT_DOT_STATE 1011
#define FLOAT_STATE 1012
#define FLOATE_STATE 1013
#define FLOATE_CYCL_STATE 1014
#define SIGN_FLOATE_STATE 1015

#define ID_STATE 102
#define UNDER_STATE 1021
#define MUL_STATE 103
#define ADD_STATE 104
#define SUB_STATE 105
#define DIV_STATE 106
#define LESS_STATE 107
#define GREAT_STATE 108
#define EQ_STATE 109
#define QUEST_STATE 110
#define EXCL_STATE 111

#define QUOTE_STATE 112
#define MRLSTR_STATE 1121

#define STRING_STATE 113
#define STRING_ESCAPE_STATE 114
#define STRING_HEX_ESCAPE_STATE 115
#define STRING_HEX_ESCAPE_LBRACKET_STATE 116
#define MRLSTR_END_STATE 117

const char *keywords[] = {
    "Double",
    "Int",
    "String",
    "KW_doubleQ",
    "KW_intQ",
    "KW_stringQ",
    "else",
    "func",
    "if",
    "let",
    "nil",
    "return",
    "var",
    "while",
};

const char* tokenTypeStrings[] = {
    "TOK_add",
    "TOK_sub",
    "TOK_arrow",
    "TOK_mul",
    "TOK_div",
    "TOK_less",
    "TOK_lessEq",
    "TOK_great",
    "TOK_greatEq",
    "TOK_equal",
    "TOK_compEq",
    "TOK_doubleQuest",
    "TOK_exclam",
    "TOK_notEq",
    "TOK_keyword",
    "TOK_identifier",
    "TOK_int",
    "TOK_float",
    "TOK_string",
    "TOK_colon",
    "TOK_openRoundBracket",
    "TOK_closeRoundBracket",
    "TOK_openCurlyBracket",
    "TOK_closeCurlyBracket",
    "TOK_comma",
    "TOK_underScore",
    "TOK_lexError",
    "TOK_EOF",
    "TOK_EOL"
};

int handleError(int errorCode, String *string) {
	static const char *SERR_STRINGS[6] = {
			"",
            "Lexical analysis error",
            "Lexical analysis end",
			"String allocation error",
            "KW tables are NOT same size",
            "Lexical internal error"
	};

	if (errorCode <= 0 || errorCode > 5)
	{
		errorCode = 0;
	}

    freeStr(string);

    if (errorCode != 0 && errorCode != 2) {
	    fprintf(stderr, "%s\n", SERR_STRINGS[errorCode]);
        #ifndef UNIT_TEST
        exit(errorCode);
        #endif
    }


    return errorCode;
}

int processInt(String *string, Token *token) {
    char *ptr;

    token->tokenType = TOK_int;
    token->tokenValue.num = strtol(string->string, &ptr, 10);
    // string obsahoval jine znaky nez cisla
    if (*ptr != '\0') return handleError(LEX_ERROR_INTERNAL, string);


    return handleError(LEX_OK, string);
}

int processFloat(String *string, Token *token) {
    char *ptr;

    token->tokenType = TOK_float;
    token->tokenValue.numFloat = strtod(string->string, &ptr);
    // string obsahoval jine znaky nez cisla
    if (*ptr != '\0') return handleError(LEX_ERROR_INTERNAL, string);


    return handleError(LEX_OK, string);
}

int processQueryKeyword(Token *token, FILE *swiftFile) {

    if (swiftFile == NULL) return LEX_ERROR_INTERNAL;

    char currSymbol = getc(swiftFile);

    if (currSymbol != '?') {
        ungetc(currSymbol, swiftFile);
        return LEX_OK;
    }

    if (token->tokenValue.keyword == KW_double) {
        token->tokenValue.keyword = KW_doubleQ;
    }
    else if (token->tokenValue.keyword == KW_int) {
        token->tokenValue.keyword = KW_intQ;
    }
    else if (token->tokenValue.keyword == KW_string) {
        token->tokenValue.keyword = KW_stringQ;
    }


    return LEX_OK;
}

int processId(String *string, Token *token, FILE *swiftFile) {
    if (KW_COUNT != (sizeof(keywords) / sizeof(keywords[0]))) {
        return handleError(KW_TABLE_SIZE_NOT_EQUAL, string);
    }

    for (unsigned i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(string->string, keywords[i]) == 0) {
            token->tokenValue.keyword = (Keyword)i;
            token->tokenType = TOK_keyword;

            processQueryKeyword(token, swiftFile);
                debug("Processed keyword %s\n", keywords[token->tokenValue.keyword]);

            return handleError(LEX_OK, string);
        }
    }

    token->tokenType = TOK_identifier;
    token->tokenValue.string = strdup(string->string);
        debug("Processed identifier %s\n", token->tokenValue.string);

    return handleError(LEX_OK, string);
}

int processString(String *string, Token *token, FILE *swiftFile, bool isMultiLine) {
    char *ptr;
    char currChar;
    int state = STRING_STATE;
    int quoteCounter = 0;

    String *hexEscape;

    if (isMultiLine) {
        currChar = getc(swiftFile);
        if (currChar != '\n') return handleError(LEX_ERROR, string);
    }

    while (1) {
        currChar = getc(swiftFile);
        debug("String, CurrChar: %c, State: %d, Curr string: %s, Allocated: %d, Len: %d\n", currChar, state, string->string, string->allocSize, string->len);
        if (currChar == EOF || (currChar == '\n' && !isMultiLine)) return handleError(LEX_ERROR, string);

        switch (state) {
            case STRING_STATE:
                if (currChar == '\\') {
                    state = STRING_ESCAPE_STATE;
                }
                else if (currChar == '"') {
                    
                    token->tokenValue.string = strdup(string->string);
                    token->tokenType = TOK_string;
                    return handleError(LEX_OK, string);
                }
                else if (currChar > 31) {
                    debug("Adding char\n");
                    addCharStr(currChar, string);
                    debug("Added char\n");
                }
                else if (isMultiLine && currChar == '\n') {
                    addCharStr('\n', string);
                    state = MRLSTR_END_STATE;
                }
                else if (isMultiLine && isspace(currChar)) {
                    addCharStr(currChar, string);
                }
                else {
                    return handleError(LEX_ERROR, string);
                }

                break;

            case MRLSTR_END_STATE:
                if (currChar == '"') {
                    quoteCounter++;
                    addCharStr(currChar, string);
                    if (quoteCounter == 3) {
                        delLastCharStr(string); // "
                        delLastCharStr(string); // "
                        delLastCharStr(string); // "
                        delLastCharStr(string); // /n
                        token->tokenValue.string = strdup(string->string);
                        token->tokenType = TOK_string;
                        return handleError(LEX_OK, string);
                    }
                }
                else {
                    ungetc(currChar, swiftFile);
                    state = STRING_STATE;
                }

                break;

            case STRING_ESCAPE_STATE:
                if (currChar == 'u') {
                    state = STRING_HEX_ESCAPE_LBRACKET_STATE;
                    hexEscape = (String *) malloc(sizeof(String));
                    initStr(hexEscape);
                    break;
                } 
                else if (currChar == '"') {
                    addCharStr('"', string);
                }
                else if (currChar == 'n') {
                    addCharStr('\n', string);
                }
                else if (currChar == 'r') {
                    addCharStr('\r', string);
                }
                else if (currChar == 't') {
                    addCharStr('\t', string);
                }
                else if (currChar == '\\') {
                    addCharStr('\\', string);
                }
                else {
                    return handleError(LEX_ERROR, string);
                }

                state = STRING_STATE;
                
                break;

            case STRING_HEX_ESCAPE_LBRACKET_STATE:
                if (currChar == '{') {
                    state = STRING_HEX_ESCAPE_STATE;
                }
                else {
                    freeStr(hexEscape);
                    return handleError(LEX_OK, string);
                }
                break;

            case STRING_HEX_ESCAPE_STATE:
                if (hexEscape->len > 8) {
                    freeStr(hexEscape);
                    return handleError(LEX_ERROR, string);
                }
                else if (isdigit(currChar) || (currChar >= 'A' && currChar <= 'F') || 
                    (currChar >= 'a' && currChar <= 'f')) {
                    addCharStr(currChar, hexEscape);
                }
                else if (currChar == '}') {
                    int hexChar = strtol(hexEscape->string, &ptr, 16);

                    if (*ptr != '\0' || hexChar > 255) return handleError(LEX_ERROR_INTERNAL, string);

                    freeStr(hexEscape);

                    addCharStr((char)hexChar, string);
                    state = STRING_STATE;
                }
                else {
                    freeStr(hexEscape);
                    return handleError(LEX_ERROR, string);
                }
                break;
        }
    }

    return handleError(LEX_ERROR, string);
}

int processMultiComm(FILE *swiftFile, int *commentDepth) {
    char currChar;
    int state = 0;

    while (1) {
        currChar = getc(swiftFile);

        if (*commentDepth == 0) return LEX_OK;
        else if (currChar == EOF) return LEX_ERROR;

        switch (state) {
            case 0:
                if (currChar == '/') {
                    state = DIV_STATE;
                }
                else if (currChar == '*') {
                    state = MUL_STATE;
                }

                break;

            case DIV_STATE:
                if (currChar == '*') {
                    *commentDepth += 1;
                    if (processMultiComm(swiftFile, commentDepth) == 1) {
                        return LEX_ERROR; 
                    }
                }
                else {
                    state = 0;
                }

                break;
            
            case MUL_STATE:
                if (currChar == '/') {
                    *commentDepth -= 1;
                    return LEX_OK;
                }
                else {
                    state = 0;
                }
                break;
        }



    }



    return LEX_ERROR;
}

int processLineComm(FILE *swiftFile) {

    char currSymbol = getc(swiftFile);
    while(currSymbol != '\n' && currSymbol != EOF) {
        currSymbol = getc(swiftFile);
    }

    if (currSymbol == '\n') {
        ungetc(currSymbol, swiftFile);
    }

    return 0;
}


int getToken(Token *token, FILE *swiftFile) {

    if (swiftFile == NULL) {
        fprintf(stderr, "SCANNER: Jsem na konci souboru\n");
        return FILE_ERROR;
    }
    
    String *string;
    string = (String *)malloc(sizeof(String));

    initStr(string);

    int state = S_STATE;

    while(1) {
        char currSymbol = getc(swiftFile);

        debug("Currently reading: %c\nState: %d\n", currSymbol, state);

        switch(state) {
            case S_STATE:
                if (isdigit(currSymbol)) {
                    state = NUM_STATE;
                    addCharStr(currSymbol, string);
                }
                else if (isalpha(currSymbol)) {
                    state = ID_STATE;
                    addCharStr(currSymbol, string);
                }
                else if (currSymbol == '_') {
                    state = UNDER_STATE;
                    addCharStr(currSymbol, string);
                }
                else if (currSymbol == '+') {
                    token->tokenType = TOK_add;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == '*') {
                    token->tokenType = TOK_mul;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == '-') {
                    state = SUB_STATE;
                }
                else if (currSymbol == '/') {
                    state = DIV_STATE;
                }
                else if (currSymbol == '<') {
                    state = LESS_STATE;
                }
                else if (currSymbol == '>') {
                    state = GREAT_STATE;
                }
                else if (currSymbol == '=') {
                    state = EQ_STATE;
                }
                else if (currSymbol == '?') {
                    state = QUEST_STATE;
                }
                else if (currSymbol == '!') {
                    state = EXCL_STATE;
                }
                else if (currSymbol == '"') {
                    state = QUOTE_STATE;
                }
                else if (currSymbol == ':') {
                    token->tokenType = TOK_colon;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == '(') {
                    token->tokenType = TOK_openRoundBracket;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == ')') {
                    token->tokenType = TOK_closeRoundBracket;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == '{') {
                    token->tokenType = TOK_openCurlyBracket;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == '}') {
                    token->tokenType = TOK_closeCurlyBracket;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == ',') {
                    token->tokenType = TOK_comma;
                    return handleError(LEX_OK, string);
                }
                else if (currSymbol == '\n') {
                    
                    while (currSymbol == '\n' || isspace(currSymbol)) {
                        currSymbol = getc(swiftFile);
                    } 
                    
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_EOL;
                    
                    return handleError(LEX_OK, string);
                }
                else if(isspace(currSymbol)) {
                    break;
                }
                else if (currSymbol == EOF) {
                    token->tokenType = TOK_EOF;
                    return handleError(LEX_END, string);
                }
                else {
                    return handleError(LEX_ERROR, string);
                }

                break;

            case NUM_STATE:
                if (isdigit(currSymbol)) {
                    // continue to accept numbers
                }
                else if (currSymbol == '.') {
                    state = FLOAT_DOT_STATE;
                }
                else if (currSymbol == 'e' || currSymbol == 'E') {
                    state = FLOATE_STATE;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    return processInt(string, token);
                }

                addCharStr(currSymbol, string);

                break;

            case FLOAT_DOT_STATE:
                if (isdigit(currSymbol)) {
                    state = FLOAT_STATE;
                }
                else {
                    return handleError(LEX_ERROR, string);
                }

                addCharStr(currSymbol, string);

                break;

            case FLOAT_STATE:
                if (isdigit(currSymbol)) {
                    // continue
                }
                else if (currSymbol == 'e' || currSymbol == 'E') {
                    state = FLOATE_STATE;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    return processFloat(string, token);
                }

                addCharStr(currSymbol, string);

                break;

            case FLOATE_STATE:
                if (isdigit(currSymbol)) {
                    state = FLOATE_CYCL_STATE;
                }
                else if (currSymbol == '+' || currSymbol == '-') {
                    state = SIGN_FLOATE_STATE;
                }
                // after "e" must be sign, or number
                else {
                    return handleError(LEX_ERROR, string);
                }

                addCharStr(currSymbol, string);
                
                break;

            case FLOATE_CYCL_STATE:
                if (isdigit(currSymbol)) {
                    addCharStr(currSymbol, string);
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    return processFloat(string, token);
                }

                break;

            case SIGN_FLOATE_STATE:
                if (isdigit(currSymbol)) {
                    addCharStr(currSymbol, string);
                    state = FLOATE_CYCL_STATE;
                }
                else {
                    return handleError(LEX_ERROR, string);
                }

                break;

            case UNDER_STATE:
                if (isalpha(currSymbol) || isdigit(currSymbol) || currSymbol == '_') {
                    addCharStr(currSymbol, string);
                    state = ID_STATE;
                }
                else {
                    token->tokenType = TOK_underScore;
                    ungetc(currSymbol, swiftFile);
                    return handleError(LEX_OK, string);   
                }

                break;

            case ID_STATE:
                if (isalpha(currSymbol) || isdigit(currSymbol) || currSymbol == '_') {
                    addCharStr(currSymbol, string);
                }
                else  {
                    ungetc(currSymbol, swiftFile);
                    return processId(string, token, swiftFile);
                }

                break;

            
            case SUB_STATE:
                if (currSymbol == '>') {
                    token->tokenType = TOK_arrow;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_sub;
                }

                return handleError(LEX_OK, string);    

            case DIV_STATE:
                if (currSymbol == '/') {
                    processLineComm(swiftFile);
                    state = S_STATE;
                    break;
                }
                else if (currSymbol == '*') {
                    int commentDepth = 1;

                    if (processMultiComm(swiftFile, &commentDepth) == 1) {
                        return handleError(LEX_ERROR, string);
                    };

                    state = S_STATE;
                    break;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_div;
                }

                return handleError(LEX_OK, string);

            case LESS_STATE:
                if (currSymbol == '=') {
                    token->tokenType = TOK_lessEq;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_less;
                }

                return handleError(LEX_OK, string);

            case GREAT_STATE:
                if (currSymbol == '=') {
                    token->tokenType = TOK_greatEq;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_great;
                }

                return handleError(LEX_OK, string);

            case EQ_STATE:
                if (currSymbol == '=') {
                    token->tokenType = TOK_compEq;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_equal;
                }

                return handleError(LEX_OK, string);

            case QUEST_STATE:
                if (currSymbol == '?') {
                    token->tokenType = TOK_doubleQuest;
                    return handleError(LEX_OK, string);
                }
                else {
                    return handleError(LEX_ERROR, string);
                }

            
            case EXCL_STATE:
                if (currSymbol == '=') {
                    token->tokenType = TOK_notEq;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_exclam;
                }

                return handleError(LEX_OK, string);
            
            case QUOTE_STATE:
                if (currSymbol == '"') {
                    state = MRLSTR_STATE;
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    return processString(string, token, swiftFile, 0);
                }

                break;

            case MRLSTR_STATE:
                if (currSymbol == '"') {
                    return processString(string, token, swiftFile, 1);
                }
                else {
                    ungetc(currSymbol, swiftFile);
                    token->tokenType = TOK_string;
                    token->tokenValue.string = strdup(string->string);
                    return handleError(LEX_OK, string);
                }

                break;
            
        }
    }
}

