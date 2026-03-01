#include "expressionTestUnit.h"
#include <filesystem>
#include <fstream>

FILE* createTestFileWithExpression(const char* expression) {
    FILE* file = tmpfile();
    fputs(expression, file);
    rewind(file);
    return file;
}

TEST_F(ExpressionTestUnit, CombinedArithmeticAndComparisonOperation4) {
    const char* expression = " j < 42";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    expressionStruct result = createExpression(testFile, symstack, &token);

    // Očekáváme, že výsledek bude typu bool kvůli operátoru porovnání
    EXPECT_EQ(TYPE_BOOL, result.type);

    fclose(testFile);
    free(result.postFix); // Uvolnění alokované paměti pro postfixové pole
}



TEST_F(ExpressionTestUnit, HandlesComplexOperationWithBrackets) {
    
    const char* expression = "(5 + 3) * 2";
    FILE* testFile = createTestFileWithExpression(expression);

   unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    expressionStruct result = createExpression(testFile, symstack, &token);

    // Očekáváme, že postfixové pole obsahuje určitý počet tokenů
    const int expectedTokenCount = 5;
    EXPECT_EQ(expectedTokenCount, result.postFixLen);

    // Kontrola jednotlivých tokenů
    // Předpokládáme, že tokeny jsou ve formátu: 5 3 + 2 *
    EXPECT_EQ(TOK_int, result.postFix[0].tokenType);
    EXPECT_EQ(5, result.postFix[0].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[1].tokenType);
    EXPECT_EQ(3, result.postFix[1].tokenValue.num);

    EXPECT_EQ(TOK_add, result.postFix[2].tokenType);

    EXPECT_EQ(TOK_int, result.postFix[3].tokenType);
    EXPECT_EQ(2, result.postFix[3].tokenValue.num);

    EXPECT_EQ(TOK_mul, result.postFix[4].tokenType);

    fclose(testFile); // Uzavření souboru

    // Uvolnění případně alokované paměti
    free(result.postFix);
}
/*
TEST_F(ExpressionTestUnit, HandlesSimpleOperationWithIdentifiers) {
    const char* expression = "x + 3.0 - y";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    // Předpokládáme, že x a y jsou inicializované identifikátory v symstack
    // Zde je potřeba nastavit hodnoty x a y v symstack
    // Například x může mít hodnotu 5 a y hodnotu 2

    expressionStruct result = createExpression(testFile, symstack, &token);

    fclose(testFile);

    // Očekáváme, že postfixové pole obsahuje určitý počet tokenů
    const int expectedTokenCount = 5;
    EXPECT_EQ(expectedTokenCount, result.postFixLen);

    // Kontrola jednotlivých tokenů
    // Předpokládáme, že tokeny jsou ve formátu: x 3 + y -
    EXPECT_EQ(TOK_identifier, result.postFix[0].tokenType);
    EXPECT_STREQ("x", result.postFix[0].tokenValue.string);

    EXPECT_EQ(TOK_float, result.postFix[1].tokenType);
    EXPECT_EQ(3.0, result.postFix[1].tokenValue.numFloat);

    EXPECT_EQ(TOK_add, result.postFix[2].tokenType);

    EXPECT_EQ(TOK_identifier, result.postFix[3].tokenType);
    EXPECT_STREQ("y", result.postFix[3].tokenValue.string);

    EXPECT_EQ(TOK_sub, result.postFix[4].tokenType);

    // Uvolnění případně alokované paměti
    free(result.postFix);
}
*/

TEST_F(ExpressionTestUnit, CombinedArithmeticAndComparisonOperation) {
    const char* expression = "(3 + 2) == 5";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    expressionStruct result = createExpression(testFile, symstack, &token);

    // Očekáváme, že výsledek bude typu bool kvůli operátoru porovnání
    EXPECT_EQ(TYPE_BOOL, result.type);

    fclose(testFile);
    free(result.postFix); // Uvolnění alokované paměti pro postfixové pole
}

TEST_F(ExpressionTestUnit, CombinedArithmeticAndComparisonOperation2) {
    const char* expression = "(3 + 2) == 5";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    expressionStruct result = createExpression(testFile, symstack, &token);

    // Očekáváme, že výsledek bude typu bool kvůli operátoru porovnání
    EXPECT_EQ(TYPE_BOOL, result.type);

    fclose(testFile);
    free(result.postFix); // Uvolnění alokované paměti pro postfixové pole
}

TEST_F(ExpressionTestUnit, HandlesSimpleOperationWithIdentifiers4) {
    const char* expression = "(1)/(2)";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    // Předpokládáme, že x a y jsou inicializované identifikátory v symstack
    // Zde je potřeba nastavit hodnoty x a y v symstack
    // Například x může mít hodnotu 5 a y hodnotu 2

    expressionStruct result = createExpression(testFile, symstack, &token);

    fclose(testFile);

    // Očekáváme, že postfixové pole obsahuje určitý počet tokenů
    const int expectedTokenCount = 3;
    EXPECT_EQ(expectedTokenCount, result.postFixLen);

    // Kontrola jednotlivých tokenů
    // Předpokládáme, že tokeny jsou ve formátu: x 3 + y -
    EXPECT_EQ(TOK_int, result.postFix[0].tokenType);
    EXPECT_EQ(1, result.postFix[0].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[1].tokenType);
    EXPECT_EQ(2, result.postFix[1].tokenValue.num);

    EXPECT_EQ(TOK_div, result.postFix[2].tokenType);


    // Uvolnění případně alokované paměti
    free(result.postFix);
}



TEST_F(ExpressionTestUnit, ComplexExpressionWithRedundantBrackets) {
    const char* expression = "((((2)) + ((3) * ((())4 - (((((1)))))))) / ((5))) - ((6) + (7 - (((8)) / 2)))";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    expressionStruct result = createExpression(testFile, symstack, &token);

    fclose(testFile);

    // Očekávaný počet tokenů v postfixovém výrazu
    const int expectedTokenCount = 17;
    EXPECT_EQ(expectedTokenCount, result.postFixLen);

    // Kontrola jednotlivých tokenů
    EXPECT_EQ(TOK_int, result.postFix[0].tokenType);
    EXPECT_EQ(2, result.postFix[0].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[1].tokenType);
    EXPECT_EQ(3, result.postFix[1].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[2].tokenType);
    EXPECT_EQ(4, result.postFix[2].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[3].tokenType);
    EXPECT_EQ(1, result.postFix[3].tokenValue.num);

    EXPECT_EQ(TOK_sub, result.postFix[4].tokenType);

    EXPECT_EQ(TOK_mul, result.postFix[5].tokenType);

    EXPECT_EQ(TOK_add, result.postFix[6].tokenType);

    EXPECT_EQ(TOK_int, result.postFix[7].tokenType);
    EXPECT_EQ(5, result.postFix[7].tokenValue.num);

    EXPECT_EQ(TOK_div, result.postFix[8].tokenType);

    EXPECT_EQ(TOK_int, result.postFix[9].tokenType);
    EXPECT_EQ(6, result.postFix[9].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[10].tokenType);
    EXPECT_EQ(7, result.postFix[10].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[11].tokenType);
    EXPECT_EQ(8, result.postFix[11].tokenValue.num);

    EXPECT_EQ(TOK_int, result.postFix[12].tokenType);
    EXPECT_EQ(2, result.postFix[12].tokenValue.num);

    EXPECT_EQ(TOK_div, result.postFix[13].tokenType);

    EXPECT_EQ(TOK_sub, result.postFix[14].tokenType);

    EXPECT_EQ(TOK_add, result.postFix[15].tokenType);

    EXPECT_EQ(TOK_sub, result.postFix[16].tokenType);

    // Uvolnění případně alokované paměti
    free(result.postFix);
}

TEST_F(ExpressionTestUnit, CombinedArithmeticAndComparisonOperation3) {
    const char* expression = "34.5 + 42";
    FILE* testFile = createTestFileWithExpression(expression);

    unsigned postfixLength = 0;
    expressionStruct exprData;

    Token token;
    getToken(&token, testFile);

    expressionStruct result = createExpression(testFile, symstack, &token);

    // Očekáváme, že výsledek bude typu bool kvůli operátoru porovnání
    EXPECT_EQ(TYPE_DOUBLE, result.type);

    fclose(testFile);
    free(result.postFix); // Uvolnění alokované paměti pro postfixové pole
}









int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}