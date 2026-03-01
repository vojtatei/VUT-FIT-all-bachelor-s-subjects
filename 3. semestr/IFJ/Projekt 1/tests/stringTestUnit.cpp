#include "stringTestUnit.h"
#include <filesystem>

TEST_F(StringTestUnit, InitString) {
    EXPECT_EQ(string->allocSize, STR_LEN);
    EXPECT_EQ(string->len, 0);
    EXPECT_EQ(string->string[0], '\0');

    freeStr(string);
}

TEST_F(StringTestUnit, AddCharStr) {
    EXPECT_EQ(string->len, 0);
    EXPECT_EQ(string->allocSize, 10);
    addCharStr('a', string); // string[0]
    EXPECT_EQ(string->len, 1);
    EXPECT_EQ(string->string[1], '\0');
    EXPECT_EQ(string->string[0], 'a');

    addCharStr('b', string);
    addCharStr('c', string);
    addCharStr('d', string);

    EXPECT_EQ(string->string[3], 'd');
    EXPECT_EQ(string->len, 4);
    EXPECT_EQ(string->allocSize, STR_LEN);

    char str[] = "56789123";
    addStrStr(str, string);

    EXPECT_EQ(string->string[10], '2');
    EXPECT_EQ(string->allocSize, 2*STR_LEN);
    EXPECT_EQ(string->len, 12);

    free(string->string);
    string->string = NULL;
    free(string);

}

TEST_F(StringTestUnit, AddCharStr2) {
    char inputString[] = "Toto je nejaky text v programu jazyka IFJ23 a udelam ho jeste delsim";

    for (unsigned i = 0; i < strlen(inputString); i++) {
        addCharStr(inputString[i], string);
    }

    EXPECT_EQ(string->string[string->len], inputString[strlen(inputString)]);

    free(string->string);
    string->string = NULL;
    free(string);
}

TEST_F(StringTestUnit, delLastCharStr) {
    //delka 6, posledni na 5
    char newStr[] = "Tohle.";
    addStrStr(newStr, string);

    EXPECT_EQ(string->len, strlen(newStr));

    delLastCharStr(string);

    EXPECT_EQ(string->len, strlen(newStr)-1);
    EXPECT_EQ(string->string[string->len-1], 'e');

    free(string->string);
    string->string = NULL;
    free(string);
}

TEST_F(StringTestUnit, AddStrStr) {
    char newStr[] = "Tohle je novy string.";

    addStrStr(newStr, string);
    EXPECT_EQ(string->len, strlen(newStr));
    int result = strcmp(newStr, string->string);
    EXPECT_EQ(result, 0);

    char anotherStr[] = "Dalsi string, ale tento uz je o neco delsi.";
    
    addStrStr(anotherStr, string);
    char expectedStr[100];
    strcpy(expectedStr, newStr);
    strcat(expectedStr, anotherStr);

    EXPECT_EQ(string->len, strlen(newStr)+strlen(anotherStr));
    result = strcmp(string->string, expectedStr);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(string->allocSize, (int)ceil(strlen(expectedStr)/10.0)*10);

    free(string->string);
    string->string = NULL;
    free(string);
}


// Hlavní funkce pro spuštění testů
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}