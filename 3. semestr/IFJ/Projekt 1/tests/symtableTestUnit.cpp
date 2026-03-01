/**
 * IFJ Projekt FIT VUT 2023
 *
 * @brief Symtable testing
 *
 * @author Monika Záhradníková (xzahra33)
 */

#include "./symtableTestUnit.h"

TEST_F(SymtableTest, Init) {
    Tnode* root = symtableInit();

    EXPECT_EQ(root, nullptr);
}


TEST_F(SymtableTest, InsertMoreNodes) {

    ASSERT_TRUE(tree != nullptr);

    ASSERT_STREQ(datas[0]->idName, tree->key);
    ASSERT_STREQ(datas[1]->idName, tree->right->key);
    ASSERT_STREQ(datas[2]->idName, tree->right->right->key);
    ASSERT_STREQ(datas[3]->idName, tree->left->key);
    ASSERT_STREQ(datas[4]->idName, tree->left->right->key);
    ASSERT_STREQ(datas[5]->idName, tree->left->left->key);
    ASSERT_STREQ(datas[6]->idName, tree->right->left->key);
    ASSERT_STREQ(datas[7]->idName, tree->right->left->right->key);
    ASSERT_STREQ(datas[8]->idName, tree->right->left->right->right->key);
    ASSERT_STREQ(datas[9]->idName, tree->right->right->left->key);
    ASSERT_STREQ(datas[10]->idName, tree->left->left->right->key);
    ASSERT_STREQ(datas[11]->idName, tree->left->left->right->left->key);

    symtableDispose(&tree);
}


TEST_F(SymtableTest, Search) {
    datas[8]->idName = "i";
    datas[8]->value = "5";

    Tsymsearch s;

    s = symtableSearch(&tree, strdup("f"));
    EXPECT_EQ(s.found, true);

    s = symtableSearch(&tree, strdup("c"));
    EXPECT_EQ(s.found, true);

    s = symtableSearch(&tree, strdup("m"));
    EXPECT_NE(s.found, true);

    s = symtableSearch(&tree, strdup("y"));
    EXPECT_NE(s.found, true);

    s = symtableSearch(&tree, strdup("i"));
    EXPECT_EQ(s.found, true);
    EXPECT_EQ(s.found_data->value, "5");

    symtableDispose(&tree);
}


TEST_F(SymtableTest, BalanceAfterInsert) {
    symtableInsert(&bush, datas[3]);  //d
    symtableBalanceTree(&bush);
    EXPECT_STREQ("d", bush->key);

    symtableInsert(&bush, datas[10]);  //c
    symtableBalanceTree(&bush);
    EXPECT_STREQ("d", bush->key);
    EXPECT_STREQ("c", bush->left->key);

    symtableInsert(&bush, datas[4]);  //e
    symtableBalanceTree(&bush);
    EXPECT_STREQ("d", bush->key);
    EXPECT_STREQ("c", bush->left->key);
    EXPECT_STREQ("e", bush->right->key);

    symtableInsert(&bush, datas[5]);  //a
    symtableBalanceTree(&bush);
    EXPECT_STREQ("d", bush->key);
    EXPECT_STREQ("c", bush->left->key);
    EXPECT_STREQ("e", bush->right->key);
    EXPECT_STREQ("a", bush->left->left->key);

    symtableInsert(&bush, datas[11]);  //b
    symtableBalanceTree(&bush);
    EXPECT_STREQ("c", bush->key);
    EXPECT_STREQ("a", bush->left->key);
    EXPECT_STREQ("b", bush->left->right->key);
    EXPECT_STREQ("d", bush->right->key);
    EXPECT_STREQ("e", bush->right->right->key);

    symtableDispose(&bush);
}


TEST_F(SymtableTest, ComplexBalancing) {
    symtableInsert(&bush, datas[0]);//f
    symtableBalanceTree(&bush);
    EXPECT_STREQ("f", bush->key);

    symtableInsert(&bush, datas[1]);//j
    symtableBalanceTree(&bush);
    EXPECT_STREQ("f", bush->key);
    EXPECT_STREQ("j", bush->right->key);

    symtableInsert(&bush, datas[2]);//l
    symtableBalanceTree(&bush);
    EXPECT_STREQ("j", bush->key);
    EXPECT_STREQ("l", bush->right->key);
    EXPECT_STREQ("f", bush->left->key);

    symtableInsert(&bush, datas[3]);//d
    symtableBalanceTree(&bush);
    EXPECT_STREQ("j", bush->key);
    EXPECT_STREQ("l", bush->right->key);
    EXPECT_STREQ("f", bush->left->key);
    EXPECT_STREQ("d", bush->left->left->key);

    symtableInsert(&bush, datas[4]);//e
    symtableBalanceTree(&bush);
    EXPECT_STREQ("f", bush->key);
    EXPECT_STREQ("j", bush->right->key);
    EXPECT_STREQ("l", bush->right->right->key);
    EXPECT_STREQ("d", bush->left->key);
    EXPECT_STREQ("e", bush->left->right->key);

    symtableInsert(&bush, datas[5]);//a
    symtableBalanceTree(&bush);
    EXPECT_STREQ("f", bush->key);
    EXPECT_STREQ("j", bush->right->key);
    EXPECT_STREQ("l", bush->right->right->key);
    EXPECT_STREQ("d", bush->left->key);
    EXPECT_STREQ("e", bush->left->right->key);
    EXPECT_STREQ("a", bush->left->left->key);

    symtableInsert(&bush, datas[6]);//g
    symtableBalanceTree(&bush);
    EXPECT_STREQ("f", bush->key);
    EXPECT_STREQ("j", bush->right->key);
    EXPECT_STREQ("l", bush->right->right->key);
    EXPECT_STREQ("g", bush->right->left->key);
    EXPECT_STREQ("d", bush->left->key);
    EXPECT_STREQ("e", bush->left->right->key);
    EXPECT_STREQ("a", bush->left->left->key);

    symtableInsert(&bush, datas[7]);//h
    symtableBalanceTree(&bush);
    EXPECT_STREQ("f", bush->key);
    EXPECT_STREQ("j", bush->right->key);
    EXPECT_STREQ("l", bush->right->right->key);
    EXPECT_STREQ("g", bush->right->left->key);
    EXPECT_STREQ("h", bush->right->left->right->key);
    EXPECT_STREQ("d", bush->left->key);
    EXPECT_STREQ("e", bush->left->right->key);
    EXPECT_STREQ("a", bush->left->left->key);

    symtableInsert(&bush, datas[8]);//i
    symtableBalanceTree(&bush);
    EXPECT_STREQ("g", bush->key);
    EXPECT_STREQ("i", bush->right->key);
    EXPECT_STREQ("h", bush->right->left->key);
    EXPECT_STREQ("j", bush->right->right->key);
    EXPECT_STREQ("l", bush->right->right->right->key);
    EXPECT_STREQ("d", bush->left->key);
    EXPECT_STREQ("a", bush->left->left->key);
    EXPECT_STREQ("e", bush->left->right->key);
    EXPECT_STREQ("f", bush->left->right->right->key);

    symtableDispose(&tree);
}


//{"f", "j", "l", "d", "e", "a", "g", "h", "i", "k", "c", "b"};
//  0    1    2    3    4    5    6    7    8    9   10   11
TEST_F(SymtableTest, InsertExistingNode) {

    Tdata* dataA = (Tdata *) malloc(sizeof(Tdata));
    dataA->idName = strdup("a");
    dataA->uniqId = 1;
    Tdata* dataA2 = (Tdata *) malloc(sizeof(Tdata));
    dataA2->idName = strdup("a");
    dataA2->uniqId = 12;

    Tdata* dataD = (Tdata *) malloc(sizeof(Tdata));
    dataD->idName = strdup("d");
    dataD->uniqId = 2;
    Tdata* dataD2 = (Tdata *) malloc(sizeof(Tdata));
    dataD2->idName = strdup("d");
    dataD2->uniqId = 36;

    Tdata* dataL = (Tdata *) malloc(sizeof(Tdata));
    dataL->idName = strdup("l");
    dataL->uniqId = 4;
    Tdata* dataL2 = (Tdata *) malloc(sizeof(Tdata));
    dataL2->idName = strdup("l");
    dataL2->uniqId = 10;

    Tdata* dataZ = (Tdata *) malloc(sizeof(Tdata));
    dataZ->idName = strdup("z");
    dataZ->uniqId = 6;


    symtableInsert(&bush, dataA);//a
    symtableBalanceTree(&bush);
    EXPECT_STREQ("a", bush->key);
    EXPECT_EQ(1, bush->attributes->uniqId);

    symtableInsert(&bush, dataZ);//z
    symtableBalanceTree(&bush);
    EXPECT_STREQ("a", bush->key);
    EXPECT_EQ(1, bush->attributes->uniqId);
    EXPECT_STREQ("z", bush->right->key);

    symtableInsert(&bush, dataA2);//a
    symtableBalanceTree(&bush);
    EXPECT_STREQ("a", bush->key);
    EXPECT_EQ(12, bush->attributes->uniqId);
    EXPECT_STREQ("z", bush->right->key);

    symtableInsert(&bush, dataL);//l
    symtableBalanceTree(&bush);
    EXPECT_STREQ("l", bush->key);
    EXPECT_STREQ("a", bush->left->key);
    EXPECT_STREQ("z", bush->right->key);

    symtableInsert(&bush, dataD);//d
    symtableBalanceTree(&bush);
    EXPECT_STREQ("l", bush->key);
    EXPECT_EQ(4, bush->attributes->uniqId);
    EXPECT_STREQ("a", bush->left->key);
    EXPECT_STREQ("d", bush->left->right->key);
    EXPECT_STREQ("z", bush->right->key);

    symtableInsert(&bush, dataL2);//l
    symtableBalanceTree(&bush);
    EXPECT_STREQ("l", bush->key);
    EXPECT_EQ(10, bush->attributes->uniqId);
    EXPECT_STREQ("a", bush->left->key);
    EXPECT_STREQ("d", bush->left->right->key);
    EXPECT_EQ(2, bush->left->right->attributes->uniqId);
    EXPECT_STREQ("z", bush->right->key);

    symtableInsert(&bush, dataD2);//d
    symtableBalanceTree(&bush);
    EXPECT_STREQ("l", bush->key);
    EXPECT_STREQ("a", bush->left->key);
    EXPECT_STREQ("d", bush->left->right->key);
    EXPECT_EQ(36, bush->left->right->attributes->uniqId);
    EXPECT_STREQ("z", bush->right->key);

    symtableDispose(&bush);
}


TEST_F(SymtableTest, Dispose) {
    symtableDispose(&tree);
    EXPECT_EQ(tree, nullptr);
}


TEST_F(SymtableTest, SearchWithData) {
    Tdata *newData = (Tdata *)malloc(sizeof(Tdata));
    newData->idName = "internet";
    newData->value = "5";
    newData->dataType = INTQ;
    newData->initType = VAR;

    Tsymsearch s;
    symtableInsert(&tree, newData);

    s = symtableSearch(&tree, strdup("internet"));
    EXPECT_EQ(s.found, true);
    EXPECT_STREQ(s.found_data->value, "5");
    EXPECT_EQ(s.found_data->dataType, INTQ);
    EXPECT_EQ(s.found_data->initType, VAR);

    symtableDispose(&tree);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
