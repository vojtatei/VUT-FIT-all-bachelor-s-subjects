/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symstack testing
 *
 * @author Monika Záhradníková (xzahra33)
 */

#define UNIT_TEST
#define DATA_COUNT 12
#include "gtest/gtest.h"
#include <filesystem>

extern "C" {
#include "../src/symstack.c"
#include "../src/symtable.c"
}

class SymstackTest : public testing::Test
{
    protected:
    Tnode *tree = symtableInit();
    Tnode *tree1 = symtableInit();
    Tnode *tree2 = symtableInit();
    Tnode *tree3 = symtableInit();

    Tdata* datas[DATA_COUNT];  // Ukazatelé na dynamicky alokované struktury
    Tdata* datas1[DATA_COUNT];
    Tdata* datas2[DATA_COUNT];
    Tdata* datas3[DATA_COUNT];

    const char* names[DATA_COUNT] = { "f", "j", "l", "d", "e", "a", "g", "h", "i", "k", "c", "b" };
    const char* names1[DATA_COUNT] = { "o", "m", "l", "q", "d", "f", "gr", "z", "er", "ko", "ca", "be" };
    const char* names2[DATA_COUNT] = { "fiha", "jaj", "b", "kri", "elf", "aha", "wer", "fd", "cv", "hk", "cc", "bi" };
    const char* names3[DATA_COUNT] = { "fiha", "jaj", "b", "kri", "elf", "aha", "wer", "fd", "cv", "hk", "cc", "bi" };
    void SetUp() override {
        for (int i = 0; i < DATA_COUNT; i++) {
            datas[i] = (Tdata *) malloc(sizeof(Tdata));
            if (datas[i] != NULL) {
                datas[i]->idName = strdup(names[i]);
                datas[i]->localFrame = false;
            } else {
                // Ošetření chyby alokace paměti
                printf("Nepodařilo se alokovat paměť pro datas[%d]\n", i);
                exit(1);
            }

            symtableInsert(&tree, datas[i]);

        }

        for (int i = 0; i < DATA_COUNT; i++) {
            datas1[i] = (Tdata *) malloc(sizeof(Tdata));
            if (datas1[i] != NULL) {
                datas1[i]->idName = strdup(names1[i]);
                datas1[i]->localFrame = true;
            } else {
                // Ošetření chyby alokace paměti
                printf("Nepodařilo se alokovat paměť pro datas[%d]\n", i);
                exit(1);
            }

            symtableInsert(&tree1, datas1[i]);
        }

        for (int i = 0; i < DATA_COUNT; i++) {
            datas2[i] = (Tdata *) malloc(sizeof(Tdata));
            if (datas2[i] != NULL) {
                datas2[i]->idName = strdup(names2[i]);
                datas2[i]->localFrame = true;
            } else {
                // Ošetření chyby alokace paměti
                printf("Nepodařilo se alokovat paměť pro datas[%d]\n", i);
                exit(1);
            }

            symtableInsert(&tree2, datas2[i]);
        }

        for (int i = 0; i < DATA_COUNT; i++) {
            datas3[i] = (Tdata *) malloc(sizeof(Tdata));
            if (datas3[i] != NULL) {
                datas3[i]->idName = strdup(names3[i]);
                datas3[i]->localFrame = true;
            } else {
                // Ošetření chyby alokace paměti
                printf("Nepodařilo se alokovat paměť pro datas[%d]\n", i);
                exit(1);
            }

            symtableInsert(&tree3, datas3[i]);
        }
    }

    void TearDown() override {

    }
};
