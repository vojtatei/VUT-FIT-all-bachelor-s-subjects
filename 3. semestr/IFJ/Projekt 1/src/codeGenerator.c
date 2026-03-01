/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Code Generator
 *
 * @author Jakub Jeřábek (xjerab28)
 */


//#define DEBUG
#include "codeGenerator.h"


#define GEN_UNEXPECTED_CHAR 1
#define GEN_FLOAT_CONV_ERR 2

#define DOUBLE_HEX_STR_SIZE 50
#define LABEL_SIZE 50


char *frame[] = {
    "GF",
    "LF",
};

void comment(int count, ...) {
    va_list args;
    va_start(args, count);

    printf("# ");

    for (int i = 0; i < count; i++) {
        const char *str = va_arg(args, const char *);
        printf("%s ", str);
    }
    printf("\n");
    va_end(args);
}

void inst(int count, ...) {
    va_list args;
    va_start(args, count);

    int counter;

    for (counter = 0; counter < count; counter++) {
        const char *str = va_arg(args, const char *);
        printf("%s", str);
    }

    if (counter != count) {
        printf("\nERROR PRINTING: LESS ARGUMENTS THAN COUNT\n");
    }

    printf("\n");
    va_end(args);
}

char* convertIndex(int labelIndex) {
    char *convertedLabel = (char* ) malloc(sizeof(char) * LABEL_SIZE); 

    if (convertedLabel != NULL) {
        sprintf(convertedLabel, "%d", labelIndex);
    }
    else {
        exit(-1);
    }

    return convertedLabel;
}


void generateHeader() {
    inst(1, ".IFJcode23");
    comment(1, "MAIN PROGRAM\n");
}

void builtCheckNil() {
    comment(1, " CHECKNIL(bool canBeNill, int/double/string value)");
    inst(1, "LABEL $$checkNil");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@canBeNill");
    inst(1, "POPS TF@canBeNill");
    comment(1, " pokud nemuze byt nil, jdu zkontrolovat hodnotu promenne");
    inst(1, "JUMPIFEQ $$checkInputValue TF@canBeNill bool@false");
    inst(1, "RETURN");
    comment(1, "Zkontroluj hodnotu vstupu");
    inst(1, "LABEL $$checkInputValue");
    inst(1, "DEFVAR TF@inputValue");
    inst(1, "POPS TF@inputValue");
    inst(1, "JUMPIFEQ $$cantBeNillError TF@inputValue nil@nil");
    inst(1, "RETURN");
    comment(1, "CHYBA, ukonci program");
    inst(1, "LABEL $$cantBeNillError");
    inst(1, "WRITE string@DAL\\032SI\\032NILL\\032ZLOBIDLO!!!");
    inst(1, "EXIT int@7");
}

void builtReadInt() {
    inst(1, "LABEL readInt");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@integer");
    inst(1, "READ TF@integer int");
    inst(1, "PUSHS TF@integer");
    inst(1, "RETURN");
}

void builtReadDouble() {
    inst(1, "LABEL readDouble");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@double");
    inst(1, "READ TF@double float");
    inst(1, "PUSHS TF@double");
    inst(1, "RETURN");
}

void builtReadString() {
    inst(1, "LABEL readString");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@string");
    inst(1, "READ TF@string string");
    inst(1, "PUSHS TF@string");
    inst(1, "RETURN");
}

void builtWrite() {
    comment(1, "# BUILTWRITE (int count, ...)");
    inst(1, "LABEL write");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@actualValue");
    inst(1, "DEFVAR TF@counter");
    inst(1, "POPS TF@counter");
    inst(1, "JUMPIFEQ $$writeEnd TF@counter int@0");
    inst(1, "LABEL $$writeCycle");
    inst(1, "POPS TF@actualValue");
    inst(1, "WRITE TF@actualValue");
    inst(1, "SUB TF@counter TF@counter int@1");
    inst(1, "JUMPIFNEQ $$writeCycle TF@counter int@0");
    inst(1, "RETURN");
}

void builtInt2Double() {
    comment(1, " BUILT-Int2Double(int term) -> Double");
    inst(1, "LABEL Int2Double");
    inst(1, "INT2FLOATS");
    inst(1, "RETURN");
}

void builtDouble2Int() {
    comment(1, " BUILT-Double2Int(int term) -> Int");
    inst(1, "LABEL Double2Int");
    inst(1, "FLOAT2INTS");
    inst(1, "RETURN");
}

void builtLength() {
    comment(1, " BUILT - LENGTH(string str) -> Int");
    inst(1, "LABEL length");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@lengthNum");
    inst(1, "DEFVAR TF@inputString");
    inst(1, "POPS TF@inputString");
    inst(1, "STRLEN TF@lengthNum TF@inputString");
    inst(1, "PUSHS TF@lengthNum");
    inst(1, "RETURN");
}

void builtSubString() {
    inst(1, "LABEL substring");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@inputString");
    inst(1, "DEFVAR TF@startIndex");
    inst(1, "DEFVAR TF@endIndex");
    inst(1, "POPS TF@inputString");
    inst(1, "POPS TF@startIndex");
    inst(1, "POPS TF@endIndex");
    inst(1, "DEFVAR TF@resultString");
    inst(1, "MOVE TF@resultString string@");
    inst(1, "DEFVAR TF@actualChar");
    inst(1, "DEFVAR TF@isStartGtEnd");
    inst(1, "JUMP $$subStringCheckCond");
    inst(1, "LABEL $$subStringLoop");
    inst(1, "JUMPIFEQ $$subStringEnd TF@startIndex TF@endIndex");
    inst(1, "GETCHAR TF@actualChar TF@inputString TF@startIndex # char c = input[start]");
    inst(1, "CONCAT TF@resultString TF@resultString TF@actualChar");
    inst(1, "ADD TF@startIndex TF@startIndex int@1");
    inst(1, "JUMP $$subStringLoop");
    inst(1, "LABEL $$subStringCheckCond");
    inst(1, "DEFVAR TF@strLen");
    inst(1, "DEFVAR TF@compResult");
    inst(1, "STRLEN TF@strLen TF@inputString");
    inst(1, "LT TF@compResult TF@startIndex int@0");
    inst(1, "JUMPIFEQ $$subStringNil TF@compResult bool@true");
    inst(1, "LT TF@compResult TF@endIndex int@0");
    inst(1, "JUMPIFEQ $$subStringNil TF@compResult bool@true");
    inst(1, "GT TF@compResult TF@startIndex TF@endIndex");
    inst(1, "JUMPIFEQ $$subStringNil TF@compResult bool@true");
    inst(1, "JUMPIFEQ $$subStringNil TF@startIndex TF@strLen");
    inst(1, "GT TF@compResult TF@startIndex TF@strLen");
    inst(1, "JUMPIFEQ $$subStringNil TF@compResult bool@true");
    inst(1, "GT TF@compResult TF@endIndex TF@strLen");
    inst(1, "JUMPIFEQ $$subStringNil TF@compResult bool@true");
    inst(1, "JUMP $$subStringLoop");
    inst(1, "LABEL $$subStringEnd");
    inst(1, "PUSHS TF@resultString");
    inst(1, "RETURN");
    inst(1, "LABEL $$subStringNil");
    inst(1, "MOVE TF@resultString nil@nil");
    inst(1, "PUSHS TF@resultString");
    inst(1, "RETURN");

}

void builtOrd() {
    comment(1, " BUILT ORD (string c) -> Int");
    inst(1, "LABEL ord");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@strLen");
    inst(1, "DEFVAR TF@inputString");
    inst(1, "DEFVAR TF@asciiValue");
    inst(1, "POPS TF@inputString");
    inst(1, "STRLEN TF@strLen TF@inputString");
    inst(1, "JUMPIFEQ $$ord0 TF@strLen int@0");
    inst(1, "STRI2INT TF@asciiValue TF@inputString int@0");
    inst(1, "PUSHS TF@asciiValue");
    inst(1, "RETURN");
    inst(1, "LABEL $$ord0");
    inst(1, "PUSHS int@0");
    inst(1, "RETURN");
}


void builtChr() {
    comment(1, " BUILT CHR (int number) -> String");
    inst(1, "LABEL chr");
    inst(1, "CREATEFRAME");
    inst(1, "DEFVAR TF@inputNum");
    inst(1, "DEFVAR TF@outputChar");
    inst(1, "POPS TF@inputNum");
    inst(1, "INT2CHAR TF@outputChar TF@inputNum");
    inst(1, "PUSHS TF@outputChar");
    inst(1, "RETURN");
}


void doubleQuest() {
    inst(1, "LABEL $doubleQuest");
    inst(1, "POPS GF@$operand2");
    inst(1, "POPS GF@$operand1");
    inst(1, "JUMPIFEQ $isNil GF@$operand1 nil@nil");
    comment(1, "Levy operand neni nil, davam na zasobnik a konec");
    inst(1, "PUSHS GF@$operand1");
    inst(1, "RETURN");
    inst(1, "LABEL $isNil");
    inst(1, "JUMPIFEQ $isNilError GF@$operand2 nil@nil");
    inst(1, "PUSHS GF@$operand2");
    inst(1, "RETURN");
    comment(1, "Chyba: pravy operand nemuze byt nil");
    inst(1, "LABEL $isNilError");
    inst(1, "EXIT int@7");
}

void canBeNil(const char* idName, dataTypeT dataType, const char* frame, const char* uniqId) {

    if (dataType == VOID) {
        return;
    }
    // nemuze byt nil
    if (dataType == INT || dataType == DOUBLE || dataType == STRING) {
        inst(7 ,"MOVE ", frame, "@", "$$", idName, uniqId, " bool@false");
    }
    // Muze byt nil
    else {
        inst(7 ,"MOVE ", frame, "@", "$$", idName, uniqId, " bool@true");
    }

}

void endProgram() {
    inst(1, "EXIT int@0");
}

void generateVariable(Tdata *data) {
    String *numValue = (String *) malloc(sizeof(String));
    if (numValue == NULL) exit(-1);
    initStr(numValue);

    snprintf(numValue->string, numValue->allocSize, "%d", data->uniqId);

    inst(5, "DEFVAR ", frame[data->localFrame], "@", data->idName, numValue->string);
    inst(6, "DEFVAR ", frame[data->localFrame], "@", "$$", data->idName, numValue->string);
    
    canBeNil(data->idName, data->dataType, frame[data->localFrame], numValue->string);

    if (data->dataType == INTQ || data->dataType == DOUBLEQ || data->dataType == STRINGQ) {
        inst(6, "MOVE ", frame[data->localFrame], "@", data->idName, numValue->string, " nil@nil");
    }
}

void assignValue(Tdata *data, expressionStruct *expression) {
    generateExpression(expression);
    String *numValue = (String *) malloc(sizeof(String));
    initStr(numValue);
    snprintf(numValue->string, numValue->allocSize, "%d", data->uniqId);

    if ((data->dataType == DOUBLE || data->dataType == DOUBLEQ) && expression->type == TYPE_INT) {
        inst(1, "INT2FLOATS");
    }

    inst(5, "POPS ", frame[data->localFrame], "@", data->idName, numValue->string);
    freeStr(numValue);
}

void generateExpression(expressionStruct *expression) {
    int countVariables = 0;
    char *convertedStr;
    String *numValue = (String *) malloc(sizeof(String));
    if (numValue == NULL) exit(-1);
    initStr(numValue);
    bool convertDouble = 0;

    if(reallocStr(DOUBLE_HEX_STR_SIZE, numValue) != 0) {
        fprintf(stderr, "Internal generator error\n");
        exit(-1);
    }

    if (expression->allIdentifiersKnown == 0 && (expression->variables[0]->dataType == DOUBLE || expression->variables[0]->dataType == DOUBLEQ)) {
        convertDouble = 1;
    }
    else if (expression->type == TYPE_DOUBLE){
        convertDouble = 1;
    }

    for (int token = 0; token < expression->postFixLen; token++) {
        Token currToken = expression->postFix[token];
        debug("Currently reading: %s\n", tokenTypeStrings[currToken.tokenType]);
        switch (currToken.tokenType) {
            case TOK_add:
                if (expression->type == TYPE_STRING) {
                    inst(1, "POPS GF@$operand2");
                    inst(1, "POPS GF@$operand1");
                    inst(1, "CONCAT GF@$operand1 GF@$operand1 GF@$operand2");
                    inst(1, "PUSHS GF@$operand1");
                }
                else if (expression->type == TYPE_INT || expression->type == TYPE_DOUBLE) {
                    inst(1, "ADDS");
                }
                break;
            case TOK_sub:
                inst(1, "SUBS");
                break;
            case TOK_mul:
                inst(1, "MULS");
                break;
            case TOK_div:
                if (expression->type == TYPE_INT) {
                    inst(1, "IDIVS");
                }
                else {
                    inst(1, "DIVS");
                }
                break;
            case TOK_less:
                inst(1, "LTS");
                break;
            case TOK_great:
                inst(1, "GTS");
                break;
            case TOK_lessEq:
                inst(1, "POPS GF@$operand2");
                inst(1, "POPS GF@$operand1");
                inst(1, "LT GF@$statValue GF@$operand1 GF@$operand2");
                inst(1, "PUSHS GF@$statValue");
                inst(1, "EQ GF@$statValue GF@$operand1 GF@$operand2");
                inst(1, "PUSHS GF@$statValue");
                inst(1, "ORS");
                break;
            case TOK_greatEq:
                inst(1, "POPS GF@$operand2");
                inst(1, "POPS GF@$operand1");
                inst(1, "GT GF@$statValue GF@$operand1 GF@$operand2");
                inst(1, "PUSHS GF@$statValue");
                inst(1, "EQ GF@$statValue GF@$operand1 GF@$operand2");
                inst(1, "PUSHS GF@$statValue");
                inst(1, "ORS");
                break;
            case TOK_compEq:
                inst(1, "EQS");
                break;
            case TOK_notEq:
                inst(1, "POPS GF@$operand2");
                inst(1, "POPS GF@$operand1");
                inst(1, "EQ GF@$statValue GF@$operand1 GF@$operand2");
                inst(1, "NOT GF@$statValue GF@$statValue");
                inst(1, "PUSHS GF@$statValue");
                break;
            // todo nevim
            case TOK_exclam:
                inst(1, "POPS GF@$operand1");
                inst(1, "TYPE GF@$operand2 GF@$operand1");
                inst(1, "JUMPIFEQ $isNilError GF@$operand2 nil@nil");
                inst(1, "PUSHS GF@$operand1");
                break;
            case TOK_doubleQuest:
                inst(1, "CALL $doubleQuest");
                break;
            case TOK_identifier:
                snprintf(numValue->string, numValue->allocSize, "%d", expression->variables[countVariables]->uniqId);
                inst(5, "PUSHS ", frame[expression->variables[countVariables]->localFrame], "@", currToken.tokenValue.string, numValue->string);
                countVariables++;
                break;
            case TOK_int:
                snprintf(numValue->string, numValue->allocSize, "%d", currToken.tokenValue.num);
                inst(2, "PUSHS int@", numValue->string);
                if (convertDouble) {
                    inst(1, "INT2FLOATS");
                }
                clearStr(numValue);
                break;
            case TOK_float:
                snprintf(numValue->string, numValue->allocSize, "%a", currToken.tokenValue.numFloat);
                inst(2, "PUSHS float@", numValue->string);
                clearStr(numValue);
                break;
            case TOK_string:
                convertedStr = escapeString(currToken.tokenValue.string);
                inst(2, "PUSHS string@", convertedStr);
                free(convertedStr);
                break;
            case TOK_keyword:
                inst(1, "PUSHS nil@nil");
                break;
            default:
                fprintf(stderr, "GEN ERROR: UNEXPECTED TOKEN IN POSTFIX EXPRESSION\n");
                exit(-1);
                break;
        }
    }
    freeStr(numValue);
}

void generateWhileHead(expressionStruct *expression, int labelIndex) {
    char *index = convertIndex(labelIndex);

    if (expression->type != TYPE_BOOL) {
        fprintf(stderr, "GEN ERROR: EXPECTED TYPE BOOL\n");
        exit(-1);
    }

    inst(2, "LABEL $$whileHead", index);
    generateExpression(expression);
    inst(1, "POPS GF@$statValue");
    inst(3, "JUMPIFEQ $$whileEnd", index, " GF@$statValue bool@false");

    inst(2, "LABEL $$while", index);
    
    free(index);
}

void generateWhileEnd(int labelIndex) {
    char *index = convertIndex(labelIndex);

    inst(2,"JUMP $$whileHead", index);
    inst(2, "LABEL $$whileEnd", index);
    free(index);
}

void generateIfLet(Tdata *letVariable, int labelIndex) {
    char *index = convertIndex(labelIndex);
    String *numValue = (String *) malloc(sizeof(String));
    if (numValue == NULL) exit(-1);
    initStr(numValue);
    snprintf(numValue->string, numValue->allocSize, "%d", letVariable->uniqId);

    inst(8, "JUMPIFEQ $$else", index, " ", frame[letVariable->localFrame], "@", letVariable->idName, numValue->string, " nil@nil");
    inst(2, "LABEL $$if", index);
    canBeNil(letVariable->idName, letVariable->dataType, frame[letVariable->localFrame], numValue->string);
    
    free(index);
    freeStr(numValue);
}

void generateIf(expressionStruct *expression, int labelIndex) {

    if (expression->type != TYPE_BOOL) {
        fprintf(stderr, "GEN ERROR: EXPECTED TYPE BOOL\n");
        exit(-1);
    }

    char *index = convertIndex(labelIndex);
    generateExpression(expression);
    inst(1, "POPS GF@$statValue");
    inst(3, "JUMPIFEQ $$else", index, " GF@$statValue bool@false");

    inst(2, "LABEL $$if", index);
    free(index);
}

void generateElse(int labelIndex, Tdata *letVariable) {

    char *index = convertIndex(labelIndex);

    if (letVariable != NULL) {
        String *numValue = (String *) malloc(sizeof(String));
        if (numValue == NULL) exit(-1);
        initStr(numValue);
        snprintf(numValue->string, numValue->allocSize, "%d", letVariable->uniqId);
        canBeNil(letVariable->idName, letVariable->dataType, frame[letVariable->localFrame], numValue->string);
        freeStr(numValue);
    }


    inst(2, "JUMP $$endElse", index);
    inst(2, "LABEL $$else", index);
    free(index);

}

void generateEndElse(int labelIndex) {
    char *index = convertIndex(labelIndex);

    inst(2, "LABEL $$endElse", index);
    free(index);
}

void initGenerator() {
    generateHeader();
    inst(1, "LABEL $MAIN");
    comment(1, " GLOBAL VARIABLES");
    inst(1, "DEFVAR GF@$statValue");
    inst(1, "DEFVAR GF@$operand1");
    inst(1, "DEFVAR GF@$operand2");
    inst(1, "DEFVAR GF@$tmpValue");
    comment(1, "# MAIN PROGRAM\n\n");
    inst(1, "CREATEFRAME");
    inst(1, "PUSHFRAME");
}

void generateFunctionHead(Tdata* function) {

    inst(2, "JUMP $end", function->idName);
    inst(2, "LABEL ", function->idName);
    inst(1, "CREATEFRAME");
    inst(1, "PUSHFRAME");

    if (function->dataType != VOID) {
        inst(1, "DEFVAR LF@$funcReturn0");
        inst(1, "DEFVAR LF@$$funcReturn0");
        canBeNil("funcReturn", function->dataType, "LF", "0");
    }

    for (int i = 0; i < function->numOfParam; i++) {
        inst(3, "DEFVAR LF@", function->arrayOfParams[i].id_par, "0");
        inst(3, "POPS LF@", function->arrayOfParams[i].id_par, "0");
        inst(3, "DEFVAR LF@$$", function->arrayOfParams[i].id_par, "0");
        canBeNil(function->arrayOfParams[i].id_par, function->arrayOfParams[i].type, "LF", "0");
    }

    comment(1, "Parameters pushed");
}

void generateFunctionReturn(expressionStruct *expression) {
    // vim ze void funkce
    if (expression == NULL) {
        inst(1, "RETURN");
        return;
    }

    generateExpression(expression);
    inst(1, "POPS LF@$funcReturn0");

    inst(1, "PUSHS LF@$funcReturn0");
    inst(1, "PUSHS LF@$$funcReturn0");
    inst(1, "CALL $$checkNil");

    inst(1, "PUSHS LF@$funcReturn0");
    inst(1, "POPFRAME");
    inst(1, "RETURN");
}

void generateFunctionEnd(Tdata * function) {

    inst(1, "POPFRAME");
    inst(1, "RETURN");
    inst(2, "LABEL $end", function->idName);

}

void callFunction(Tdata* function, Tdata* params, int parameterCount) {
    String *numValue = (String *) malloc(sizeof(String));
    if (numValue == NULL) exit(-1);
    initStr(numValue);

    for (int i = parameterCount - 1; i >= 0; i--) {
        if (params[i].idName != NULL) {
            snprintf(numValue->string, numValue->allocSize, "%d", params[i].uniqId);
            inst(5, "PUSHS ", frame[params[i].localFrame], "@", params[i].idName, numValue->string);
            clearStr(numValue);
        }
        else if (params[i].dataType == INT || params[i].dataType == INTQ) {
            inst(3, "PUSHS ", "int@", params[i].value);
        }
        else if (params[i].dataType == DOUBLE || params[i].dataType == DOUBLEQ) {
            inst(3, "PUSHS ", "float@", params[i].value);
        }
        else if (params[i].dataType == STRING || params[i].dataType == STRINGQ) {
            char *convertedStr = escapeString(params[i].value);
            inst(3, "PUSHS ", "string@", convertedStr);
            free(convertedStr);
        }
    }

    // write muze mit variabilni pocet parametru - musim vedet kolik
    if (strcmp(function->idName, "write") == 0) {
        char* paramCount = convertIndex(parameterCount);
        inst(2 ,"PUSHS int@", paramCount);
        free(paramCount);
    }

    inst(2, "CALL ", function->idName);
}

void assignFunctionValue(Tdata *variable) {
    String *numValue = (String *) malloc(sizeof(String));
    if (numValue == NULL) exit(-1);
    initStr(numValue);
    snprintf(numValue->string, numValue->allocSize, "%d", variable->uniqId);

    inst(5, "POPS ", frame[variable->localFrame], "@", variable->idName, numValue->string);

    if (variable->dataType == INT || variable->dataType == DOUBLE || variable->dataType == STRING) {
        inst(5, "PUSHS ", frame[variable->localFrame], "@", variable->idName, numValue->string);
        inst(5, "PUSHS ", frame[variable->localFrame], "@$$", variable->idName, numValue->string);
        inst(1, "CALL $$checkNil\n");
    }

    freeStr(numValue);
}

char* escapeString(char* input) {
    int extra_space = 0;
    for (const char* p = input; *p; p++) {
        unsigned char ch = *p;
        if ((ch > 0 && ch <= 32) || ch == 35 || ch == 92) {
            extra_space += 3; // Pro "\xyz"
        }
    }

    char* escaped = (char* ) malloc(strlen(input) + extra_space + 1);

    if (!escaped) {
        fprintf(stderr, "Generator internal alloc error");
        exit(-1);
    }

    const char* src = input;
    char* dest = escaped;
    while (*src) {
        unsigned char ch = *src;
        if ((ch > 0 && ch <= 32) || ch == 35 || ch == 92) {
            sprintf(dest, "\\%03d", ch);
            dest += 4;
        } else {
            *dest++ = ch;
        }
        src++;
    }
    *dest = '\0'; 

    return escaped;
}
