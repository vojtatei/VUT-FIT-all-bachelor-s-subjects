#include "scannerTest.h"
#include <filesystem>

TEST_F(ScannerTest, TestProcessLineString) {
    FILE *swiftFile; 
    swiftFile = fopen("./tests/testinput/scanner-lineString.swift", "r");

    Token token;
    int result;

    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_EOL);

    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_keyword);
    EXPECT_EQ(token.tokenValue.keyword, KW_var);

    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_identifier);
    
    free(token.tokenValue.string);
    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_equal);


    result = getToken(&token, swiftFile);

    char expectedString[] = "String [ a tak dal";

    int cmpresult = strcmp(expectedString, token.tokenValue.string);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_string);
    EXPECT_EQ(cmpresult, 0);

    free(token.tokenValue.string);

    result = getToken(&token, swiftFile);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_EOL);    
    // empty string
    result = getToken(&token, swiftFile);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_string);

    fclose(swiftFile);
}

TEST_F(ScannerTest, TestProcessMultiString) {
    FILE *swiftFile; 
    swiftFile = fopen("./tests/testinput/scanner-multiString.swift", "r");

    Token token;
    int result;

    result = getToken(&token, swiftFile);

    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_EOL);

    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_keyword);
    EXPECT_EQ(token.tokenValue.keyword, KW_var);

    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_identifier);

    free(token.tokenValue.string);
    
    result = getToken(&token, swiftFile);

    EXPECT_EQ(token.tokenType, TOK_equal);

    result = getToken(&token, swiftFile);

    char expectedString[] = "tohle je\n\tmulti\n[\nline\nstring";
    int cmpresult = strcmp(expectedString, token.tokenValue.string);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_string);
    EXPECT_EQ(cmpresult, 0);

    fclose(swiftFile);
    free(token.tokenValue.string);
}

TEST_F(ScannerTest, TestGetToken) {
    FILE *swiftFile; 
    swiftFile = fopen("./tests/testinput/scannerManual.swift", "a+");

    if (swiftFile == NULL) {
        fprintf(stderr, "Nepodarilo se soubor otevrit");
        return;
    }

    int result;
    int cmpresult;
    Token token;

    fprintf(swiftFile, "Int? ident = 85 errorident#");
    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/scannerManual.swift", "r");

    result = getToken(&token, swiftFile);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_keyword);
    EXPECT_EQ(token.tokenValue.keyword, KW_intQ);

    result = getToken(&token, swiftFile);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_identifier);

    cmpresult = strcmp("ident", token.tokenValue.string);
    EXPECT_EQ(cmpresult, 0);

    free(token.tokenValue.string);

    result = getToken(&token, swiftFile);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_equal);

    result = getToken(&token, swiftFile);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_int);
    EXPECT_EQ(token.tokenValue.num, 85);

    result = getToken(&token, swiftFile);

    EXPECT_EQ(result, 0);
    EXPECT_EQ(token.tokenType, TOK_identifier);
    cmpresult = strcmp("errorident", token.tokenValue.string);
    EXPECT_EQ(cmpresult, 0);

    free(token.tokenValue.string);

    result = getToken(&token, swiftFile);

    EXPECT_EQ(result, 1);

    remove("./tests/testinput/scannerManual.swift");
    fclose(swiftFile);
}

TEST_F(ScannerTest, TestGetTokenPrintedOutput) {

    FILE *swiftFile; 
    swiftFile = fopen("./tests/testinput/scanner-1.swift", "r");

    FILE *outputFile;
    outputFile = fopen("./tests/testoutputs/scanner-1-actual.txt", "w");

    Token token;

    int result = 0;

    while (result == 0 || result == 1) {
        result = getToken(&token, swiftFile);

        if (result == 0) {
            fprintf(outputFile, "%s ", tokenTypeStrings[token.tokenType]);

            if (token.tokenType == TOK_string) {
                fprintf(outputFile, "String: %s", token.tokenValue.string);
            }
            else if (token.tokenType == TOK_identifier) {
                fprintf(outputFile, "Ident: %s", token.tokenValue.string);
            }

            fprintf(outputFile, "\n");
        }
        else if (result == 1) {
            fprintf(outputFile, "LEX ERROR\n");
        }

        if (token.tokenType == TOK_identifier || (token.tokenType == TOK_string && token.tokenValue.string != NULL)) {
            free(token.tokenValue.string);
            token.tokenValue.string = NULL;
        }
    }
    
    if (result == 2) {
        fprintf(outputFile, "%s ", tokenTypeStrings[token.tokenType]);
    }


    fclose(swiftFile);
    fclose(outputFile);

}

TEST_F(ScannerTest, TestGetTokenPrintedOutput2) {
    FILE *swiftFile; 
    swiftFile = fopen("./tests/testinput/scanner-2.swift", "r");

    FILE *outputFile;
    outputFile = fopen("./tests/testoutputs/scanner-2-actual.txt", "w");

    Token token;

    int result = 0;

    while (result == 0 || result == 1) {
        result = getToken(&token, swiftFile);
        if (result == 0) {
            fprintf(outputFile, "%s ", tokenTypeStrings[token.tokenType]);

            if (token.tokenType == TOK_string) {
                fprintf(outputFile, "String: %s", token.tokenValue.string);
            }
            else if (token.tokenType == TOK_identifier) {
                fprintf(outputFile, "Ident: %s", token.tokenValue.string);
            }

            fprintf(outputFile, "\n");
        }
        else if (result == 1) {
            fprintf(outputFile, "LEX ERROR\n");
        }

        if (token.tokenType == TOK_identifier || (token.tokenType == TOK_string && token.tokenValue.string != NULL)) {
            free(token.tokenValue.string);
            token.tokenValue.string = NULL;
        }
    }

    if (result == 2) {
        fprintf(outputFile, "%s ", tokenTypeStrings[token.tokenType]);
    }

    fclose(swiftFile);
    fclose(outputFile);

}


TEST_F(ScannerTest, TestGetTokenPrintedOutput3) {
    FILE *swiftFile; 
    swiftFile = fopen("./tests/testinput/scanner-3.swift", "r");

    FILE *outputFile;
    outputFile = fopen("./tests/testoutputs/scanner-3-actual.txt", "w");

    Token token;

    int result = 0;

    while (result == 0 || result == 1) {
        result = getToken(&token, swiftFile);
        if (result == 0) {
            fprintf(outputFile, "%s ", tokenTypeStrings[token.tokenType]);

            if (token.tokenType == TOK_string) {
                fprintf(outputFile, "String: %s", token.tokenValue.string);
            }
            else if (token.tokenType == TOK_identifier) {
                fprintf(outputFile, "Ident: %s", token.tokenValue.string);
            }

            fprintf(outputFile, "\n");
        }
        else if (result == 1) {
            fprintf(outputFile, "LEX ERROR\n");
            return;
        }

        if (token.tokenType == TOK_identifier || (token.tokenType == TOK_string && token.tokenValue.string != NULL)) {
            free(token.tokenValue.string);
            token.tokenValue.string = NULL;
        }
    }

    if (result == 2) {
        fprintf(outputFile, "%s ", tokenTypeStrings[token.tokenType]);
    }

    fclose(swiftFile);
    fclose(outputFile);

}



// Hlavní funkce pro spuštění testů
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}