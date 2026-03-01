/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief parser header
 *
 * @author Doubravka Šimůnková (xsimun05), Jakub Jeřábek (xjerab28)
 */

#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>

FILE *swiftFile = NULL;

typedef struct parser {
    int labelIndex;
    int uniqId;
    bool inFunction;
    int inWhile;
    int inIf;
    bool pushNewTree;
    int currentWhile;
    int *whileDefinitionsCounter;
    Tdata *whileDefinitions;
} parser_t;

parser_t *parser;
Token token;
Tnode* tree;
Tsymstack* stack;

/// @brief pomocná funkce pro určení zda aktuální token je nějaké klíčové slovo
/// @param keyword klíčové slovo, které chceme
/// @return true pokud aktuální token je požadované klíčové slovo
bool isKeyword(Keyword keyword);

/// @brief inicializuje strukturu parseru
/// @param swiftFile 
void initParser(FILE *swiftFile);

/// @brief vrátí strukturu parseru do původního stavu
void killParser();

/// @brief přidá do tabulky symbolů build-in funkce
void buildInFunctions();

/// @brief vygeneruje deklarace všech build-in funkcí
void genBuiltFunctions();

/// @brief první průchod programem, uloží deklarace funkcí do tabulky symbolů
///a najde a uloží do pole všechny proměnné deklarované uvnitř while cyklu
/// @param swiftFile 
/// @return 0 pokud vše proběhlo v pořádku 
int firstPassage(FILE *swiftFile);

/// @brief cílem je dojít na konec funkce - počítá složené závorky
/// @param swiftFile 
/// @return 0 pokud vše proběhlo v pořádku 
int goThroughFunction(FILE *swiftFile);

/// @brief hlavní funkce, jako jediná se volá explicitně, kontroluje správnost programu
/// @param swiftFile 
/// @return 
int program(FILE *swiftFile);

/// @brief zahodní všechny EOL tokeny
/// @param swiftFile 
/// @return 0 pokud vše proběhlo v pořádku
int enter(FILE *swiftFile);

/// @brief hlavní tělo programu, kontroluje správnoust sekvence příkazů a deklarací funkcí, může být prázdné
/// @param swiftFile 
/// @return 0 pokud vše proběhlo v pořádku 
int sequenceOrFunction(FILE *swiftFile);

/// @brief zjistí typ proměnné a popř. jaká jí byla přiřazená hodnota
/// @param swiftFile 
/// @param data struktura proměnné jejíž typ a hodnotu funkce zjišťuje, sem se uloží
/// @param result sem se uloží postfixový výraz, pokud se má do proměnné uložit výraz, jinak NULL
/// @return 0 pokud vše proběhlo v pořádku 
int determineType(FILE *swiftFile, Tdata* data, expressionStruct* result);

/// @brief zjistí návratový typ funkce
/// @param swiftFile 
/// @param data struktura funkce jejíž návratový typ se zjišťuje, sem se uloží
/// @return 0 pokud vše proběhlo v pořádku 
int returnType(FILE *swiftFile, Tdata* data);

/// @brief zjistí s kolika a jakými parametry je funkce deklarovaná
/// @param swiftFile 
/// @param data struktura funkce jejíž parametry se zjišťují, sem se uloží
/// @return 0 pokud vše proběhlo v pořádku 
int listOfParams(FILE *swiftFile, Tdata* data);

/// @brief pomocná funkce pro zjištění s kolika a jakými parametry je funkce deklarovaná, od 2. výš
/// @param swiftFile 
/// @param data struktura funkce jejíž parametry se zjišťují, sem se uloží
/// @return 0 pokud vše proběhlo v pořádku 
int parameters(FILE *swiftFile, Tdata* data);

/// @brief zjistí jestli je parametr při volání funkce správně napsán a uloží ho
/// @param swiftFile 
/// @param data struktura funkce jejíž parametr se zjišťuje, sem se uloží
/// @return 0 pokud vše proběhlo v pořádku 
int parameter(FILE *swiftFile, Tdata* data);

/// @brief kontroluje sekvenci příkazů 
///(deklarace proměnné, přiřazení do proměnné, volání funkce, if podmínka, while cyklus, return)
/// @param swiftFile 
/// @param IsReturn true pokud se v kažné větvi sekvence příkazů nachází return
/// @param functionType typ funkce jejíž tělo příkazy tvoří, pokud se nacházíme ve funkci
/// @return 0 pokud vše proběhlo v pořádku 
int sequence(FILE *swiftFile, bool* IsReturn, dataTypeT functionType);

/// @brief pravdivostní podmínka v if statementu (výraz/if let)
/// @param swiftFile 
/// @param currentLabel
/// @param letVariable proměnná nacházející se v if let statementu, jinak NULL
/// @return 0 pokud vše proběhlo v pořádku 
int statement(FILE *swiftFile, int currentLabel, Tdata **letVariable);

/// @brief zjistí hodnotu, která se přiřazuje do přoměnné
/// @param swiftFile 
/// @param data struktura proměnné jejíž typ a hodnotu funkce zjišťuje, sem se uloží
/// @param result sem se uloží postfixový výraz, pokud se má do proměnné uložit výraz, jinak NULL
/// @param declaring true pokud proměnná ještě není deklarovaná
/// @return 0 pokud vše proběhlo v pořádku 
int valueToVariable(FILE *swiftFile, Tdata* data, expressionStruct* result, bool declaring);

/// @brief zjistí datový typ
/// @param swiftFile 
/// @param dataType sem se uloží typ
/// @return 0 pokud vše proběhlo v pořádku 
int type(FILE *swiftFile, dataTypeT* dataType);

/// @brief zjistí zda je funkce správně volaná
/// @param swiftFile 
/// @param function funkce, která je volaná
/// @return 0 pokud vše proběhlo v pořádku 
int callingFunction(FILE *swiftFile, Tdata* function);

/// @brief zkontroluje zda je funkce volaná se správnými parametry
/// @param swiftFile 
/// @param function funkce, která je volaná
/// @return 0 pokud vše proběhlo v pořádku 
int listOfEnterParams(FILE *swiftFile, Tdata* function);

/// @brief pomocná funkce pro kontrolu zda je funkce volaná se správnými parametry, od 2. výš
/// @param swiftFile 
/// @param function funkce, která je volaná
/// @param parameterCount kolik parametrů je již zpracováno
/// @param enterParams pole struktrur pro uložení inforamací o parametrech
/// @return 0 pokud vše proběhlo v pořádku 
int enterParameters(FILE *swiftFile, Tdata* function, int parameterCount, Tdata* enterParams);

/// @brief 
/// @param swiftFile zkontroluje zda je konktérní parametr volané funkce správný
/// @param function funkce, která je volaná
/// @param parameterCount kolik parametrů je již zpracováno
/// @param enterParams pole struktrur pro uložení inforamací o parametrech
/// @return 0 pokud vše proběhlo v pořádku 
int enterParameter(FILE *swiftFile, Tdata* function, int parameterCount, Tdata* enterParams);

#endif