/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symstack testing
 *
 * @author Monika Záhradníková (xzahra33)
 */

#include "./symstackTestUnit.h"

TEST_F(SymstackTest, Init) {
    Tsymstack *stack = symstackInit();
    ASSERT_NE(stack, nullptr);
    EXPECT_EQ(stack->stackArray, nullptr);
    EXPECT_EQ(stack->topIndex, -1);

    symstackDispose(stack);
}

TEST_F(SymstackTest, Push) {

Tsymstack *stack = symstackInit();
    symstackPush(stack, tree);
    EXPECT_EQ(stack->topIndex, 0);
    ASSERT_NE(stack->stackArray, nullptr);

    EXPECT_EQ((stack->stackArray[0]), tree);
    EXPECT_EQ((stack->stackArray[1]), nullptr);
    EXPECT_EQ((stack->stackArray[2]), nullptr);

    symstackPush(stack, tree1);
    EXPECT_EQ(stack->topIndex, 1);

    EXPECT_EQ((stack->stackArray[0]), tree);
    EXPECT_EQ((stack->stackArray[1]), tree1);
    EXPECT_EQ((stack->stackArray[2]), nullptr);

    symstackPush(stack, tree2);
    EXPECT_EQ(stack->topIndex, 2);

    EXPECT_EQ((stack->stackArray[0]), tree);
    EXPECT_EQ((stack->stackArray[1]), tree1);
    EXPECT_EQ((stack->stackArray[2]), tree2);

    symstackDispose(stack);
}

TEST_F(SymstackTest, PushEmptyTrees) {
    Tsymstack *stack = symstackInit();
    Tnode *treesko = symtableInit();
    Tnode *treesko2 = symtableInit();

    symstackPush(stack, treesko);
    EXPECT_EQ(stack->topIndex, 0);
    EXPECT_EQ(stack->stackArray[0], nullptr);
    EXPECT_EQ(stack->stackArray[1], nullptr);
    EXPECT_EQ(stack->stackArray[2], nullptr);

    symstackPush(stack, treesko2);
    EXPECT_EQ(stack->topIndex, 1);
    EXPECT_EQ(stack->stackArray[0], nullptr);
    EXPECT_EQ(stack->stackArray[1], nullptr);
    EXPECT_EQ(stack->stackArray[2], nullptr);

    symstackPop(stack);
    EXPECT_EQ(stack->topIndex, 0);
    EXPECT_EQ(stack->stackArray[0], nullptr);
    EXPECT_EQ(stack->stackArray[1], nullptr);
    EXPECT_EQ(stack->stackArray[2], nullptr);

    symstackPop(stack);
    EXPECT_EQ(stack->topIndex, -1);
    EXPECT_EQ(stack->stackArray[0], nullptr);
    EXPECT_EQ(stack->stackArray[1], nullptr);
    EXPECT_EQ(stack->stackArray[2], nullptr);
}


TEST_F(SymstackTest, Pop) {
    Tsymstack *stack = symstackInit();
    symstackPush(stack, tree);
    symstackPush(stack, tree1);
    symstackPush(stack, tree2);

    symstackPop(stack);

    EXPECT_EQ(stack->stackArray[2], nullptr);
    EXPECT_EQ(stack->topIndex, 1);
    EXPECT_EQ((stack->stackArray[1]), tree1);
    EXPECT_EQ((stack->stackArray[0]), tree);

    symstackPop(stack);

    EXPECT_EQ((stack->stackArray[1]), nullptr);
    EXPECT_EQ(stack->topIndex, 0);
    EXPECT_EQ(stack->stackArray[2], nullptr);
    EXPECT_EQ((stack->stackArray[0]), tree);

    symstackPop(stack);

    EXPECT_EQ((stack->stackArray[0]), nullptr);
    EXPECT_EQ(stack->topIndex, -1);
    EXPECT_EQ((stack->stackArray[1]), nullptr);
    EXPECT_EQ(stack->stackArray[2], nullptr);

    symstackDispose(stack);
}

TEST_F(SymstackTest, Top) {
    Tsymstack *stack = symstackInit();
    Tnode* top;

    symstackPush(stack, tree);
    top = symstackTop(stack);
    EXPECT_EQ(top, stack->stackArray[0]);

    symstackPush(stack, tree1);
    top = symstackTop(stack);
    EXPECT_EQ(top, stack->stackArray[1]);

    symstackPop(stack);
    top = symstackTop(stack);
    EXPECT_EQ(top, stack->stackArray[0]);

    symstackDispose(stack);
}


TEST_F(SymstackTest, ComplexTests2) {
    Tsymstack *stack = symstackInit();
    Tnode *treesko = symtableInit();
    Tnode *treesko2 = symtableInit();
    Tnode *treesko3 = symtableInit();

    Tdata *data1 = (Tdata *) malloc(sizeof(Tdata));
    data1->idName = strdup("Promenna");
    data1->dataType = INT;
    data1->value = strdup("142");

    symtableInsert(&treesko, data1);

    symstackPush(stack, treesko);
    EXPECT_EQ(stack->topIndex, 0);
    EXPECT_EQ(stack->stackArray[1], nullptr);
    EXPECT_EQ(stack->stackArray[2], nullptr);

    symstackPush(stack, treesko2);
    EXPECT_EQ(stack->topIndex, 1);
    EXPECT_EQ(stack->stackArray[2], nullptr);

    bool pushNewTree = 0;
    Tnode* currentTree = symstackTop(stack);

    if (currentTree == NULL) {
        pushNewTree = 1;
    }
    EXPECT_EQ(currentTree, treesko2); //su rovnake
    EXPECT_EQ(currentTree, nullptr); //prazdny strom

    Tsymsearch foundSearch = symtableSearch(&currentTree, "Promenna");
    EXPECT_EQ(foundSearch.found, 0);


    Tdata *data2 = (Tdata *) malloc(sizeof(Tdata));
    data2->idName = strdup("Promenna");
    data2->dataType = DOUBLE;
    data2->value = strdup("10.5");

    symtableInsert(&currentTree, data2);

    if (pushNewTree == 1) {
        symstackPush(stack, currentTree);
        pushNewTree = 0;
    }

    EXPECT_NE(stack->stackArray[stack->topIndex], nullptr);
    EXPECT_NE(currentTree, nullptr); //nie je prazdny strom

    symstackPush(stack, treesko3);

    Tdata *found = symstackSearchiId(stack, "Promenna");

    EXPECT_NE(found, nullptr);
    EXPECT_STREQ(found->idName, "Promenna");
    EXPECT_STREQ(found->value, "10.5");
    EXPECT_EQ(found->dataType, DOUBLE);
}

TEST_F(SymstackTest, Search) {
    Tsymstack *stack = symstackInit();
    Tdata* data = nullptr;
    char *idName;

    symstackPush(stack, tree);
    symstackPush(stack, tree1);
    symstackPush(stack, tree2);

    idName = strdup("j");      //tree
    data = symstackSearchiId(stack, idName);
    EXPECT_EQ(*data->idName, *idName);
    EXPECT_EQ(data->localFrame, false);

    idName = strdup("er");      //tree1
    data = symstackSearchiId(stack, idName);
    EXPECT_EQ(*data->idName, *idName);
    EXPECT_EQ(data->localFrame, true);

    idName = strdup("fiha");      //tree2
    data = symstackSearchiId(stack, idName);
    EXPECT_EQ(*data->idName, *idName);
    EXPECT_EQ(data->localFrame, true);

    idName = strdup("b");      //tree GF, tree2 LF
    data = symstackSearchiId(stack, idName);
    EXPECT_EQ(*data->idName, *idName);
    EXPECT_EQ(data->localFrame, true);

    idName = strdup("x");      // neexistuje
    data = symstackSearchiId(stack, idName);
    EXPECT_EQ(data, nullptr);
}

TEST_F(SymstackTest, Dispose) {
    Tsymstack *stack = symstackInit();
    symstackPush(stack, tree);
    symstackPush(stack, tree1);
    symstackPush(stack, tree2);

    EXPECT_EQ(stack->topIndex, 2);
    EXPECT_NE(stack->stackArray, nullptr);

    symstackDispose(stack);

    // keď je jeden strom v stacku
    Tsymstack *stack2 = symstackInit();
    symstackPush(stack2, tree3);
    symstackDispose(stack2);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}