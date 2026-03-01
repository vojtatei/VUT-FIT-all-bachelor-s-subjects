/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while(tree != NULL)
  {
    if (tree->key == key)
    {
      //printf("Found %c\n", key);
      *value = tree->value;
      return true;
    }
    else if (key < tree->key)
    {
      tree = tree->left;
    }
    else
    {
      tree = tree->right;
    }
  }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *newnode = (bst_node_t *)malloc(sizeof(bst_node_t));
  newnode->key = key;
  newnode->value = value;
  newnode->left = NULL;
  newnode->right = NULL;

  if (*tree == NULL)
  {
    *tree = newnode;
    return;
  }

  bst_node_t *current = *tree;
  while (0 < 1)
  {
    if (key == current->key)
    {
      current->value = value;
      free(newnode);
      return;
    }
    else if (key < current->key)
    {
      if (current->left == NULL)
      {
        current->left = newnode;
        return;
      }
      current = current->left;
    }
    else
    {
      if(current->right == NULL)
      {
        current->right = newnode;
        return;
      }
      current = current->right;
    }
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (*tree == NULL)
  {
    return;
  }

  bst_node_t *parent = NULL;
  bst_node_t *current = *tree;
  while (current->right != NULL)
  {
    parent = current;
    current = current->right;
  }

  target->key = current->key;
  target->value = current->value;

  if (parent == NULL)
  {
    *tree = current->left;
  }
  else
  {
    parent->right = current->left;
  }
  free(current);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  
  bst_node_t *parent = NULL;
  bst_node_t *current = *tree;

  while (current != NULL && current->key != key)
  {
    parent = current;
    if (key < current->key)
    {
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  
  if (current == NULL)
  {
    return;
  }
  if (current->left == NULL || current->right == NULL)
  {
    bst_node_t *tmp = current->left ? current->left : current->right;

    if (parent == NULL)
    {
      *tree = tmp;
    }
    else
    {
      if (current == parent->left)
      {
        parent->left = tmp;
      }
      else
      {
        parent->right = tmp;
      }
      
    }
    free(current);
  }
  else
  {
    bst_replace_by_rightmost(current, &current->left);
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
    stack_bst_init(&stack);

    while (*tree != NULL || !stack_bst_empty(&stack)) {
        if (*tree != NULL) 
        {
            bst_node_t *current = *tree;
            *tree = (*tree)->left;
            stack_bst_push(&stack, current);
        } else 
        {
            bst_node_t *temp = stack_bst_pop(&stack);
            free(temp);
        }
    }
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while (tree != NULL)
  {
    bst_add_node_to_items(tree, items);
    if(tree->right != NULL)
    {
      stack_bst_push(to_visit, tree->right);
    }
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);

  while (tree != NULL || !stack_bst_empty(&to_visit))
  {
    if (tree != NULL)
    {
      bst_add_node_to_items(tree, items);
      if (tree->right != NULL)
      {
        stack_bst_push(&to_visit, tree->right);
      }
      tree = tree->left;
    }
    else
    {
      tree = stack_bst_pop(&to_visit);
    }
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);
  bst_leftmost_inorder(tree, &to_visit);

  while (!stack_bst_empty(&to_visit))
  {
    tree = stack_bst_pop(&to_visit);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, &to_visit);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
          
        while (tree != NULL)
        {
          stack_bst_push(to_visit, tree);
          stack_bool_push(first_visit, true);
          tree = tree->left;
        }
        
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t to_visit;
  stack_bool_t first_visit;
  stack_bst_init(&to_visit);
  stack_bool_init(&first_visit);
  bst_leftmost_postorder(tree, &to_visit, &first_visit);
  while (!stack_bst_empty(&to_visit))
  {
    tree = stack_bst_pop(&to_visit);
    if (stack_bool_pop(&first_visit))
    {
      stack_bst_push(&to_visit, tree);
      stack_bool_push(&first_visit, false);
      bst_leftmost_postorder(tree->right, &to_visit, &first_visit);
    }
    else
    {
      bst_add_node_to_items(tree, items);
    }
  }
  
}
