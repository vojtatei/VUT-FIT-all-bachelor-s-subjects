/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief parser, syntaktická a sémantická analýza
 *
 * @author Doubravka Šimůnková (xsimun05), Jakub Jeřábek (xjerab28)
 */


#include "scanner.h"
#include "expression.h"
#include "codeGenerator.h"
#include "symtable.h"
#include "debug.h"
#include "symstack.h"
#include "parser.h"

#define COMPILER

bool isKeyword(Keyword keyword){
    return (token.tokenType==TOK_keyword)&&(token.tokenValue.keyword==keyword);
}

void initParser(FILE *swiftFile) {
    parser = (parser_t *) malloc(sizeof(parser_t));
    if(parser==NULL){
        exit(99);
    }
    swiftFile = swiftFile;
    parser->labelIndex = 0;
    parser->uniqId = 0;
    parser->inFunction = 0;
    parser->inWhile = 0;
    parser->inIf = 0;
    parser->pushNewTree = 0;
    parser->whileDefinitions = NULL;
    parser->whileDefinitionsCounter = NULL;
    parser->currentWhile = 0;
    tree = symtableInit();
    buildInFunctions();
    stack=symstackInit();
    
}

void genBuiltFunctions() {
    endProgram();
    builtWrite();
    builtReadInt();
    builtReadString();
    builtReadDouble();
    builtInt2Double();
    builtDouble2Int();
    builtLength();
    builtSubString();
    builtOrd();
    builtChr();
    builtCheckNil();
    doubleQuest();
}

void killParser(){
    parser->labelIndex = 0;
    parser->inFunction = false;
    parser->inWhile = 0;
    parser->inIf = 0;

    
    if (parser->whileDefinitions != NULL) {
        free(parser->whileDefinitions);
    }
    
    if (parser->whileDefinitionsCounter != NULL) {
        free(parser->whileDefinitionsCounter);
    }
    
    symstackDispose(stack);
}

void buildInFunctions(){
    Tdata* readString;
    readString=(Tdata*) malloc(sizeof(Tdata));
    if(readString==NULL){
        exit(99);
    }
    readString->initType=FUNC;
    readString->idName="readString";
    readString->dataType=STRINGQ;
    readString->numOfParam=0;
    symtableInsert(&tree, readString);
    symtableBalanceTree(&tree);

    Tdata* readInt;
    readInt=(Tdata*) malloc(sizeof(Tdata));
    if(readInt==NULL){
        exit(99);
    }
    readInt->initType=FUNC;
    debug("readint\n");
    readInt->idName="readInt";
    readInt->dataType=INTQ;
    readInt->numOfParam=0;
    symtableInsert(&tree, readInt);
    symtableBalanceTree(&tree);

    Tdata* readDouble;
    readDouble=(Tdata*) malloc(sizeof(Tdata));
    if(readDouble==NULL){
        exit(99);
    }
    readDouble->initType=FUNC;
    readDouble->idName="readDouble";
    readDouble->dataType=DOUBLEQ;
    readDouble->numOfParam=0;
    symtableInsert(&tree, readDouble);
    symtableBalanceTree(&tree);

    Tdata* Int2Double;
    Int2Double=(Tdata*) malloc(sizeof(Tdata));
    if(Int2Double==NULL){
        exit(99);
    }
    Int2Double->initType=FUNC;
    Int2Double->idName="Int2Double";
    Int2Double->dataType=DOUBLE;
    Int2Double->numOfParam=1;
    Int2Double->arrayOfParams=(Tparam*) malloc(sizeof(Tparam));
    if(Int2Double==NULL){
        exit(99);
    }
    Int2Double->arrayOfParams[0].name_par="_";
    Int2Double->arrayOfParams[0].id_par="term";
    Int2Double->arrayOfParams[0].type=INT;
    symtableInsert(&tree, Int2Double);
    symtableBalanceTree(&tree);

    Tdata* Double2Int;
    Double2Int=(Tdata*) malloc(sizeof(Tdata));
    if(Double2Int==NULL){
        exit(99);
    }
    Double2Int->initType=FUNC;
    Double2Int->idName="Double2Int";
    Double2Int->dataType=INT;
    Double2Int->numOfParam=1;
    Double2Int->arrayOfParams=(Tparam*) malloc(sizeof(Tparam));
    if(Double2Int->arrayOfParams==NULL){
        exit(99);
    }
    Double2Int->arrayOfParams[0].name_par="_";
    Double2Int->arrayOfParams[0].id_par="term";
    Double2Int->arrayOfParams[0].type=DOUBLE;
    symtableInsert(&tree, Double2Int);
    symtableBalanceTree(&tree);

    Tdata* buildInWrite;
    buildInWrite=(Tdata*) malloc(sizeof(Tdata));
    if(buildInWrite==NULL){
        exit(99);
    }
    buildInWrite->initType=FUNC;
    buildInWrite->idName="write";
    buildInWrite->dataType=VOID;
    buildInWrite->numOfParam=-1;//nekonečno
    symtableInsert(&tree, buildInWrite);
    symtableBalanceTree(&tree);

    Tdata* length;
    length=(Tdata*) malloc(sizeof(Tdata));
    if(length==NULL){
        exit(99);
    }
    length->initType=FUNC;
    length->idName="length";
    length->dataType=INT;
    length->numOfParam=1;
    length->arrayOfParams=(Tparam*) malloc(sizeof(Tparam));
    if(length->arrayOfParams==NULL){
        exit(99);
    }
    length->arrayOfParams[0].name_par="_";
    length->arrayOfParams[0].id_par="s";
    length->arrayOfParams[0].type=STRING;
    symtableInsert(&tree, length);
    symtableBalanceTree(&tree);

    Tdata* substring;
    substring=(Tdata*) malloc(sizeof(Tdata));
    if(substring==NULL){
        exit(99);
    }
    substring->initType=FUNC;
    substring->idName="substring";
    substring->dataType=STRINGQ;
    substring->numOfParam=3;
    substring->arrayOfParams=(Tparam*) malloc(3*sizeof(Tparam));
    if(substring->arrayOfParams==NULL){
        exit(99);
    }
    substring->arrayOfParams[0].name_par="of";
    substring->arrayOfParams[0].id_par="s";
    substring->arrayOfParams[0].type=STRING;
    substring->arrayOfParams[1].name_par="startingAt";
    substring->arrayOfParams[1].id_par="i";
    substring->arrayOfParams[1].type=INT;
    substring->arrayOfParams[2].name_par="endingBefore";
    substring->arrayOfParams[2].id_par="j";
    substring->arrayOfParams[2].type=INT;
    symtableInsert(&tree, substring);
    symtableBalanceTree(&tree);

    Tdata* ord;
    ord=(Tdata*) malloc(sizeof(Tdata));
    if(ord==NULL){
        exit(99);
    }
    ord->initType=FUNC;
    ord->idName="ord";
    ord->dataType=INT;
    ord->numOfParam=1;
    ord->arrayOfParams=(Tparam*) malloc(sizeof(Tparam));
    if(ord->arrayOfParams==NULL){
        exit(99);
    }
    ord->arrayOfParams[0].name_par="_";
    ord->arrayOfParams[0].id_par="c";
    ord->arrayOfParams[0].type=STRING;
    symtableInsert(&tree, ord);
    symtableBalanceTree(&tree);

    Tdata* chr;
    chr=(Tdata*) malloc(sizeof(Tdata));
    if(chr==NULL){
        exit(99);
    }
    chr->initType=FUNC;
    chr->idName="chr";
    chr->dataType=STRING;
    chr->numOfParam=1;
    chr->arrayOfParams=(Tparam*) malloc(sizeof(Tparam));
    if(chr->arrayOfParams==NULL){
        exit(99);
    }
    chr->arrayOfParams[0].name_par="_";
    chr->arrayOfParams[0].id_par="i";
    chr->arrayOfParams[0].type=INT;
    symtableInsert(&tree, chr);
    symtableBalanceTree(&tree);
}

int goThroughFunction(FILE *swiftFile) {
    int countBrackets = 1;

    while (token.tokenType != TOK_EOF) {
        getToken(&token, swiftFile);

        if (countBrackets == 0) {
            return 0;
        }
        else if (token.tokenType == TOK_openCurlyBracket) {
            countBrackets++;
        }
        else if (token.tokenType == TOK_closeCurlyBracket) {
            countBrackets--;
        }
        else if (token.tokenType == TOK_keyword && token.tokenValue.keyword == KW_while) {
            return 0;
        }
    }

    return 1;
}

//uloží deklarace funkcí do symtable a proměnné deklarované ve while do pole
int firstPassage(FILE *swiftFile) {
    getToken(&token, swiftFile);

    int whileCount = 0;
    int varCount = 0;

    while (token.tokenType != TOK_EOF) {

        if (isKeyword(KW_func)) {
            getToken(&token, swiftFile);

            enter(swiftFile);

            Tdata*data;//pro ukládání do symtable
            data=(Tdata*) malloc(sizeof(Tdata));
            if(data==NULL){
                exit(99);
            }
            data->initType=FUNC;

            //jmeno funkce
            if(token.tokenType!=TOK_identifier){
                exit(2); 
            }

            data->idName=token.tokenValue.string;

            getToken(&token, swiftFile);
            enter(swiftFile);

            if(token.tokenType!=TOK_openRoundBracket){
                exit(2); 
            }

            getToken(&token, swiftFile);
            enter(swiftFile);

            //získání parametrů funkce
            data->numOfParam=0; 
            listOfParams(swiftFile, data);

            if(token.tokenType!=TOK_closeRoundBracket){
                exit(2); 
            }
            getToken(&token, swiftFile);
            enter(swiftFile);

            //zjištění typu funkce
            returnType(swiftFile, data);

            Tsymsearch found = symtableSearch(&tree, data->idName);
            if(found.found==true){
                fprintf(stderr, "ERROR: redefinice %s\n", found.found_data->idName);
                exit(3);
            }
            else{
                symtableInsert(&tree, data);
                symtableBalanceTree(&tree);
            }
        

            // ted kontrola otevrene curly bracket
            if(token.tokenType!=TOK_openCurlyBracket){
                exit(2); 
            }

            if (goThroughFunction(swiftFile) == 1) {
                exit(2);
            }

        }

        if (isKeyword(KW_while)) {
            int currentVarCount = 0;
            parser->whileDefinitions = (Tdata *) realloc(parser->whileDefinitions, sizeof(Tdata) * (varCount+1));
            parser->whileDefinitionsCounter = (int *) realloc(parser->whileDefinitionsCounter, sizeof(int) * (whileCount+1));
            whileCount++;
            

            while (token.tokenType != TOK_openCurlyBracket) {
                getToken(&token, swiftFile);
                if (token.tokenType == TOK_EOF) {
                    fprintf(stderr, "CHYBA: Neocekavany konec souboru\n");
                    exit(2);
                }

            }

            int countBrackets = 1;


            while (token.tokenType != TOK_EOF) {

                getToken(&token, swiftFile);

                if (token.tokenType == TOK_openCurlyBracket) {
                    countBrackets++;
                }
                else if (countBrackets == 0) {
                    parser->whileDefinitionsCounter[whileCount-1] = varCount - currentVarCount;
                    break;
                }
                else if (token.tokenType == TOK_closeCurlyBracket) {
                    countBrackets--;
                }
                else if (token.tokenType == TOK_keyword && (token.tokenValue.keyword == KW_var || token.tokenValue.keyword == KW_let)) {
                    parser->whileDefinitions = (Tdata *) realloc(parser->whileDefinitions, sizeof(Tdata) * (varCount+1));
                    getToken(&token, swiftFile);
                    if (token.tokenType == TOK_EOF) exit(2);
                    enter(swiftFile);
                    parser->whileDefinitions[varCount].idName = token.tokenValue.string;
                    varCount++;
                }
            }
        }
        getToken(&token, swiftFile);
    }

    fseek(swiftFile, 0, SEEK_SET);
}

//hlavní funkce, jako jediná se volá explicitně
int program(FILE *swiftFile){

    firstPassage(swiftFile); //první průchod, zapsání deklarací funkcí do symtable a proměnných deklarovaných ve while do pole

    symstackPush(stack, tree); //přidání globálního symtablu na symstack
    initGenerator();

    getToken(&token, swiftFile);
    if(token.tokenType==TOK_EOL || token.tokenType==TOK_EOF || token.tokenType==TOK_identifier || isKeyword(KW_func) || isKeyword(KW_var) || isKeyword(KW_let) || isKeyword(KW_while) || isKeyword(KW_if)){
        enter(swiftFile);
        sequenceOrFunction(swiftFile);//tělo programu
        if(token.tokenType==TOK_EOF){
            return 0;
        }
        else{
            exit(2);
        }
    }
    else{
        exit(2); 
    }
}

//když je token EOL, jde na další token
int enter(FILE *swiftFile){
    while(token.tokenType==TOK_EOL){
        getToken(&token, swiftFile);
    }
    return 0;
}

//hlavní tělo programu, buď deklarace funkce nebo sekvence příkazů, popř. prázdné tělo
int sequenceOrFunction(FILE *swiftFile){

    //větev pro žádné příkazy ani deklarace
    if(token.tokenType==TOK_EOF){
        return 0;
    }
   
    //větev pro deklaraci funkce
    else if(isKeyword(KW_func)){
        debug("infunction\n");
        Tnode* localTree = symtableInit();

        getToken(&token, swiftFile);
        enter(swiftFile);

        parser->inFunction=true;

        //jmeno funkce
        if(token.tokenType!=TOK_identifier){
            exit(2); 
        }

        char *funcName = token.tokenValue.string;

        while (token.tokenType != TOK_openCurlyBracket) {
            getToken(&token, swiftFile);
        }

        if(token.tokenType!=TOK_openCurlyBracket){
            exit(2); 
        }
        Tsymsearch funkce = symtableSearch(&tree, funcName);

        for (int param = 0; param < funkce.found_data->numOfParam; param++) {
            if(strcmp(funkce.found_data->arrayOfParams[param].id_par,"_")!=0){
                Tdata *paramData = (Tdata *) malloc(sizeof(Tdata));
                if(paramData==NULL){
                    exit(99);
                }
                paramData->idName = funkce.found_data->arrayOfParams[param].id_par;
                paramData->dataType = funkce.found_data->arrayOfParams[param].type;
                paramData->initType = LET;
                paramData->uniqId = 0;
                paramData->isInit = true;
                paramData->localFrame = true;
                symtableInsert(&localTree, paramData);
                symtableBalanceTree(&localTree);
            }
        }
        symstackPush(stack, localTree);

        getToken(&token, swiftFile);
        enter(swiftFile);

        generateFunctionHead(funkce.found_data);

        //tělo funkce
        bool IsReturn=false;
        sequence(swiftFile, &IsReturn, funkce.found_data->dataType);//sekvence příkazů
        if(IsReturn==false && funkce.found_data->dataType!=VOID){
            fprintf(stderr, "ERROR: NO RETURN in function %s\n", funkce.found_data->idName);
            exit(2);
        }
        symstackPop(stack);

        if(token.tokenType!=TOK_closeCurlyBracket){
            exit(2); 
        }

        generateFunctionEnd(funkce.found_data);

        getToken(&token, swiftFile);

        //správný konec funkce, buď EOL nebo EOF
        if(token.tokenType==TOK_EOL){
            getToken(&token, swiftFile);
            enter(swiftFile);
        }
        else if(token.tokenType!=TOK_EOF){
            exit(2); 
        }

        parser->inFunction=false;//už nejsme ve funkci

        //rekurzivní volání, můžou následovat další příkazy nebo deklarace funkce
        sequenceOrFunction(swiftFile);
        return 0;
    }//konec větve pro deklaraci funkce

    //větev pro ostatní příkazy
    else if(token.tokenType==TOK_identifier || isKeyword(KW_var) || isKeyword(KW_let) || isKeyword(KW_while) || isKeyword(KW_if)){
        debug("inothersequence\n");
        bool IsReturn;
        sequence(swiftFile, &IsReturn, VOID);
        sequenceOrFunction(swiftFile);
        return 0;
    }

    //větev pro špatný token
    else{
        exit(2); 
    }
}

//seznam parametrů funkce, kterou deklarujeme
int listOfParams(FILE *swiftFile, Tdata* data){
    //žádné parametry
    if(token.tokenType==TOK_closeRoundBracket){
        return 0;
    }

    else{
        data->arrayOfParams=(Tparam*) malloc(sizeof(Tparam));//alokování místa pro 1 parametr
        if(data->arrayOfParams==NULL){
            exit(99);
        }
        parameter(swiftFile, data); //1. parametr
        parameters(swiftFile, data); //další parametry
        return 0;
    }
}

//parametry funkce oddělené čárkou, od 2. parametru výš
int parameters(FILE *swiftFile, Tdata* data){
    //žádný další parametr
    if(token.tokenType==TOK_closeRoundBracket){
        return 0;
    }

    //další parametry oddělené čárkou
    else if(token.tokenType==TOK_comma){
        getToken(&token, swiftFile);
        enter(swiftFile);

        data->arrayOfParams=(Tparam*) realloc(data->arrayOfParams, (data->numOfParam+1)*sizeof(Tparam)); //alokace paměti pro další parametr

        parameter(swiftFile, data);
        //kontrola, že parametry nemají stejné id
        for(int i=2; i <= (data->numOfParam); i++){
            if(strcmp(data->arrayOfParams[data->numOfParam -1].id_par, data->arrayOfParams[data->numOfParam -i].id_par)==0){
                fprintf(stderr, "ERROR: redefinice parametru %s\n", data->arrayOfParams[data->numOfParam -1].id_par);
                exit(3);
            }
        }
        parameters(swiftFile, data);
        return 0;
    }

    else{
        exit(2);
    }
}

//parametr funkce, kterou deklarujeme
int parameter(FILE *swiftFile, Tdata* data){
    
    //jmeno parametru
    if(token.tokenType==TOK_identifier){
        data->arrayOfParams[data->numOfParam].name_par=token.tokenValue.string;
    }
    else if(token.tokenType==TOK_underScore){
        data->arrayOfParams[data->numOfParam].name_par="_";
    }
    else{
        exit(2);
    }
    getToken(&token,swiftFile);
    enter(swiftFile);

    //id parametru
    if(token.tokenType==TOK_identifier){
        data->arrayOfParams[data->numOfParam].id_par=token.tokenValue.string;
    }
    else if(token.tokenType==TOK_underScore){
        data->arrayOfParams[data->numOfParam].id_par="_";
    }
    else{
        exit(2);
    }
    getToken(&token,swiftFile);
    enter(swiftFile);

    //jmeno a id parametru se nesmí rovnat
    if(strcmp(data->arrayOfParams[data->numOfParam].name_par, data->arrayOfParams[data->numOfParam].id_par)==0 && strcmp(data->arrayOfParams[data->numOfParam].name_par,"_")!=0){
        exit(2);
    }

    if(token.tokenType!=TOK_colon){
        exit(2);
    }
    getToken(&token,swiftFile);
    enter(swiftFile);

    //zjištění typu parametru
    dataTypeT dataType; 
    type(swiftFile, &dataType);
    enter(swiftFile);
    data->arrayOfParams[data->numOfParam].type=dataType;
    
    //zvětšení počtu parametrů
    data->numOfParam+=1;
    return 0;
}

//návratový typ funkce
int returnType(FILE *swiftFile, Tdata* data){
    //bez návratového typu
    if(token.tokenType==TOK_openCurlyBracket){
        data->dataType=VOID;
        return 0;
    }

    //s návratovým typem
    else if(token.tokenType==TOK_arrow){
        getToken(&token, swiftFile);
        enter(swiftFile);
        dataTypeT dataType;
        type(swiftFile, &dataType);
        enter(swiftFile);
        data->dataType=dataType;
        return 0;
    }

    else{
        exit(2);
    }
}

//zjistí typ a uloží do dataType
int type(FILE *swiftFile, dataTypeT* dataType){
    if(isKeyword(KW_double) || isKeyword(KW_doubleQ) || isKeyword(KW_int) || isKeyword(KW_intQ) || isKeyword(KW_string) || isKeyword(KW_stringQ)){
         switch (token.tokenValue.keyword){
            case KW_double:
                *dataType=DOUBLE;
                break;
            case KW_doubleQ:
                *dataType=DOUBLEQ;
                break;
            case KW_int:
                *dataType=INT;
                break;
            case KW_intQ:
                *dataType=INTQ;
                break;
            case KW_string:
                *dataType=STRING;
                break;
            case KW_stringQ:
                *dataType=STRINGQ;
                break;
            }
        getToken(&token, swiftFile);
        return 0;
    }
    else{
        exit(2);
    }
}

//sekvence příkazů
int sequence(FILE *swiftFile, bool* IsReturn, dataTypeT functionType){

    debug("sekvence prikazu, token:%s\n", tokenTypeStrings[token.tokenType]);
    //větev pro žádné příkazy
    if(token.tokenType==TOK_EOF || token.tokenType==TOK_closeCurlyBracket || isKeyword(KW_func)){
        debug("zadne dalsi\n");
        return 0;
    }

    //větev pro var/let deklarace
    else if(isKeyword(KW_var) || isKeyword(KW_let)){
        Tdata*data; //struktura pro ukládání do symtable a pro generátor
        data=(Tdata*) malloc(sizeof(Tdata));
        if(data==NULL){
            exit(99);
        }
        //let nebo var do struktury
        if(isKeyword(KW_var)){
            data->initType=VAR;
        }
        else{
            data->initType=LET;
        }
        data->isInit=false;
        data->localFrame = parser->inFunction || parser->inIf || parser->inWhile;

        getToken(&token, swiftFile);
        enter(swiftFile);

        //jmeno promenne
        if(token.tokenType!=TOK_identifier){
            exit(2); 
        }
        if (parser->inWhile >= 1) {
            for (int i = parser->whileDefinitionsCounter[parser->currentWhile-1]-1; i >= 0; i--) {
                if (strcmp(parser->whileDefinitions[i].idName, token.tokenValue.string) == 0){
                    data->idName=parser->whileDefinitions[i].idName;
                    data->uniqId=parser->whileDefinitions[i].uniqId;
                }
            }
        }
        else {
            data->idName=token.tokenValue.string;
            data->uniqId=parser->uniqId;
            parser->uniqId++;
        }
        getToken(&token, swiftFile);
        enter(swiftFile);

        if(parser->inFunction==true || parser->inIf>0 || parser->inWhile>0){
            data->localFrame=true;
        }
        else{
            data->localFrame=false;
        }
        //typ a hodnota
        expressionStruct* result;
        result=(expressionStruct*)malloc(sizeof(expressionStruct));
        if(result==NULL){
            exit(99);
        }
        //nastaveni default hodnot
         if(data->initType==VAR){
            result->postFix=NULL;
        }
        else{
            result->postFix=NULL;
        }
        

        determineType(swiftFile, data, result); 
        
        debug("typ promenne:%d, jmeno:%s letvar:%d\n",data->dataType, data->idName,data->initType);

        //správný konec příkazu, EOL, EOF nebo ukončujíci množinová závorka
        if(token.tokenType==TOK_EOL){
            getToken(&token, swiftFile);
            enter(swiftFile);
        }
        else if(token.tokenType!=TOK_EOF && token.tokenType!=TOK_closeCurlyBracket){
            debug("spatny konec prikazu, token je:%d\n", token.tokenType);
            exit(2); 
        }

        //kontrola redeklarace a uložení do symtable

            Tnode* currentTree = symstackTop(stack);
            
            if (currentTree == NULL) parser->pushNewTree = 1;
            
            Tsymsearch found = symtableSearch(&currentTree, data->idName);

            if(found.found==true){
                fprintf(stderr, "ERROR: redefinice %s\n", found.found_data->idName);
                exit(3);
            }
            
            debug("insertuju: typ %d, jmeno %s\n", data->dataType, data->idName);
            symtableInsert(&currentTree, data);

            if (parser->pushNewTree == 1) {
                symtableBalanceTree(&currentTree);
                symstackPush(stack, currentTree);
                parser->pushNewTree = 0;
            }

        debug("vygenerovano\n");
        sequence(swiftFile, IsReturn, functionType); //rekurzivní volání
        debug("konec rekurze sequence\n");
        return 0;
    }

    //větev pro if podmínku
    else if(isKeyword(KW_if)){
        getToken(&token, swiftFile);
        enter(swiftFile);
        parser->labelIndex +=1; //míra zanoření se zvětší
        parser->inIf +=1;
        bool IsReturnIf=false;
        bool IsReturnElse=false;
        int currentLabel = parser->labelIndex;

        //pravdivostní výraz
        Tdata *letVariable = NULL;
        statement(swiftFile, currentLabel, &letVariable);

        //sekce if
        if(token.tokenType!=TOK_openCurlyBracket){
            exit(2);
        }

        bool wasVar = 0;
        if (letVariable != NULL && letVariable->initType == VAR) {
            letVariable->initType = LET;
            wasVar = 1;
        }

        getToken(&token, swiftFile);
        enter(swiftFile);

        Tnode* ifTree = symtableInit();
        symstackPush(stack, ifTree);
        sequence(swiftFile, &IsReturnIf, functionType);
        symstackPop(stack);

        if(token.tokenType!=TOK_closeCurlyBracket){
            exit(2);
        }
        getToken(&token, swiftFile);
        enter(swiftFile);

        if(!isKeyword(KW_else)){
            exit(2);
        }

        //změna datového typu zpět po vystoupení z if při "let id" podmínce
        if (letVariable != NULL) {
            if (letVariable->dataType == INT) {
                letVariable->dataType = INTQ;
            } 
            else if (letVariable->dataType == DOUBLE) {
                letVariable->dataType = DOUBLEQ;
            }
            else if (letVariable->dataType == STRING) {
                letVariable->dataType = STRINGQ;
            }
        }


        generateElse(currentLabel, letVariable);
        getToken(&token, swiftFile);
        enter(swiftFile);

        //sekce else
        if(token.tokenType!=TOK_openCurlyBracket){
            exit(2);
        }
        getToken(&token, swiftFile);
        enter(swiftFile);

        Tnode* elseTree = symtableInit();
        symstackPush(stack, elseTree);
        sequence(swiftFile, &IsReturnElse, functionType);

        symstackPop(stack);

        if(token.tokenType!=TOK_closeCurlyBracket){
            exit(2);
        }
        generateEndElse(currentLabel);
        getToken(&token, swiftFile);

        //korektní konec příkazu, EOL, EOF nebo ukončující množinové závorky
        if(token.tokenType==TOK_EOL){
            getToken(&token, swiftFile);
            enter(swiftFile);
        }
        else if(token.tokenType!=TOK_EOF && token.tokenType!=TOK_closeCurlyBracket){
            exit(2); 
        }

        parser->inIf -=1;

        debug("if:%d else:%d\n",IsReturnIf,IsReturnElse);
        *IsReturn=(IsReturnIf && IsReturnElse);

        if (wasVar) {
            letVariable->initType = LET;
        }

        sequence(swiftFile, IsReturn, functionType); 
        return 0;  
    }

    //větev pro while
    else if(isKeyword(KW_while)){
        debug("ve while\n");
        getToken(&token, swiftFile);
        enter(swiftFile);

        parser->labelIndex+=1;
        parser->inWhile +=1;

        if (parser->inWhile == 1 && parser->whileDefinitionsCounter != NULL) {
            String *numValue = (String *) malloc(sizeof(String));
            if(numValue==NULL){
                exit(99);
            }
            initStr(numValue);

            for (int i = parser->whileDefinitionsCounter[parser->currentWhile]-1; i >= 0; i--) {
                snprintf(numValue->string, numValue->allocSize, "%d", parser->uniqId);
                inst(3, "DEFVAR LF@", parser->whileDefinitions[i].idName, numValue->string);
                parser->whileDefinitions[i].uniqId = parser->uniqId;
                parser->uniqId++;
                clearStr(numValue);
            }
            parser->currentWhile++;
            freeStr(numValue);
        }

        int currentLabel = parser->labelIndex;

        expressionStruct result = createExpression(swiftFile, stack, &token);
        if(result.type!=TYPE_BOOL){
            fprintf(stderr, "ERROR: podmínka ve while není typu BOOL\n");
            exit(7);
        }
        generateWhileHead(&result, currentLabel);
        enter(swiftFile);

        //tělo while
        if(token.tokenType!=TOK_openCurlyBracket){
            exit(2);
        } 
        getToken(&token, swiftFile);
        enter(swiftFile);
        
        Tnode* whileTree = symtableInit();
        symstackPush(stack, whileTree);
        sequence(swiftFile, IsReturn, functionType);
        symstackPop(stack);

        if(token.tokenType!=TOK_closeCurlyBracket){
            exit(2);
        }
        generateWhileEnd(currentLabel);
        getToken(&token, swiftFile);

        //korektní konec příkazu, EOL, EOF nebo ukončující množinová závorka
        if(token.tokenType==TOK_EOL){
            getToken(&token, swiftFile);
            enter(swiftFile);
        }
        else if(token.tokenType!=TOK_EOF && token.tokenType!=TOK_closeCurlyBracket){
            exit(2); 
        }

        parser->inWhile -=1;

        sequence(swiftFile, IsReturn, functionType); 
        return 0;  
    }

    //větev pro přiřazení do proměnné nebo volání funkce
    else if(token.tokenType==TOK_identifier){

        debug("vetev pro volani nebo prirazeni\n");
        //je to identifikátor proměnné?
        bool isVariable=false;

        Tdata* variable = symstackSearchiId(stack, token.tokenValue.string);

        if(variable==NULL){//identifikator neni v symtable
            getToken(&token, swiftFile);
            //nedeklarovana funkce
            if(token.tokenType==TOK_openRoundBracket){
                fprintf(stderr, "ERROR: nedefinovaná funkce\n");
                exit(3);
            }
            //nedeklarovana promenna
            else if(token.tokenType==TOK_equal){
                fprintf(stderr, "ERROR: nedefinovaná proměnná\n");
                exit(5);
            }
            else{
                exit(2);
            }
        }
        if(variable->initType==LET || variable->initType==VAR){
            debug("nasla jsem promennou v symtable\n");
            isVariable=true;
        }

        //pokud je to identifikátor proměnné, tak variable=...
        if(isVariable){
            debug("variable=\n");
            //hodnota let se nesmí měnit
            if(variable->initType==LET && variable->isInit==true){
                fprintf(stderr, "ERROR: hodnota konstanty %s se nesmí měnit\n", variable->idName);
                exit(9);
            }
            getToken(&token,swiftFile);
            enter(swiftFile);

            if(token.tokenType!=TOK_equal){
                exit(2);
            }
            getToken(&token,swiftFile);
            enter(swiftFile);

            expressionStruct* result;
            result=(expressionStruct*)malloc(sizeof(expressionStruct));
            if(result==NULL){
                exit(99);
            }
            valueToVariable(swiftFile, variable, result, false);
           
            debug("prirazeno\n");
        }

        //pokud to není identifikátor proměnné, tak volání funkce bez návratové hodnoty
        else{
            debug("function without return\n");
            callingFunction(swiftFile, variable);
        }

        //korektní konec příkazu, EOL, EOF nebo ukončující množinová závorka
        if(token.tokenType==TOK_EOL){
            getToken(&token, swiftFile);
            enter(swiftFile);
        }
        else if(token.tokenType!=TOK_EOF && token.tokenType!=TOK_closeCurlyBracket){
            exit(2); 
        }
        sequence(swiftFile, IsReturn, functionType); 
        return 0; 
    }

    //větev pro návrat z funkce
    else if(isKeyword(KW_return)){
        getToken(&token, swiftFile);

        //pokud nejsme ve funkci, tak chyba
        if(parser->inFunction==false){
            exit(2);
        }
        
        debug("kontroluju navratovou hodnotu returnu\n");
        //určitě s návratovou hodnotou
        if(token.tokenType==TOK_identifier || token.tokenType==TOK_openRoundBracket || token.tokenType==TOK_int || token.tokenType==TOK_float || token.tokenType==TOK_string || isKeyword(KW_nil)){
            expressionStruct result = createExpression(swiftFile, stack, &token);
            if(result.type==TYPE_BOOL || (result.type==TYPE_VOID && functionType!=DOUBLEQ && functionType!=INTQ && functionType!=STRINGQ) || (result.type==TYPE_DOUBLE && functionType!=DOUBLE && functionType!=DOUBLEQ) || (result.type==TYPE_INT && functionType!=INT && functionType!=INTQ) || (result.type==TYPE_STRING && functionType!=STRING && functionType!=STRINGQ)){
                debug("spatny return type\n");
                debug("expected type of return:%d, actual:%d\n", functionType, result.type);
                //Processed keyword %s\n", keywords[token->tokenValue.keyword]
                fprintf(stderr, "ERROR: špatný typ u návratu z funkce\n");
                exit(6);
            }
            generateFunctionReturn(&result);
        }
        //neni tam navratova hodnota
        else{
            if(functionType!=VOID){
                fprintf(stderr, "ERROR: chybějící hodnota u návratu z funkce\n");
                exit(6);
            }
            expressionStruct *result = NULL;
            generateFunctionReturn(result);
        }

        //korektní konec příkazu, EOL, EOF nebo ukončující množinová závorka
        if(token.tokenType==TOK_EOL){
            getToken(&token, swiftFile);
            enter(swiftFile);
        }
        else if(token.tokenType!=TOK_EOF && token.tokenType!=TOK_closeCurlyBracket){
            exit(2); 
        }
        *IsReturn=true;
        debug("return je true %d\n", *IsReturn);

        sequence(swiftFile, IsReturn, functionType); 
        return 0; 
    }

    //větev pro špatný token
    else{
        exit(2); 
    }
}

//typ a případná hodnota deklarované proměnné
int determineType(FILE *swiftFile, Tdata* data, expressionStruct* result){
            
    //: s explicitně daným typem
    if(token.tokenType==TOK_colon){
        getToken(&token, swiftFile);
        enter(swiftFile);
        
        //zjištění typu
        dataTypeT dataType; 
        type(swiftFile, &dataType);
        data->dataType=dataType; 

        if((data->dataType==INTQ || data->dataType==DOUBLEQ || data->dataType==STRINGQ) && data->isInit==VAR){
            data->isInit=true;
        }  



        //má určitě i hodnotu
        if(token.tokenType==TOK_equal){
            getToken(&token, swiftFile);
            enter(swiftFile);
            data->isInit=true;
            valueToVariable(swiftFile, data, result, true);
            
            return 0;
        }
        //je určitě bez hodnoty
        else if(parser->inWhile == 0 && (token.tokenType==TOK_closeCurlyBracket || token.tokenType==TOK_EOF || token.tokenType==TOK_EOL)){
            generateVariable(data);
            return 0;
        }

        
        else{
            exit(2);
        }
        
    }

    //= bez explicitního typu
    else if(token.tokenType==TOK_equal){
        data->dataType=VOID;
        getToken(&token, swiftFile);
        enter(swiftFile);
        data->isInit=true;

        valueToVariable(swiftFile, data, result, true);

        return 0;
    }

    //špatný token
    else{
        exit(2);
    }
}

//pravdivostní výraz pro if
int statement(FILE *swiftFile, int currentLabel, Tdata **letVariable){
    //let id
    if(isKeyword(KW_let)){
        getToken(&token, swiftFile);
        enter(swiftFile);
        if(token.tokenType!=TOK_identifier){
            exit(2);
        }
        *letVariable = symstackSearchiId(stack, token.tokenValue.string);

        if(*letVariable==NULL || (*letVariable)->isInit==false || ((*letVariable)->initType!=VAR && (*letVariable)->initType!=LET)){
            fprintf(stderr, "ERROR: použití nedefinované nebo neinicializované proměnné %s\n", token.tokenValue.string);
            exit(5);
        }

        //změna typu proměnné na typ bez nil
        if ((*letVariable)->dataType == INTQ) {
            (*letVariable)->dataType = INT;
        } 
        else if ((*letVariable)->dataType == DOUBLEQ) {
            (*letVariable)->dataType = DOUBLE;
        }
        else if ((*letVariable)->dataType == STRINGQ) {
            (*letVariable)->dataType = STRING;
        }
        else {
            fprintf(stderr, "error: initializer for conditional binding must have Optional type, not %d\n", (*letVariable)->dataType);
            exit(9);
        }

        generateIfLet(*letVariable, currentLabel);

        getToken(&token, swiftFile);
        enter(swiftFile);
        return 0;
    }

    //vyraz
    else if(token.tokenType==TOK_identifier || token.tokenType==TOK_openRoundBracket || token.tokenType==TOK_int || token.tokenType==TOK_float || token.tokenType==TOK_string || isKeyword(KW_nil)){
        expressionStruct result = createExpression(swiftFile, stack, &token);
        if(result.type!=TYPE_BOOL){
            fprintf(stderr, "CHYBA: V IF NENI BOOL VYRAZ");
            exit(7);
        }
        generateIf(&result, currentLabel);
        enter(swiftFile);
        return 0;
    }

    else{
        exit(2);
    }
    
}



//přiřazení hodnoty do proměnné, buď výraz nebo návratová hodnota funkce
int valueToVariable(FILE *swiftFile, Tdata* data, expressionStruct* result, bool declaring){
    data->isInit=true;
    //pokud je token identifikátor, je to identifikátor proměnné?
    bool isVariable=false;
    Tdata* variable;
    if(token.tokenType==TOK_identifier){
        variable = symstackSearchiId(stack, token.tokenValue.string);
        //identifikator neni v symtable
        if(variable==NULL){
            getToken(&token, swiftFile);
            //nedeklarovana funkce
            if(token.tokenType==TOK_openRoundBracket){
                fprintf(stderr, "ERROR: nedefinovaná funkce\n");
                exit(3);
            }
            //nedeklarovana promenna
            else if(token.tokenType==TOK_add || token.tokenType==TOK_sub || token.tokenType==TOK_mul || token.tokenType==TOK_div || token.tokenType==TOK_doubleQuest || token.tokenType==TOK_exclam || token.tokenType==TOK_equal || token.tokenType==TOK_notEq || token.tokenType==TOK_less || token.tokenType==TOK_lessEq || token.tokenType==TOK_great || token.tokenType==TOK_greatEq || token.tokenType==TOK_EOL || token.tokenType==TOK_EOF){
                fprintf(stderr, "ERROR: nedefinovaná proměnná\n");
                exit(5);
            }
            else{
                exit(2);
            }
        }
        if(variable->initType==LET || variable->initType==VAR){
            isVariable=true;
        }
    }
    
    //vyraz 
    if((token.tokenType==TOK_identifier && isVariable) || token.tokenType==TOK_openRoundBracket || token.tokenType==TOK_int || token.tokenType==TOK_float || token.tokenType==TOK_string || isKeyword(KW_nil)){
        debug("Vytvarim expressionSTruct\n");
        *result=createExpression(swiftFile, stack, &token);

        if(result->type==TYPE_BOOL){
            fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");
            exit(7);
        }
        if(data->dataType!=VOID && ( (result->type==TYPE_DOUBLE && data->dataType!=DOUBLE && data->dataType!=DOUBLEQ) || (result->type==TYPE_INT && data->dataType!=INT && data->dataType!=INTQ && data->dataType!=DOUBLE && data->dataType!=DOUBLEQ) || (result->type==TYPE_STRING && data->dataType!=STRING && data->dataType!=STRINGQ) )){

            fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");

            exit(7);
        }
        if(result->type==TYPE_VOID && data->dataType!=DOUBLEQ && data->dataType!=INTQ && data->dataType!=STRINGQ && data->dataType!=VOID){
            fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");
            exit(7);
        }
        if(result->type==TYPE_INT && (data->dataType==DOUBLE || data->dataType==DOUBLEQ)){
            for(int i=0; i<result->postFixLen; i++){
                if(result->postFix[i].tokenType==TOK_identifier){
                    fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");
                    exit(7);
                }
            }
            
        }

        //odvození typu
        if(data->dataType==VOID){
            debug("vkladam typ\n");
            if(result->type==TYPE_DOUBLE){
                debug("typ double\n");
                data->dataType=DOUBLE;
            }
            else if(result->type==TYPE_INT){
                debug("typ int\n");
                data->dataType=INT;
            }
            else if(result->type==TYPE_STRING){
                debug("typ string\n");
                data->dataType=STRING;
            } 
            else if(result->type==TYPE_VOID){
                fprintf(stderr, "ERROR: nelze odvodit typ proměnné\n");
                exit(8);
            }
            debug("vlozila jsem typ:%d\n",data->dataType);
        }

        if(declaring==1 && parser->inWhile == 0){
            generateVariable(data);
        }
        
        assignValue(data, result);

        return 0;
    }

    //funkce, token je identifikátor, ale ne proměnné
    else if(token.tokenType==TOK_identifier){
        if(variable->dataType==VOID){
            fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");
            exit(7);
        }
        if( (data->dataType==INT && variable->dataType!=INT) || (data->dataType==DOUBLE && variable->dataType!=DOUBLE && variable->dataType!=INT) || (data->dataType==STRING && variable->dataType!=STRING) ){
            fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");
            exit(7);
        }
        if ((data->dataType==INTQ && variable->dataType!=INT && variable->dataType!=INTQ) || (data->dataType==DOUBLEQ && variable->dataType!=DOUBLE && variable->dataType!=DOUBLEQ && variable->dataType!=INT && variable->dataType!=INTQ) || (data->dataType==STRINGQ && variable->dataType!=STRING && variable->dataType!=STRINGQ)){
            fprintf(stderr, "ERROR: konfliktní typ při přiřazení do proměnné\n");
            exit(7);
        }

        //odvození typu
        if (data->dataType==VOID){
            data->dataType=variable->dataType;
        }

        if (declaring == 1 && parser->inWhile == 0){
            generateVariable(data);
        }

        callingFunction(swiftFile, variable);
        // Hodnotu z funkce priradim do promenne
        assignFunctionValue(data);
        return 0;
    }

    else{
        exit(2);
    }
}

//volání funkce, víme že token je id
int callingFunction(FILE *swiftFile, Tdata* function){
    Tdata* foundData = symstackSearchiId(stack, token.tokenValue.string);
    if(foundData==NULL){
        fprintf(stderr, "ERROR: nedefinovaná funkce\n");
        exit(3);
    }

    if(foundData!=NULL){debug("nalezena funkce, hledam:%s nalezeno:%s\n", token.tokenValue.string, foundData->idName);}

    getToken(&token,swiftFile);
    enter(swiftFile);

    if(token.tokenType!=TOK_openRoundBracket){
        exit(2);
    }
    getToken(&token,swiftFile);
    enter(swiftFile);

    listOfEnterParams(swiftFile, function);//vstupní parametry funkce

    if(token.tokenType!=TOK_closeRoundBracket){
        exit(2);
    }
    getToken(&token,swiftFile);
    return 0;
}

//seznam vstupních parametrů, když voláme funkci
int listOfEnterParams(FILE *swiftFile, Tdata* function){
    Tdata*enterParams;//seznam parametru pro generator

    //žádné parametry
    if(token.tokenType==TOK_closeRoundBracket){
        if((function->numOfParam)!=0){
            fprintf(stderr, "ERROR: špatný počet paramterů funkce %s\n", function->idName);
            exit(4);
        }
        callFunction(function, enterParams, 0);
    }

    else if(token.tokenType==TOK_identifier || token.tokenType==TOK_int || token.tokenType==TOK_float || token.tokenType==TOK_string || isKeyword(KW_nil)){
        if((function->numOfParam)==0){
            fprintf(stderr, "ERROR: špatný počet paramterů funkce %s\n", function->idName);
            exit(4);
        }
        
        enterParams=(Tdata*) malloc(sizeof(Tdata));
        if(enterParams==NULL){
            exit(99);
        }
        enterParameter(swiftFile, function, 0, enterParams);
        enterParameters(swiftFile, function, 1, enterParams);
    }
    else{
        exit(2);
    }
    
    return 0;
}

//vstupní parametry oddělené čárkou, od 2. výš
int enterParameters(FILE *swiftFile, Tdata* function, int parameterCount, Tdata* enterParams){
    //žádné další parametry
    if(token.tokenType==TOK_closeRoundBracket){
        if(parameterCount!=function->numOfParam && function->numOfParam!=-1){
            fprintf(stderr, "ERROR: špatný počet paramterů funkce %s\n", function->idName);
            exit(4);
        }

        callFunction(function, enterParams, parameterCount);

        return 0;
    }

    //další parametry
    else if(token.tokenType==TOK_comma){
        if(parameterCount==function->numOfParam){
            fprintf(stderr, "ERROR: špatný počet paramterů funkce %s\n", function->idName);
            exit(4);
        }
        getToken(&token,swiftFile);
        enter(swiftFile);
        enterParams=(Tdata*) realloc(enterParams, (parameterCount+1)*sizeof(Tdata));
        enterParameter(swiftFile, function, parameterCount, enterParams);
        parameterCount++;
        enterParameters(swiftFile, function, parameterCount, enterParams);
        return 0;
    }
    else{
        exit(2);
    }
}

int enterParameter(FILE *swiftFile, Tdata* function, int parameterCount, Tdata* enterParams){
    //parametr má jméno, zkontroluju ho
    if(function->numOfParam!=-1 && strcmp(function->arrayOfParams[parameterCount].name_par, "_")!=0){
        if(token.tokenType!=TOK_identifier){
            exit(2);
        }
        if(strcmp(token.tokenValue.string, function->arrayOfParams[parameterCount].name_par)!=0){
            fprintf(stderr, "ERROR: špatné jméno parametru funkce %s\n", function->idName);
            exit(4);
        }
        getToken(&token, swiftFile);
        enter(swiftFile);
        if(token.tokenType!=TOK_colon){
            exit(2);
        }
        getToken(&token, swiftFile);
        enter(swiftFile);  
    }

    //kontrola správnosti parametru
    //parametr je proměnná
    if(token.tokenType==TOK_identifier){
        bool isVariable=false;
        Tdata* variable = symstackSearchiId(stack, token.tokenValue.string);
        if(variable==NULL || (variable->initType!=LET && variable->initType!=VAR) || (variable->isInit==false)){
            debug("nenasla jsem promennou v symtable\n");
            fprintf(stderr, "ERROR: použití nedefinované nebo neinicializované proměnné %s\n", token.tokenValue.string);
            exit(5);
        }
        if(function->numOfParam!=-1 && (function->arrayOfParams[parameterCount].type==INT || function->arrayOfParams[parameterCount].type==DOUBLE || function->arrayOfParams[parameterCount].type==STRING) && (function->arrayOfParams[parameterCount].type!=variable->dataType)){
            fprintf(stderr, "ERROR: špatný typ paramteru funkceeeeee %s variable datatype: %d parameter type: %d\n", function->idName, function->dataType, function->arrayOfParams[parameterCount].type);
            exit(4);
        }
        if(function->numOfParam!=-1 && ( (function->arrayOfParams[parameterCount].type==INTQ && variable->dataType!=INT && variable->dataType!=INTQ) || (function->arrayOfParams[parameterCount].type==DOUBLEQ && variable->dataType!=DOUBLE && variable->dataType!=DOUBLEQ) || (function->arrayOfParams[parameterCount].type==STRINGQ && variable->dataType!=STRING && variable->dataType!=STRINGQ) )){
            fprintf(stderr, "ERROR: špatný typ paramteru funkce %s\n", function->idName);
            exit(4);
        }
        enterParams[parameterCount]=*(variable);
        debug("parameter %s\n",enterParams[parameterCount].idName);
    }
    //parametr je číslo nebo nil
    else if(token.tokenType==TOK_int || token.tokenType==TOK_string || token.tokenType==TOK_float || isKeyword(KW_nil)){
        Tdata *paramData = (Tdata *) malloc(sizeof(Tdata));
        if(paramData==NULL){
            exit(99);
        }
        paramData->idName = NULL;
        paramData->value = (char *) malloc(sizeof(char) * 50);
        if(paramData->value==NULL){
            exit(99);
        }

        if(token.tokenType==TOK_int){
            if(function->numOfParam!=-1 && function->arrayOfParams[parameterCount].type!=INT && function->arrayOfParams[parameterCount].type!=INTQ){
                fprintf(stderr, "ERROR: špatný typ paramteru funkce %s\n", function->idName);
                exit(4);
            }
            paramData->dataType = INT;
            snprintf(paramData->value, 50, "%d", token.tokenValue.num);
        }
        else if(token.tokenType==TOK_string){
            if(function->numOfParam!=-1 && function->arrayOfParams[parameterCount].type!=STRING && function->arrayOfParams[parameterCount].type!=STRINGQ){
                fprintf(stderr, "ERROR: špatný typ paramteru funkce %s\n", function->idName);
                exit(4);
            }
            free(paramData->value);
            paramData->value = NULL;
            paramData->dataType = STRING;
            paramData->value = token.tokenValue.string;
        }
        else if(token.tokenType==TOK_float){
            if(function->numOfParam!=-1 && function->arrayOfParams[parameterCount].type!=DOUBLE && function->arrayOfParams[parameterCount].type!=DOUBLEQ){
                fprintf(stderr, "ERROR: špatný typ paramteru funkce %s\n", function->idName);
                exit(4);
            }
            paramData->dataType = DOUBLE;
            snprintf(paramData->value, 50, "%a", token.tokenValue.numFloat);

        }
        else{
            if(function->numOfParam!=-1 && function->arrayOfParams[parameterCount].type!=DOUBLEQ && function->arrayOfParams[parameterCount].type!=INTQ && function->arrayOfParams[parameterCount].type!=STRINGQ){
                fprintf(stderr, "ERROR: špatný typ paramteru funkce %s\n", function->idName);
                exit(4);
            }
        }

    enterParams[parameterCount] = *paramData;

    }

    //parametr není ani číslo, ani string ani identifikátor
    else{
        fprintf(stderr, "ERROR: špatný typ paramteru funkce %s\n", function->idName);
        exit(4);
    }
    getToken(&token, swiftFile);
    enter(swiftFile);
    return 0;
}

#ifndef UNIT_TEST 
int main(){

    initParser(stdin);
    program(stdin);
    genBuiltFunctions();
    killParser();

    return 0;
}
#endif 