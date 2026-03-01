#include "scannerTestUnit.h"
#include <filesystem>

TEST_F(ScannerTestUnit, TestProcessInt) {
    Token token;
    char input = '9';
    addCharStr(input, string);

    input = '5';
    addCharStr(input, string);

    processInt(string, &token);
    
    // Očekáváme, že se jedná o token typu NUMBER a hodnotu 123
    EXPECT_EQ(token.tokenType, TOK_int);
    EXPECT_EQ(token.tokenValue.num, 95);

}

TEST_F(ScannerTestUnit, TestProcessFloat) {
    Token token;

    char input[] = "9.5453";
    addStrStr(input, string);
    processFloat(string, &token);
    
    EXPECT_EQ(token.tokenType, TOK_float);
    EXPECT_EQ(token.tokenValue.numFloat, 9.5453);


}

TEST_F(ScannerTestUnit, TestProcessFloatE) {
    Token token;
    char input[] = "101.e+125";
    addStrStr(input, string);

    processFloat(string, &token);

    EXPECT_EQ(token.tokenType, TOK_float);
    EXPECT_EQ(token.tokenValue.numFloat, 101.e+125);
}

TEST_F(ScannerTestUnit, TestClearStr) {
    char input[] = "while";
    addStrStr(input, string);

    EXPECT_EQ(string->len, strlen(input));
    EXPECT_EQ(string->allocSize, STR_LEN);

    clearStr(string);

    EXPECT_EQ(string->len, 0);

    freeStr(string);
}

TEST_F(ScannerTestUnit, TestProccesId) {
    Token token;
    char input[] = "while";
    FILE *swiftFile = NULL;
    addStrStr(input, string);

    int response = processId(string, &token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_keyword);
    EXPECT_EQ(token.tokenValue.keyword, KW_while);
    EXPECT_EQ(response, 0);

    string = (String *)malloc(sizeof(String));
    initStr(string);

    char anotherInput[] = "identifier";
    addStrStr(anotherInput, string);
    
    EXPECT_EQ(string->len, strlen(anotherInput));
    EXPECT_EQ(string->allocSize, 2*STR_LEN);
    response = processId(string, &token, swiftFile);

    EXPECT_EQ(response, 0);
    EXPECT_EQ(token.tokenType, TOK_identifier);
    EXPECT_NE(strlen(token.tokenValue.string), 0);

    free(token.tokenValue.string);
}


// Hlavní funkce pro spuštění testů
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}