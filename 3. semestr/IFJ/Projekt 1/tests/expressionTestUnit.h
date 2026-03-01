#define UNIT_TEST
#define DATA_COUNT 12

#include <cstdio>
#include <cmath>
#include <gtest/gtest.h>
#include <cstring>
#include <cstdio>

extern "C" {
#include "../src/expression.c"
#include "../src/symtable.c"
#include "../src/symstack.c"
#include "../src/scanner.c"
#include "../src/string.c"
#include "../src/stack.c"
}

class ExpressionTestUnit : public testing::Test {
protected:
    Tnode *tree = nullptr;
    Tnode *parent = nullptr;
    Tsymstack *symstack = symstackInit();
    Tdata* datas[DATA_COUNT];  // Ukazatelé na dynamicky alokované struktury
    const char* names[DATA_COUNT] = {"f", "j", "l", "d", "e", "a", "g", "h", "i", "x", "b", "y"};


    ExpressionTestUnit() {}

    ~ExpressionTestUnit() override {}

    void SetUp() override {
            double valueIncrement = 0.0;

    for (int i = 0; i < DATA_COUNT; i++) {
        datas[i] = (Tdata *) malloc(sizeof(Tdata));
        if (datas[i] != NULL) {
            datas[i]->idName = strdup(names[i]);
            //datas[i]->isKnown = false;

            // Vytvoření řetězce pro hodnotu - předpokládá se, že je to string
            char valueStr[50];
            sprintf(valueStr, "%f", 42 + valueIncrement); // Příklad, kde se zvyšuje hodnota o 0,5
            datas[i]->value = strdup(valueStr);

            datas[i]->dataType = DOUBLE;
            datas[i]->initType = VAR;
            datas[i]->isInit = true;
            datas[i]->numOfParam = 0;
            datas[i]->arrayOfParams = NULL;

            valueIncrement += 0.5; // Zvýšení hodnoty pro další prvek
        } else {
            // Ošetření chyby alokace paměti
            printf("Nepodařilo se alokovat paměť pro datas[%d]\n", i);
            exit(1);
        }
        symtableInsert(&tree, datas[i]);
    }
    symstackPush(symstack, tree);
}

    void TearDown() override {
        symtableDispose(&tree);

        
    }

    // Zde můžete přidat další pomocné metody pro vaše testy
};

