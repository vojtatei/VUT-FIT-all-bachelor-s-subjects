/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/

// Pomocná funkce pro vyvážení stromu
void balance_tree(bst_node_t **tree, bst_node_t **nodes, int start, int end) {
    if (start <= end) {
        int mid = (start + end) / 2;
        bst_insert(tree, nodes[mid]->key, nodes[mid]->value);

        balance_tree(tree, nodes, start, mid - 1);
        balance_tree(tree, nodes, mid + 1, end);
    }
}

void letter_count(bst_node_t **tree, char *input) {
    // Procházíme řetězec znak po znaku
    for (int i = 0; input[i] != '\0'; i++) {
        char current_char = input[i];
        char key;

        if (current_char >= 'a' && current_char <= 'z') {
            key = current_char;
        } else if (current_char >= 'A' && current_char <= 'Z') {
            key = current_char + 32; // Konverze na malé písmeno
        } else if (current_char == ' ') {
            key = ' ';
        } else {
            key = '_';
        }

        int value = 1;  // Výchozí hodnota pro nový uzel

        // Vyhledáváme klíč ve stromu
        if (bst_search(*tree, key, &value)) {
            value++;  // Zvýšení hodnoty, pokud klíč existuje
        }
        bst_insert(tree, key, value);  // Vložení nebo aktualizace uzlu
    }
}



/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // Krok 1: Získej všechny uzly ze stromu v inorder
    bst_items_t items;
    items.nodes = NULL;
    items.capacity = 0;
    items.size = 0;
    bst_inorder(*tree, &items);

    // Krok 2: Vytvoř nový strom
    bst_node_t *new_tree = NULL;
    balance_tree(&new_tree, items.nodes, 0, items.size - 1);

    // Krok 3: Uvolni starý strom a přepni ukazatel na nový
    bst_dispose(tree);
    *tree = new_tree;
}


