/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symstack implementation
 *
 * @author Monika Záhradníková (xzahra33)
 */

#ifndef SYMSTACK_H
#define SYMSTACK_H

#include "symtable.h"

typedef struct tsymstack {
    Tnode** stackArray;    // pole pre uloženie symtablov
    int topIndex;     // index vrcholu zásobníka
}Tsymstack;


/// @brief funkcia, ktorá inicializuje zásobník tabuliek symbolov
/// @return Tsymstack* ukazateľ na zásobník
Tsymstack* symstackInit();


/// @brief funkcia, ktorá dealokuje zásobník tabuliek symbolov
/// @param stack ukazateľ na zásobník
void symstackDispose(Tsymstack *stack);


/// @brief funkcia, ktorá uloží novú tabuľku symbolov na vrchol zásobníka
/// @param stack ukazateľ na zásobnik
/// @param tree tabuľka symbolov
void symstackPush(Tsymstack *stack, Tnode *tree);


/// @brief funkcia, ktorá odstráni tabuľku symbolov z vrcholu zásobníka
/// @param stack ukazateľ na zásobnik
void symstackPop(Tsymstack *stack);


/// @brief funkcia, ktorá vráti tabuľku symbolov na vrchole zásobníka
/// @param stack ukazateľ na zásobnik
/// @return ukazateľ na tabuľku symbolov na vrchole zásobníka
Tnode* symstackTop(Tsymstack *stack);



/// @brief funkcia, ktorá bude prehľadávať celý zásobník smerom od vrcholu, rámec po rámci,
/// kým v niektorom z rámcov (v strome) nenájde uzol s kľúčom idName
/// @param stack ukazateľ na zásobnik
/// @param idName identifikátor, ktorý má hľadať
/// @return ukazateľ na informácie o identifikátore, inak NULL
Tdata* symstackSearchiId(Tsymstack *stack, char *idName);


#endif //SYMSTACK_H