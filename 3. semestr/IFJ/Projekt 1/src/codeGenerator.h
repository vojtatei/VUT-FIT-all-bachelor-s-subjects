/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Code Generator header
 *
 * @author Jakub Jeřábek (xjerab28)
 */


#ifndef GENERATOR_H
#define GENERATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include "string.h"
#include "symtable.h"
#include "expression.h"
#include "debug.h"

/**
 * @brief Funkce pro printovani komentaru
 * @param count pocet retezcu v parametru funkce
 * @param char* promenlivy pocet retezcu
*/
void comment(int count, ...);

/**
 * @brief Funkce pro printovani jednotlivych instrukci
 * @param count pocet retezcu v parametrech funkce
 * @param char* promenlivy pocet retezcu
*/
void inst(int count, ...);

void endProgram();

/**
 * @brief Interni Funkce prekladace pro kontrolu, zda promenna muze nabyvat hodnoty nil
*/
void builtCheckNil();

/**
 * @brief Vestavena funkce pro nacteni stringu ze stdin
*/
void builtReadString();

/**
 * @brief Vestavena funkce pro nacteni Int ze stdin
*/
void builtReadInt();

/**
 * @brief Vestavena funkce pro nacteni Double ze stdin
*/
void builtReadDouble();

/**
 * @brief Vestavena funkce pro postupny vypis termu (int a double) na stdout
*/
void builtWrite();

/**
 * @brief Vestavena funkce pro prevod hodnoty int na double
*/
void builtInt2Double();

/**
 * @brief Vestavena funkce pro prevod hodnoty double na int
*/
void builtDouble2Int();

/**
 * @brief Vestavena funkce pro vraceni delky retezce string
*/
void builtLength();

/**
 * @brief Vestavena funkce pro vraceni podretezec zadaneho retezce s
*/
void builtSubString();

/**
 * @brief Vestavena funkce pro vraceni ordinalni hodnoty(ASCII) prvniho znaku v retezci
*/
void builtOrd();

/**
 * @brief Vestavena funkce pro vraceni retezec(char) jehoz ascii hodnota odpovida parametru
*/
void builtChr();

/**
 * @brief Generuje hlavicku IFJ23Code kodu
*/
void generateHeader();

/**
 * @brief Interni funkce generatoru pro vygenerovani pomocne promenne
 * ktera urci, zda promenna muze nabyvat "Nil" ci ne.
*/
void canBeNil(const char* idName, dataTypeT dataType, const char* frame, const char* uniqId);


/**
 * @brief Vygenerovani nove promenne (deklarace)
 * @param data informace o promenne
*/
void generateVariable(Tdata *data);

/**
 * @brief Prirazeni hodnoty do promenne
 * @param data informace o promenne s novym data->value
 * @param expression struktura obsahujici vyraz v postfixu
*/
void assignValue(Tdata *data, expressionStruct* expression);

/**
 * @brief Generovani vyrazu
 * @param expression struktura s postfix vyrazem a informacemi o promennych
*/
void generateExpression(expressionStruct *expression);


/**
 * @brief Vygenerovani hlavicky while
 * @param expression struktura s postfix vyrazem a informacemi o promennych
 * @param labelIndex cislo labelu
*/
void generateWhileHead(expressionStruct *expression, int labelIndex);

/**
 * @brief Vygenerovani konce while
 * @todo param pro labelIndex
*/

/**
 * @brief interni funkce pro interpretaci IFJ23Code pokud je pouzit operator "??"
*/
void doubleQuest();


/**
 * @brief Vygenerovani hlavicky if, pokud obsahuje "let"
 * @param letVariable informace o let promenne
 * @param labelIndex univerzalni oznaceni if navesti
*/
void generateIfLet(Tdata *letVariable, int labelIndex);

/**
 * @brief Vygenerovani if hlavicky
 * @param expression struktura s postfix vyrazem a informacemi o promennych
 * @param labelIndex univerzalni oznaceni if navesti
*/
void generateIf(expressionStruct *expression, int labelIndex);


/**
 * @brief Vygenerovani else hlavicky
 * @param labelIndex univerzalni oznaceni navesti
 * @param letVariable informace o let promenne, pokud byla pouzita v if podmince
*/
void generateElse(int labelIndex, Tdata *letVariable);


/**
 * @brief Vygenerovani konce else
 * @param labelIndex univerzalni oznaceni navesti
*/
void generateEndElse(int labelIndex);

/**
 * @brief interni funkce generatoru pro prevod labelIndex na char* a nasledne jednodussi generovani
 * @param labelIndex univerzalni oznaceni navesti
*/
char* convertIndex(int labelIndex);

/**
 * @brief Vygenerovani konce while
 * @param labelIndex univerzalni oznaceni navesti
*/
void generateWhileEnd(int labelIndex);

/**
 * @brief Inicializace generatoru - vygenerovani hlavicky a pomocnych promennych
*/
void initGenerator();


/**
 * @brief Vygenerovani hlavicky funkce
 * @param function informace o funkci
*/
void generateFunctionHead(Tdata* function);

/**
 * @brief vygenerovani konce funkce - navratu
 * @param expression struktura s postfix vyrazem a informacemi o promennych
*/
void generateFunctionReturn(expressionStruct *expression);

/**
 * @brief Vygenerovani konce funkce
 * @param function informace o funkci
*/
void generateFunctionEnd(Tdata * function);

/**
 * @brief Prirazeni navratove hodnoty z funkce do promenne
 * @param variable informace o promenne
*/
void assignFunctionValue(Tdata *variable);

/**
 * @brief Vygenerovani volani funkce
 * @param function informace o funkci
 * @param params pole parametru volane funkce
 * @param parameterCount pocet parametru
*/
void callFunction(Tdata* function, Tdata* params, int parameterCount);

/**
 * @brief Interni funkce generatoru pro prevod potrebnych znaku na escape sekvence
 * @param input string z tokenu
 * @return prevedeny string obsahujici escape sekvence
*/
char* escapeString(char* input);

#endif