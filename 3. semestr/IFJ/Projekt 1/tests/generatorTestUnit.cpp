#include "generatorTestUnit.h"
#include <filesystem>

TEST_F(GeneratorTestUnit, Comment) {
    testing::internal::CaptureStdout();
    comment(4, "Ahoj", "Tohle", "Je", "Koment");

    ASSERT_EQ(testing::internal::GetCapturedStdout(), "# Ahoj Tohle Je Koment \n");

    testing::internal::CaptureStdout();
    comment(0);

    ASSERT_EQ(testing::internal::GetCapturedStdout(), "# \n");

}

TEST_F(GeneratorTestUnit, GenerateVariable) {
    Tdata data;
    data.dataType = INT;
    data.idName = strdup("cislo");
    data.isKnown = true;
    data.localFrame = 0;
    data.value = (char*)malloc(sizeof(char));
    strcpy(data.value, "52");

    expressionStruct expression;

    expression.postFix = (Token*) malloc(sizeof(Token)*3);


    testing::internal::CaptureStdout();
    generateVariable(&data);
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "DEFVAR GF@cislo\nDEFVAR GF@$$cislo\nMOVE GF@$$cislo bool@false\n");

}

TEST_F(GeneratorTestUnit, generateFunctionHead) {
    Tdata data;
    data.dataType = INTQ;
    data.idName = strdup("funkce");
    data.initType = FUNC;
    data.numOfParam = 0;
    
    testing::internal::CaptureStdout();

    generateFunctionHead(&data);

    ASSERT_EQ(testing::internal::GetCapturedStdout(),
            "JUMP $endfunkce\n"
            "LABEL funkce\n"
            "CREATEFRAME\n"
            "PUSHFRAME\n"
            "DEFVAR LF@$funcReturn\n"
            "DEFVAR LF@$$funcReturn\n"
            "MOVE LF@$$funcReturn bool@true\n"
            "# Parameters pushed \n");


    data.numOfParam = 1;
    data.arrayOfParams = (Tparam *) malloc(1 * sizeof(Tparam));
    data.arrayOfParams[0].id_par = strdup("prvniParam");

    testing::internal::CaptureStdout();

    generateFunctionHead(&data);

    ASSERT_EQ(testing::internal::GetCapturedStdout(),
          "JUMP $endfunkce\n"
          "LABEL funkce\n"
          "CREATEFRAME\n"
          "PUSHFRAME\n"
          "DEFVAR LF@$funcReturn\n"
          "DEFVAR LF@$$funcReturn\n"
          "MOVE LF@$$funcReturn bool@true\n"
          "DEFVAR LF@prvniParam\n"
          "POPS LF@prvniParam\n"
          "# Parameters pushed \n");

    free(data.idName);
}

TEST_F(GeneratorTestUnit, generateFunctionHeadMultipleParams) {
    Tdata data;
    data.dataType = INTQ;
    data.idName = strdup("funkce");
    data.initType = FUNC;

    data.numOfParam = 2;
    data.arrayOfParams = (Tparam *) malloc(2 * sizeof(Tparam));
    data.arrayOfParams[0].id_par = strdup("prvniParam");
    data.arrayOfParams[1].id_par = strdup("druhyParam");

    testing::internal::CaptureStdout();

    generateFunctionHead(&data);

    ASSERT_EQ(testing::internal::GetCapturedStdout(),
          "JUMP $endfunkce\n"
          "LABEL funkce\n"
          "CREATEFRAME\n"
          "PUSHFRAME\n"
          "DEFVAR LF@$funcReturn\n"
          "DEFVAR LF@$$funcReturn\n"
          "MOVE LF@$$funcReturn bool@true\n"
          "DEFVAR LF@prvniParam\n"
          "POPS LF@prvniParam\n"
          "DEFVAR LF@druhyParam\n"
          "POPS LF@druhyParam\n"
          "# Parameters pushed \n");


}






// Hlavní funkce pro spuštění testů
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}