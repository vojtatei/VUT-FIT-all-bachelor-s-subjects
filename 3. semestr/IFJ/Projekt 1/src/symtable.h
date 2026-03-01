/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symtable implementation - AVL tree
 *
 * @author Monika Záhradníková (xzahra33)
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum typedata {
    INT,
    INTQ,
    DOUBLE,
    DOUBLEQ,
    STRING,
    STRINGQ,
    VOID,
} dataTypeT;


typedef enum typeinit {
    VAR,
    LET,
    FUNC,
} initTypeT;


//Struktura pre ulozenie parametrov funkcie
typedef struct tparam{
    char* name_par;
    char* id_par;
    dataTypeT type;
}Tparam;


typedef struct tdata {
    char* idName;    // názov identifikátora
    int uniqId;     // unikátne číslo identifikátora
    char* value;    // hodnota identifikátora
    bool isInit;     // inicializovana/neinicializovana promenna
    bool localFrame; // 0 - GF, 1 - LF
    dataTypeT dataType;    // datový typ
    initTypeT initType;    // typ inicializácie (premenná, konštanta, funkcia, parameter)
    int numOfParam;    // počet parametrov funkcie
    Tparam* arrayOfParams;    // pole parametrov funkcie (treba alokovať pamäť)
} Tdata;


// Štruktúra pre uzol AVL stromu
typedef struct tnode {
    Tdata* attributes;    // vlastnosti daného identifikátora (aj s kľúčom uzlu)
    char* key;    // kľúč
    int balance;    //váha stromu
    struct tnode* left;    // ľavý podstrom
    struct tnode* right;    // pravý podstrom
} Tnode;


//Struktura pre ulozenie vystupu funkcie symtableSearch
typedef struct typesymsearch {
    bool found;             // nasiel sa uzol (true), nenasiel sa (false)
    Tdata* found_data;      // tu sa ulozia data (atributy) najdeneho uzla
    Tnode* found_node;     // tu sa ulozi odkaz na najdeny uzol
}Tsymsearch;


typedef struct nodeArray {
    Tnode **nodes;
    int capacity;
    int size;
}TnodeArray;

/*
 * FUNKCIE PRE PRÁCU SO SYMTABLE
 */

/// @brief funkcia, ktorá inicializuje tabuľku symbolov
/// @return ukazateľ na tabuľku symbolov
Tnode* symtableInit();


/// @brief funkcia, ktorá v tabuľke symbolov hľadá daný identifikátor
/// @param tree tabuľka symbolov
/// @param idName identifikátor
/// @return štruktúru Tsymsearch, ktorá obsahuje informácie o nájdenom identifikátore
Tsymsearch symtableSearch(Tnode **tree, char *idName);


/// @brief funkcia, ktorá vloží nový identifikátor do tabuľky symbolov alebo
/// aktualizuje Tdata identifikátora, ktorý sa už nachádza v tabuľke symbolov
/// @param tree tabuľka symbolov
/// @param data dáta, ktoré sa majú vložiť do stromu (obsahujú aj názov identifikátora)
void symtableInsert(Tnode **tree, Tdata *data);


/// @brief funkcia, ktorá dealokuje tabuľku symbolov
/// @param ukazateľ na tabuľku symbolov
void symtableDispose(Tnode **tree);


/// @brief funkcia, ktorá vybalancuje strom (tabuľku symbolov)
/// @param ukazateľ na tabuľku symbolov
void symtableBalanceTree(Tnode **tree);


// pomocné funkcie - NEPOUŽÍVAŤ!
int compareStrings(const char *str1,const char *str2);
int getHeight(Tnode *node);
int getBalanceFactor(Tnode *node);
void setBalanceFactor(Tnode **tree);
bool hasUnbalancedNode(Tnode** tree);
void addNodeToArray(Tnode* node, TnodeArray *array);
void inOrderToArray(Tnode** tree, TnodeArray *array);


#endif //SYMTABLE_H
