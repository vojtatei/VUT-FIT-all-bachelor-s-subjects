/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symtable testing
 *
 * @author Monika Záhradníková (xzahra33)
 */

#define UNIT_TEST
#define DATA_COUNT 12
#include "gtest/gtest.h"
#include <filesystem>

extern "C" {
#include "../src/symtable.c"
}

class SymtableTest : public testing::Test
{
    protected:
    Tnode *tree = symtableInit();
    Tnode *bush = symtableInit();

    Tdata* datas[DATA_COUNT];  // Ukazatelé na dynamicky alokované struktury
    const char* names[DATA_COUNT] = {"f", "j", "l", "d", "e", "a", "g", "h", "i", "k", "c", "b"};

    void SetUp() override {
        for (int i = 0; i < DATA_COUNT; i++) {
            datas[i] = (Tdata *) malloc(sizeof(Tdata));
            if (datas[i] != NULL) {
                datas[i]->idName = strdup(names[i]);
            } else {
                // Ošetření chyby alokace paměti
                printf("Nepodařilo se alokovat paměť pro datas[%d]\n", i);
                exit(1);
            }

        symtableInsert(&tree, datas[i]);
        }
    }

    void TearDown() override {
    }
};