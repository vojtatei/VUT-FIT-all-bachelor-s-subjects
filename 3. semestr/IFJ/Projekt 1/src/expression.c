/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Funkce na zpracování výrazu
 *
 * @author Vojtech Teichmann ()
 */

#include "expression.h"
//#define DEBUG



typedef enum
{
	S,    // < SHIFT
	E,    // = EQUAL
	R,    // > REDUCE
	N     // # ERROR
} precTableSignEnum;


#define TABLE_SIZE 20

// Precedenční tabulka, která rozhoduje, jak bude zacházeno se zásobníkem při postfixu
int prec_table[TABLE_SIZE][TABLE_SIZE] =
{
//  | + | - | * | / | < | > | <= |>=|==| != | ! |?? | ( | ) | id|cel|des|řet|nil| $ |
    { R , R , S , S , R , R , R , R , R , R , S , R , S , R , S , S , S , S , N , R }, // +
    { R , R , S , S , R , R , R , R , R , R , S , R , S , R , S , S , S , N , N , R }, // -
    { R , R , R , R , R , R , R , R , R , R , S , R , S , R , S , S , S , N , N , R }, // *
    { R , R , R , R , R , R , R , R , R , R , S , R , S , R , S , S , S , N , N , R }, //  /
    { S , S , S , S , N , N , N , N , N , N , S , R , S , R , S , S , S , S , N , R }, // <
    { S , S , S , S , N , N , N , N , N , N , S , R , S , R , S , S , S , S , N , R }, // >
    { S , S , S , S , N , N , N , N , N , N , S , R , S , R , S , S , S , S , N , R }, // <=
    { S , S , S , S , N , N , N , N , N , N , S , R , S , R , S , S , S , S , N , R }, // >=
    { S , S , S , S , N , N , N , N , N , N , S , R , S , R , S , S , S , S , S , R }, // ==
    { S , S , S , S , N , N , N , N , N , N , S , R , S , R , S , S , S , S , S , R }, // !=
    { R , R , R , R , R , R , R , R , R , R , N , R , N , R , N , N , N , N , N , R }, // !
    { S , S , S , S , S , S , S , S , S , S , S , S , S , R , S , S , S , S , N , R }, // ??
    { S , S , S , S , S , S , S , S , S , S , S , S , S , E , S , S , S , S , S , N }, // (
    { R , R , R , R , R , R , R , R , R , R , R , R , N , R , N , N , N , N , N , R }, // )
    { R , R , R , R , R , R , R , R , R , R , R , R , N , R , N , N , N , N , N , R }, // id
    { R , R , R , R , R , R , R , R , R , R , N , N , N , R , N , N , N , N , N , R }, // cel
    { R , R , R , R , R , R , R , R , R , R , N , N , N , R , N , N , N , N , N , R }, // des
    { R , N , N , N , R , R , R , R , R , R , N , N , N , R , N , N , N , N , N , R }, // řet
    { N , N , N , N , N , N , N , N , R , R , N , N , N , R , N , N , N , N , N , R }, // nil
    { S , S , S , S , S , S , S , S , S , S , S , S , S , N , S , S , S , S , S , N }  // $
};


//Funkce která převede token na symobl. Obojí je enum, ale potřebuji to převést na svůj kvůli precedenční tabulce
void getSymbolFromToken(Token* token, helpStruct* mapping)
{

    
    switch(token->tokenType)
    {
        case TOK_add:
            mapping->index = SYMBOL_PLUS;
            break;
        case TOK_sub:
            mapping->index = SYMBOL_MINUS;
            break;
        case TOK_mul:
            mapping->index = SYMBOL_MULT;
            break;
        case TOK_div:
            mapping->index = SYMBOL_DIV;
            break;
        case TOK_less:
            mapping->index = SYMBOL_LTN;
            break;
        case TOK_great:
            mapping->index = SYMBOL_GTN;
            break;
        case TOK_lessEq:
            mapping->index = SYMBOL_LEQ;
            break;
        case TOK_greatEq:
            mapping->index = SYMBOL_GEQ;
            break;
        case TOK_compEq:
            mapping->index = SYMBOL_EQ;
            break;
        case TOK_notEq:
            mapping->index = SYMBOL_NEQ;
            break;
        case TOK_exclam:
            mapping->index = SYMBOL_NOT;
            break;
        case TOK_doubleQuest:
            mapping->index = SYMBOL_DQUEST;
            break;
        case TOK_openRoundBracket:
            mapping->index = SYMBOL_LEFT_BRACKET;
            break;
        case TOK_closeRoundBracket:
            mapping->index = SYMBOL_RIGHT_BRACKET;
            break;
        case TOK_identifier:
            mapping->index = SYMBOL_ID;
            break;
        case TOK_int:
            mapping->index = SYMBOL_CEL_CIS;
            break;
        case TOK_float:
            mapping->index = SYMBOL_DES_CIS;
            break;
        case TOK_string:
            mapping->index = SYMBOL_RET;
            break;
        case TOK_keyword:
            if(token->tokenValue.keyword == KW_nil)
            {
                mapping->index = SYMBOL_NIL;
                break;
            }
            else
            {
                exit(2);
            }
        default:
            mapping->index = SYMBOL_DOLLAR;
            break;
    }
}

//Funkce která se zavolá pokud je porušená priorita na zásobniku a popuje se zásobnik do té doby
//než se dá symbol vložit na zásobník (stejně jak v IALU)
void doReduce(Stack *stack, Token currentToken, Token *postfixExpression, unsigned *postfixExpressionLength) {
    
    helpStruct currentMapping;
    // Převedeme TokenType na PrecTableSymbolEnum
    getSymbolFromToken(&currentToken, &currentMapping);


    if (stackIsEmpty(stack)) {

        stackPush(stack, currentToken);
        return;
    }

    Token topOfStack = stackTop(stack);
    helpStruct topOperationMapping;
    getSymbolFromToken(&topOfStack, &topOperationMapping);

    if (prec_table[topOperationMapping.index][currentMapping.index] == S) {
        // Přerušíme rekurzi, protože jsme narazili na token s menší prioritou
        stackPush(stack, currentToken);
        return;
    } else {
        // Popneme token ze zásobníku a přidáme ho do výstupního pole
        postfixExpression[(*postfixExpressionLength)++] = stackPop(stack);
        
        // Pokračujeme rekurzivně

        doReduce(stack, currentToken, postfixExpression, postfixExpressionLength);
    }
}



// Funkce, která provádí operaci pomocí precedenční tabulky 
// a na základě toho pracuje se zásobníkem a vytváří postfixový výraz
void doOperation(Stack *stack, Token currentToken, Token *postfixExpression, unsigned *postfixExpressionLength) {
    helpStruct currentMapping;
    // Převedeme TokenType na PrecTableSymbolEnum
    getSymbolFromToken(&currentToken, &currentMapping);
   

    debug("Current Mapping: %d\n", currentMapping.index);

    // Pokud není index správný, ukončíme program (nebo vhodně ošetříme chybu)
    if (currentMapping.index == -1) {
        exit(4); 
    }

    helpStruct topMapping;
    // Nastavíme výchozí hodnotu indexu na -1, pokud je zásobník prázdný
    topMapping.index = -1;
    
    if (!stackIsEmpty(stack)) {
        Token topToken = stackTop(stack);
        getSymbolFromToken(&topToken, &topMapping);
        
    } else {
        // Předpokládáme, že na vrcholu zásobníku je $, pokud je prázdný
        topMapping.index = TABLE_SIZE - 1;
    }

    // Proměnná operation by měla být typu PrecTableSignEnum
    precTableSignEnum operation = (precTableSignEnum)prec_table[topMapping.index][currentMapping.index];

    // Následuje rozhodování podle hodnoty v precedenční tabulce
    //printf("topMapping.index: %d, currentMapping.index: %d, operation: %d\n", topMapping.index, currentMapping.index, operation);

    switch (operation) {
        case S: // Shift - přidáme token na zásobník
            stackPush(stack, currentToken);
            break;

        case N: // Error - nastala chyba, ukončíme program (nebo ošetříme chybu)
            fprintf(stderr, "Chyba: Chybná aritmetická operace.\n");
            exit(2); // Nahraďte vhodným způsobem ošetření chyb
        case E: // Equal - vyjmeme token ze zásobníku a přidáme do výstupního pole
            
                postfixExpression[(*postfixExpressionLength)++] = stackPop(stack);
                debug("postfixExpression expression: %d\n", postfixExpression[(*postfixExpressionLength) - 1].tokenType);
        
            break;
        case R: // Reduce - redukujeme na zásobníku, dokud nenarazíme na menší prioritu

                //Logika pro práci se závorkami
                if (currentToken.tokenType == TOK_openRoundBracket) {
                stackPush(stack, currentToken);
            } else if (currentToken.tokenType == TOK_closeRoundBracket) {
                while (!stackIsEmpty(stack) && stackTop(stack).tokenType != TOK_openRoundBracket) {
                    postfixExpression[(*postfixExpressionLength)++] = stackPop(stack);
                }
                if (!stackIsEmpty(stack) && stackTop(stack).tokenType == TOK_openRoundBracket) {
                    stackPop(stack); // Odstranění otevírací závorky ze zásobníku
                } else {
                    fprintf(stderr, "Chyba: Nevyvážené závorky ve výrazu.\n");
                    exit(2);
                }
            } else {
                doReduce(stack, currentToken, postfixExpression, postfixExpressionLength);
            }
            break;
        
        default:
            // Výchozí případ by neměl nastat, pokud ano, je to chyba
            exit(72);
    }
}



//Hlavní funkce pro convertovní infix do postfix
//Funkce postupně volá getToken dokud nenarazí na symbol, který nechce, na EOF nebo EOL
//Jakmile přijde symbol, který chceme, tak se zavolá doOperation, která tok zpracuje
//Výsledek je pole tokenu
Token* convertInfixToPostfix(FILE* swiftFile, unsigned* postfixExpressionLength, Tsymstack *symstack, expressionStruct *exprData, Token *firstToken, helpStruct *help) {
    Stack stack;
    stackInit(&stack);


    size_t estimatedMaxSize = 100; 

    Token* postfixExpression = (Token*)malloc(sizeof(Token) * estimatedMaxSize);

    if (postfixExpression == NULL) {
        // Vhodně ošetřit chybu alokace paměti
        exit(9);
    }

    *postfixExpressionLength = 0;
    exprData->allIdentifiersKnown = true;
    help->canIntDoubleCoexist = false;
    help->comparisonOperationFound = false;
    bool isFloat = false;

    int counter = 0;
    Token token = *firstToken;
    int openBrackets = 0;

    Token lastToken;
    lastToken.tokenType = TOK_EOF;
    exprData->type = TYPE_INT;



    while (token.tokenType != TOK_EOF || token.tokenType != TOK_EOL) 
    {
        if ((lastToken.tokenType >= TOK_add && lastToken.tokenType <= TOK_div) &&
            (token.tokenType >= TOK_add && token.tokenType <= TOK_div))
        {
            fprintf(stderr, "Chyba: Dva operátory za sebou.\n");
            exit(2);
        }

        if ((lastToken.tokenType >= TOK_add && lastToken.tokenType <= TOK_div) &&
            (token.tokenType == TOK_EOF || token.tokenType == TOK_closeRoundBracket || token.tokenType == TOK_EOL))
            {
                fprintf(stderr, "Chyba: Chybějící operand po operatoru.\n");
                exit(2);
            }


        if (token.tokenType == TOK_identifier)
        {

            Tdata *foundData = symstackSearchiId(symstack, token.tokenValue.string);
            
            if (foundData != NULL)
            {
                    exprData->variables[counter] = foundData;
                    counter++;

                    exprData->allIdentifiersKnown = false;

                    if (foundData->dataType == DOUBLE || foundData->dataType == DOUBLEQ)
                    {
                        help->canIntDoubleCoexist = true;
                    }
     

        if (!foundData->isInit) {
            fprintf(stderr, "Chyba: Neinicializovaný identifikátor '%s'.\n", token.tokenValue.string);
            exit(5);
        }

        // Vytvoření nového tokenu s typem na základě symstacku
        Token newToken;
        newToken.tokenType = TOK_identifier; // Typ je stále identifikátor
        newToken.tokenValue.string = strdup(token.tokenValue.string); // Kopie jména identifikátoru

        // Přiřazení dodatečného typu tokenu a aktualizace exprData->type
        switch (foundData->dataType) {
            case INT:
            case INTQ:
                exprData->type = TYPE_INT;
                break;
            case DOUBLE:
            case DOUBLEQ:
                isFloat = true;
                exprData->type = TYPE_DOUBLE;
                break;
            case STRING:
            case STRINGQ:
                exprData->type = TYPE_STRING;
                break;
            default:
                fprintf(stderr, "Chyba: Neznámý typ dat identifikátoru\n");
                exit(5);
        }

        // Přidání tokenu do postfixového pole
        postfixExpression[(*postfixExpressionLength)++] = newToken;
    } else {
        fprintf(stderr, "Chyba: Neznámý identifikátor\n");
        exit(5);
    }
}

        
        else if (token.tokenType == TOK_keyword && token.tokenValue.keyword == KW_nil)
        {
            exprData->type = TYPE_VOID;
            doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }

        else if (token.tokenType == TOK_string)
        {
            exprData->type = TYPE_STRING;

            doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }

        else if (token.tokenType == TOK_int)
        {
             exprData->type = TYPE_INT;
             doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }

        else if (token.tokenType == TOK_float)
        {
            exprData->type = TYPE_DOUBLE;
            isFloat = true;
             doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }
        else if(token.tokenType == TOK_openRoundBracket)
        {
            openBrackets++;
            doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }
        else if(token.tokenType == TOK_closeRoundBracket)
        {
            openBrackets--;
            if (openBrackets < 0)
            {
                 fprintf(stderr, "Chyba: Více zavíracích než otevíracích závorek.\n");
                exit(2);
            }
            doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }
        else if (token.tokenType == TOK_less ||
            token.tokenType == TOK_great ||
            token.tokenType == TOK_lessEq ||
            token.tokenType == TOK_greatEq ||
            token.tokenType == TOK_compEq ||
            token.tokenType == TOK_notEq
            )
        {
            help->comparisonOperationFound = true;
            exprData->type = TYPE_BOOL;
            doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        }
        

        else if (token.tokenType == TOK_add ||
            token.tokenType == TOK_sub ||
            token.tokenType == TOK_mul ||
            token.tokenType == TOK_div ||
            token.tokenType == TOK_less ||
            token.tokenType == TOK_great ||
            token.tokenType == TOK_lessEq ||
            token.tokenType == TOK_greatEq ||
            token.tokenType == TOK_notEq ||
            token.tokenType == TOK_exclam ||
            token.tokenType == TOK_openRoundBracket ||
            token.tokenType == TOK_closeRoundBracket ||
            token.tokenType == TOK_int ||
            token.tokenType == TOK_doubleQuest ||
            token.tokenType == TOK_float ||
            token.tokenType == TOK_compEq
            ) 
            {
            doOperation(&stack, token, postfixExpression, postfixExpressionLength);
        } else 
        {
            //Narazili jsme na token, který už nemá být ve výrazu
            break;
        }
        
        lastToken.tokenType = token.tokenType;
        getToken(&token, swiftFile); // Získáme další token
    }


    *firstToken = token;
    // Po dokončení vstupu, zpracujeme zbytek zásobníku
    debug("firstToken1: %d\n", firstToken->tokenType);


    //Vyprázdnímé zásbník a dáme na postfix
    while (!stackIsEmpty(&stack)) 
    {
        postfixExpression[(*postfixExpressionLength)++] = stackPop(&stack);
    }

    if(isFloat)
    {
        exprData->type = TYPE_DOUBLE;
    }

    if (help->comparisonOperationFound)
    {
        exprData->type = TYPE_BOOL;
    }

    if (openBrackets != 0)
    {
        fprintf(stderr, "Chyba: Nevyvážené závorky výrazu.\n");
        exit(2);
    }

    unsigned j = 0;
    for (unsigned i = 0; i < *postfixExpressionLength; i++) {
        if (postfixExpression[i].tokenType != TOK_openRoundBracket && postfixExpression[i].tokenType != TOK_closeRoundBracket) {
            postfixExpression[j++] = postfixExpression[i];
        }
    }
    *postfixExpressionLength = j; // Aktualizace délky postfixového výrazu



    // Nyní bychom měli mít kompletní postfixový výraz
    stackDestroy(&stack);

    /*
    fprintf(stderr, "Postfix Expression [Length %u]: ", *postfixExpressionLength);
        for (unsigned i = 0; i < *postfixExpressionLength; ++i) 
        {
                fprintf(stderr, "%d ", postfixExpression[i].tokenType);
        }
       fprintf(stderr ,"\n");

       */
    

    return postfixExpression;
}


// Funkce pro převod tokenů na řetězec
//Vloží celý posftix do struktury
void convertPostfixToString(const Token *postfixExpression, unsigned length, expressionStruct *exprData) {
    // Alokace paměti pro pole tokenů v exprData
    exprData->postFix = (Token*)malloc(sizeof(Token) * length);
    if (exprData->postFix == NULL) {
        // Ošetření chyby alokace paměti
        exit(9);
    }

    // Uložení délky postfixu
    exprData->postFixLen = length;

    // Kopírování tokenů do exprData->postFix
    for (unsigned i = 0; i < length; ++i) {
        exprData->postFix[i] = postfixExpression[i];
    }
}


//Funkce na převod dataType na TokenType z důvodu mojí kontroly typů
TokenType convertDataTypeToTokenType(dataTypeT dataType) {
    switch (dataType) {
        case INT:
        case INTQ:
            return TOK_int;
        case DOUBLE:
        case DOUBLEQ:
            return TOK_float;
        case STRING:
        case STRINGQ:
            return TOK_string;
        default:
            fprintf(stderr, "Chyba: Neznámý typ dat.\n");
            exit(5);
    }
}


//Funkce, která kontroluje, zda se sčítají stejné typy operandů
//nebo typy operandů, které jsou povoleny
void checkPostfixDataTypes(const Token *postfixExpression, unsigned length, Tsymstack *symstack, helpStruct *help, expressionStruct *exprData) {
    TokenType lastOperandType = TOK_EOF;

    for (unsigned i = 0; i < length; ++i) {
        TokenType currentType = postfixExpression[i].tokenType;

        if (currentType <= TOK_notEq) { // Pokud je to operátor, přeskočíme ho
            continue;
        }

        if (currentType == TOK_identifier) {
            Tdata* foundData = symstackSearchiId(symstack, postfixExpression[i].tokenValue.string);
            if (foundData != NULL) {
                currentType = convertDataTypeToTokenType(foundData->dataType);

                
            } else {
                fprintf(stderr, "Chyba: Neznámý identifikátor.\n");
                exit(5);
            }
        }


        if (lastOperandType == TOK_EOF) {
            lastOperandType = currentType;
        } else {
            if ((lastOperandType == TOK_int && currentType == TOK_float) || 
                (lastOperandType == TOK_float && currentType == TOK_int)) {
                if ((!help->comparisonOperationFound || help->canIntDoubleCoexist) && (exprData->allIdentifiersKnown || help->canIntDoubleCoexist)) {
                    continue; // Přetypování je povoleno
                }
            }

            if (lastOperandType != currentType) {
                fprintf(stderr, "Chyba: Nalezeny operandy různých datových typů.\n");
                exit(7);
            }
        }
    }
}

// Hlavní funkce, kterou volá Douby a naplní strukturu
// 1. První se zavolá convertInfixToPostfix, 
// 2. poté se zkontroluje, zda sedí typy operandů v poftixu 
// 3. pak se celý postfix vloží do struktury
expressionStruct createExpression(FILE *swiftFile, Tsymstack *symstack, Token *firstToken) {
    expressionStruct exprData;

    helpStruct help;


    unsigned postfixExpressionLength = 0;

    Token *postfixExpression = convertInfixToPostfix(swiftFile, &postfixExpressionLength, symstack, &exprData, firstToken, &help);

    

    checkPostfixDataTypes(postfixExpression, postfixExpressionLength, symstack, &help, &exprData);

    convertPostfixToString(postfixExpression, postfixExpressionLength, &exprData);
    
    return exprData;
}