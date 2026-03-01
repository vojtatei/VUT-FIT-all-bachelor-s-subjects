/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symtable implementation - AVL tree
 *
 * @author Monika Záhradníková (xzahra33)
 */

#include "symtable.h"

// inicializuje novy strom a vrati ukazatel na koren stromu
Tnode* symtableInit() {
   Tnode *root = NULL;

    return root;
}


// porovnava retazce po znak po znaku a vracia ciselnu hodnotu na zaklade vysledku
int compareStrings(const char *str1, const char *str2) {
    int result = strcmp(str1, str2);
    // result < 0  ... str1 < str2
    // result == 0 ... str1 == str2
    // result > 0  ... str1 > str2
    return result;
}

// rekurzivne hlada dany uzol podla parametru idName
// vrati strukturu Tsymsearch, ktora obsahuje
// bool found - ci sa uzol nasiel
// found_data - atributy, ktore ma najdeny uzol
// found_node - odkaz na najdeny uzol (potrebne pre balanc)
Tsymsearch symtableSearch(Tnode **tree, char *idName) {
    Tsymsearch ret;

    if ((*tree) == NULL) {
        ret.found = false;
        return ret;
    }
    // porovná kľúče
    int cmp = compareStrings(idName, (*tree)->key);
    // ak sa rovnajú
    if (cmp == 0) {
        ret.found = true;
        ret.found_data = (*tree)->attributes;
        ret.found_node = *tree;
        return ret;
        }
    // ak je hľadaný kľúč menší ako aktuálny kľúč uzlu
    else if (cmp < 0) {
        return symtableSearch(&((*tree)->left), idName);

    // ak je hľadaný kľúč väčší ako aktuálny kľúč uzlu
    } else {
        return symtableSearch(&((*tree)->right), idName);
    }
}


// rekurzivne vlozi uzol na dane miesto (porovnava dva retazce: data->idName a key uzla)
void symtableInsert(Tnode **tree, Tdata *data) {

    if ((*tree) == NULL) {
        // našli sme miesto, kde máme vložiť nový uzol
        (*tree) = (Tnode *) malloc(sizeof(Tnode));
        if ((*tree) != NULL) {
            (*tree)->key = strdup(data->idName);
            (*tree)->attributes = data;
            (*tree)->right = NULL;
            (*tree)->left = NULL;
        }
    } else {
        int cmp = compareStrings(data->idName, (*tree)->key);

        // ak je hľadaný kľúč menší ako aktuálny kľúč uzlu
        if (cmp < 0) {
            symtableInsert(&((*tree)->left), data);

        // ak je hľadaný kľúč väčší ako aktuálny kľúč uzlu
        } else if (cmp > 0) {
            symtableInsert(&((*tree)->right), data);

        // ak sa kľúče rovnajú
        } else {
            // uzol v strome existuje, prepíšu sa data uzlu
            (*tree)->attributes = data;
        }
    }
}


// Pomocná funkcia, ktorá vypočíta výšku uzla node
int getHeight(Tnode *node){
    if (node == NULL) {
        return 0;
    }
    else {
        int left_subtree_height = getHeight(node->left);
        int right_subtree_height = getHeight(node->right);

        if (left_subtree_height > right_subtree_height) {
            return (left_subtree_height + 1);
        }
        else {
            return (right_subtree_height + 1);
        }
    }
}


// Pomocná funkcia, ktorá vypočíta balanc uzla node
int getBalanceFactor(Tnode *node) {
    if (node == NULL) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}


// Pomocná funkcia, ktorá zapíše (obnoví) balanc uzlov celého stromu
void setBalanceFactor(Tnode **tree) {
    if ((*tree) != NULL) {
        (*tree)->balance = getBalanceFactor((*tree));

        setBalanceFactor(&(*tree)->left);
        setBalanceFactor(&(*tree)->right);
    }
}

// Pomocná funkcia, ktorá zistí, či sa v strome nachádza nevyvážený uzol
bool hasUnbalancedNode(Tnode** tree) {
    if ((*tree) == NULL) {
        return false;
    }

    if ((*tree)->balance < -1 || (*tree)->balance > 1) {
        return true;
    }

    return hasUnbalancedNode(&((*tree)->left)) || hasUnbalancedNode(&((*tree)->right));
}


//Pomocná funkcia, ktorá vloží uzol do poľa uzlov a rozšíri pamäť, pokiaľ je to potrebné
void addNodeToArray(Tnode* node, TnodeArray *array){
    if (array->capacity < array->size+1){
        array->capacity = array->capacity * 2 + 8;
        array->nodes = (Tnode**)realloc(array->nodes, array->capacity * (sizeof(Tnode*)));

        if (array->nodes == NULL) {
            printf("chyba realokacie\n");
        }
    }
    array->nodes[array->size] = node;
    array->size++;
}

// Pomocná funkcia, ktorá prechádza strom a ukladá všetky uzly do poľa uzlov
void inOrderToArray(Tnode** tree, TnodeArray *array){
    if ((*tree) != NULL){
        if ((*tree)->left != NULL) {
            inOrderToArray(&((*tree)->left), array);
        }
        addNodeToArray((*tree), array);
        if ((*tree)->right != NULL) {
            inOrderToArray(&((*tree)->right), array);
        }
    }
}

// Pomocná funkcia, ktorá z poľa uzlov vyskladá nový vyvážený strom
void createBalanceTree(Tnode** tree, TnodeArray *array, int start, int end) {
    if (start > end) {
        *tree = NULL;
        return;
    }

    int mid = (start + end) / 2;

    *tree = array->nodes[mid];
    createBalanceTree(&((*tree)->left), array, start, mid-1);
    createBalanceTree(&((*tree)->right), array, mid+1, end);
}


// funkcia vybalancuje strom tree, pokiaľ v ňom nájde nevyvážený uzol
// v opačnom prípade sa strom tree nijak nezmení
// NUTNÉ VOLAŤ PO KAŽDOM VLOŽENÍ NOVÉHO UZLU !!!
void symtableBalanceTree(Tnode **tree) {

    TnodeArray *array = (TnodeArray*)malloc(sizeof(Tnode));
    array->size = 0;
    array->capacity = 0;
    array->nodes = NULL;

    setBalanceFactor(tree);

    bool isUnbalanced = hasUnbalancedNode(tree);

    if (isUnbalanced){
        inOrderToArray(tree, array);
        int count = array->size;

        createBalanceTree(tree, array, 0, count-1);

        free(array->nodes);
        free(array);
    }
}

// dealokuje strom
// dealokovat data
void symtableDispose(Tnode **tree) {
    if ((*tree) != NULL ){
        if ((*tree)->left == NULL && (*tree)->right == NULL){
            free((*tree)->attributes);  //Uvoľnenie pamäte pre Tdata
            free((*tree)->key);  // Uvoľnenie pamäte pre kľúč.
        }
        else if ((*tree)->left == NULL){
            symtableDispose(&((*tree)->right));
        }
        else if ((*tree)->right == NULL){
            symtableDispose(&((*tree)->left));
        }
        else {
            symtableDispose(&((*tree)->left));
            symtableDispose(&((*tree)->right));
        }
        free(*tree);
        *tree = NULL;
        tree = NULL;
    }
}
