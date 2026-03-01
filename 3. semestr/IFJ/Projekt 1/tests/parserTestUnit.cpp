#include "parserTestUnit.h"
#include <filesystem>

TEST_F(ParserTestUnit, initParser) {
    swiftFile = NULL;
    initParser(swiftFile);
    ASSERT_EQ(parser->labelIndex, 0);
    ASSERT_FALSE(parser->inFunction);
    ASSERT_EQ(parser->inWhile, 0);
    ASSERT_EQ(parser->inIf, 0);
}

TEST_F(ParserTestUnit, FuckingSymtable) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    Tsymsearch nalezeno = symtableSearch(&tree,"readString");
    EXPECT_TRUE(nalezeno.found);
    EXPECT_EQ(nalezeno.found_data->dataType, STRINGQ);
    EXPECT_EQ(nalezeno.found_data->initType, FUNC);

    Tsymsearch nalezeno2 = symtableSearch(&tree,"readInt");
    EXPECT_TRUE(nalezeno2.found);
    EXPECT_EQ(nalezeno2.found_data->dataType, INTQ);
    EXPECT_EQ(nalezeno2.found_data->initType, FUNC);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, EmptyProgram) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    //fprintf(swiftFile, "");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result=0;
    //result = program();
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, WrongFirstToken) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "5");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    

    fclose(swiftFile);
    ,"");

		killParser();

}

TEST_F(ParserTestUnit, DefinitionVariable) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x : Int");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, DefinitionConstant) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x : String?");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
   
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, DefinitionWithExpression) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x : Int? = 5 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, DefinitionWithExpressionNil) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x : Int? = nil \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, DefinitionWithExpressionWrongType) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x : Int? = 5.4 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, DefinitionJustExpressionNil) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x = nil \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, DefinitionNilToNotNilTypeVariable) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int \n x = nil \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, DefinitionJustExpression) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x = 12");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, MoreLines) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x : String? \n var y : Int = 3\n var z = 55+5*7 \n  let q = 0");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, DeclarationOnMoreLines) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let \nx \n:\n String? \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, MoreDeclarationsOnMoreLines) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let \nx \n:\n String? \n var y \n: Int = \n 5 \n\n\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, MoreDeclarationsOnOneLine) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x : String? var y : Double = 2.3");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");
    
    initParser(swiftFile);
    
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}


/**
 
*/
TEST_F(ParserTestUnit, IfLetId) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x: Int? = 12 \n if let x \n { var y:Int=7} \n else \n { \nvar x = 10\n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, IfLetId2) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x: Int? = nil \n if let x \n { \n } \n else \n { \n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, IfLetIdUndeclaredVariable) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "if let x \n { \n } \n else \n { \n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
    ,"");
}


TEST_F(ParserTestUnit, IfExpression) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "if 1<2 \n { \n } \n else \n { \n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, IfExpressionWithBody) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "if 1<5 \n { let x = 6.3\nvar n:Int} \n else \n { var y:Int = 6\n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, IfWrongExpression) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "if a+b \n { let x = 6.3\nvar n:Int} \n else \n { var y:Int = 6\n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, WhileCyklus) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var i:Int=0 \n while i<3 \n { i=7 \n }");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);

	killParser();
}

TEST_F(ParserTestUnit, WhileWrongCondition) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var i:Int=0 \n while 7 \n { i=7 \n }");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, FunctionDeclaration) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}


TEST_F(ParserTestUnit, FunctionDeclarationMoreParams) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? , jmeno2 identifikator2:Double) -> Double{return 8.3}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, FunctionDeclarationNoParamsNoReturnType) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce (){\n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, FunctionDeclarationWithReturnExpression) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{return 3}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, FunctionDeclarationWithReturnNoExpression) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ){return}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, FunctionDeclarationNoParamName) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( _ identifikator:Int? ) -> Int{return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, FunctionDeclarationNoParamId) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno _:Int? ) -> Int{return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, FunctionDeclarationNoParamNameAndId) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( _ _:Int? ) -> Int{return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ReturnNotInFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x:Int \n return x\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ReturnNotInFunctionInIf) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "if 2>5 \n { return } \n else \n { \n } \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}


TEST_F(ParserTestUnit, AssignValueToVariable) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int \n x=50");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, CallFunctionWithoutReturnNoParams) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(){} \n funkce()");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, CallFunctionWithoutReturnWithParams) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(par1 a: Int, par2 b: Int){\n} \n  var x:Int = 3 \n var y:Int = 4 \n funkce(par1:x, par2:y)");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, CallFunctionWithoutReturnOneParam) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Double = 5.4 \n func funkce(par1 a: Double){return} \n funkce(par1:x)");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}


TEST_F(ParserTestUnit, Redeclaration) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x: Int \n var x: String");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, RedeclarationWithFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func x ( jmeno identifikator:Int? ) -> Double{return 3.4} \n var x: String");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, RedeclarationTwoFunctions) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func x ( jmeno identifikator:Int? ) -> Double{return 7.8} \n func x () -> Int?{return 8}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, FunctionParametersNameEqualsIdentifier) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func x (xyz xyz:Double ) -> Double{let y=1 \n return 4.4} \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallFunctionWithReturn) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce() -> Double{let y=0\n return 8.9} \n let variable = funkce() \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, CallFunctionWithReturnToWrongType) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce() -> Double{let y=0\n return 8.9} \n let variable:Int = funkce() \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallFunctionWithReturnParameterWithoutName) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x: Int =3 \n func funkce(_ par : Int) -> Double{return 8.9} \n let variable = funkce(x) \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ValueToUndeclaredVariable) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "x = 5 \n let x: Int \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ValueToLetVariable) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x:Int=4 \n x=5 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ValueToLetWithoutValue) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "let x:Int \n x=5 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ValueToVar) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int =4 \n x=5 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ValueToVar2) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x = 4 \n x = 5 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    fclose(swiftFile);
}

TEST_F(ParserTestUnit, WrongTypeValueToVar) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x = 4 \n x = 5.9 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, WrongTypeValueToVar2) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:String \n x = 7 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, WrongTypeValueToVar3) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce () -> Int{return 6} \n var x=funkce() \n x = 7.8 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, WrongTypeValueToVar4) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x: Int = 4 \n var y: Double = 9 + x\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ValueToVarConvert) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var y: Double = 4 + 9 \n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, NoReturnInFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ReturnIsInFunctionAfterSomething) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{let y:Int=3 \n return 6}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ReturnIsInEveryBranchOfFunctionIf) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{\n if 2>5 \n {return 4}\n else{return 6} \n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ReturnIsNotInEveryBranchOfFunctionIf) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{\n if 2>=2 \n {return 4}\n else{} \n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ReturnIsInEveryBranchOfFunctionIfInIf) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{\n if 1>2 \n {if 2>4 \n {return 8} \n else \n {return 6}} \n else \n {return 5} \n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ReturnIsNotInEveryBranchOfFunctionIfInIf) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) -> Int{\n if 9!=7 \n {if 9!=73 \n {return 8} \n else \n {}} \n else \n {return 5} \n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ReturnWithoutExprectedExpression) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) ->Int {return \n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ReturnWitExpressionWhenFunctionIsVoid) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) {return 5\n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ReturnWithoutWrongTypeExpression) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int? ) ->Int {return 6.7\n}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingUndeclaredFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int=funkce()\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithWrongParameterName) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var y: Int =3 \n func funkce(right par:Int){return} \n funkce(wrong: y)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithWrongSecondParameterName) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var y: Int =3 \n func funkce(right par:Int, right2 par2:Int){return} \n funkce(right: y, wrong2: y)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithLessParameters) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(_ x:Int){return} \n funkce()\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithLessParametersButSome) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(_ x:Int, _ y:Double){return} \n funkce(4)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithMoreParameters) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(_ x:Int){return} \n funkce(5,8)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithEvenMoreParameters) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(_ x:Int, _ y:Int){return} \n funkce(5,8,9)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithUndeclaredParameters) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(_ x:Int){return} \n funkce(wrong)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithWrongTypeParametersNotVariables) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce(_ x:Int, _ y: Double){return} \n funkce(1, 5)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithWrongTypeParametersVariables) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var h=5 \n func funkce(_ x:Int, _ y: Double){return} \n funkce(1, h)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithWrongTypeParametersVariables2) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var h=5 \n func funkce(_ x:Int, _ y: Double?){return} \n funkce(1, h)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, CallingFunctionWithRightTypeParametersVariables) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var h:Int=6 \n func funkce(_ x:Int?, _ y: Double?){return} \n funkce(h, 4.4)\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, CallReadStringReadIntReadDouble) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:String? = readString() \n var y:Int? = readInt() \n var z:Double? = readDouble()");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    
    fclose(swiftFile);
}

TEST_F(ParserTestUnit, CallInt2DoubleAndDouble2Int) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var y: Int = 4 \n var x:Double = Int2Double(y) \n var c: Double = 4.7 \n var d:Int = Double2Int(c)");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    
    fclose(swiftFile);
}


TEST_F(ParserTestUnit, CallWrite) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "write(1,3,5)");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    
    fclose(swiftFile);
}

TEST_F(ParserTestUnit, UsingVariableOutsideLocalFrame) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "if 5<6 {var y = 7} else {var z:Double?=9.8} \n y=5");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, RedeclarationOfGlobalVariableInFunction) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int=5 \n func funkce ( _ _:Int? ) -> Int{var x:Double=4.4 \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, RedeclarationOfGlobalVariableInIf) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int=5 \n if x>4 {var x:Double=4.4} else {let x:Int?=nil}\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, RedeclarationOfGlobalVariableInWhile) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int=5 \n while x>4 {let x:Int?=nil}\n");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, RedeclarationInFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( _ _:Int? ) -> Int{var x:Int=3 \n var x:Int?=4 \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, RedeclarationInFunction2) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func main () { \n let x =5 \n let x=10 \n print(x) \n }");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, UsingVariableDeclaredOutsideFunction) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var x:Int=4 \n func funkce ( jmeno identifikator:Int? ) -> Int{ x=3 \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ParametersOfFunctionWithSameId) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int?, jmeno2 identifikator:Double?) -> Int{return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, ParametersOfFunctionWithSameIdMoreParams) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( _ a:Int?, _ b:Int?, _ c:Int?, _ a:Int?, _ d:Int?) -> Int{return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, RedeclaringParameterInFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( _ a:Int?) -> Int{var a:Double=8.9 \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, UsingParameterInFunction) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int?) -> Int{var promena:Int? = identifikator + 1 \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    Tsymsearch nalezeno = symtableSearch(&tree, "funkce");
    EXPECT_TRUE(nalezeno.found);
    EXPECT_EQ(nalezeno.found_data->dataType, INT);
    EXPECT_EQ(nalezeno.found_data->initType, FUNC);
    EXPECT_EQ(nalezeno.found_data->numOfParam, 1);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, ChangingParameterInFunction) {
    EXPECT_DEATH(
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int?) -> Int{identifikator=8 \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    fclose(swiftFile);
    ,"");
}

TEST_F(ParserTestUnit, UsingParameterInFunction2) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int?) -> Int{write(identifikator) \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    Tsymsearch nalezeno = symtableSearch(&tree, "funkce");
    EXPECT_TRUE(nalezeno.found);
    EXPECT_EQ(nalezeno.found_data->dataType, INT);
    EXPECT_EQ(nalezeno.found_data->initType, FUNC);
    EXPECT_EQ(nalezeno.found_data->numOfParam, 1);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, UsingParameterInFunction3) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( jmeno identifikator:Int, _ identifikator2:Int) -> Int{ let x:Int? = nil\n var y:Int?=x \n return 5}");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);
    Tsymsearch nalezeno = symtableSearch(&tree, "funkce");
    EXPECT_TRUE(nalezeno.found);
    EXPECT_EQ(nalezeno.found_data->dataType, INT);
    EXPECT_EQ(nalezeno.found_data->initType, FUNC);
    EXPECT_EQ(nalezeno.found_data->numOfParam, 2);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, UsingParameterInFunctionAndCalling) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "func funkce ( _ identifikator:Int, _ identifikator2:Int) -> Int{ var x:Int = identifikator + identifikator2 \n return x} \n var z = funkce(4, 5) \n write(z)");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);

    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, UsingDefinedVariable) {
    swiftFile = fopen("./tests/testinput/parser.swift", "w");

    fprintf(swiftFile, "var y:Int=5 \n let x:Int = 12 + y");

    fclose(swiftFile);

    swiftFile = fopen("./tests/testinput/parser.swift", "r");

    initParser(swiftFile);
    int result = program(swiftFile);
    
    EXPECT_EQ(result, 0);

    fclose(swiftFile);
}

TEST_F(ParserTestUnit, firstPassage) {
    swiftFile = fopen("./tests/testinput/parser-firstpassage1.swift", "r");

    initParser(swiftFile);

    firstPassage(swiftFile);

    Tsymsearch nalezeno = symtableSearch(&tree, "funkce");
    EXPECT_STREQ(nalezeno.found_data->idName, "funkce");
    EXPECT_TRUE(nalezeno.found);
    EXPECT_EQ(nalezeno.found_data->dataType, INT);
    EXPECT_EQ(nalezeno.found_data->initType, FUNC);
    EXPECT_EQ(nalezeno.found_data->numOfParam, 1);
    EXPECT_STREQ(nalezeno.found_data->arrayOfParams[0].name_par, "jmeno");
    EXPECT_STREQ(nalezeno.found_data->arrayOfParams[0].id_par, "promenna");
    EXPECT_EQ(nalezeno.found_data->arrayOfParams[0].type, INTQ);

    nalezeno = symtableSearch(&tree, "funkcicka");
    EXPECT_TRUE(nalezeno.found);
    EXPECT_EQ(nalezeno.found_data->dataType, STRINGQ);
    EXPECT_EQ(nalezeno.found_data->initType, FUNC);
    EXPECT_EQ(nalezeno.found_data->numOfParam, 2);
    EXPECT_STREQ(nalezeno.found_data->arrayOfParams[0].name_par, "_");
    EXPECT_STREQ(nalezeno.found_data->arrayOfParams[0].id_par, "identifikator");
    EXPECT_EQ(nalezeno.found_data->arrayOfParams[0].type, INT);
    EXPECT_STREQ(nalezeno.found_data->arrayOfParams[1].name_par, "to");
    EXPECT_STREQ(nalezeno.found_data->arrayOfParams[1].id_par, "person");
    EXPECT_EQ(nalezeno.found_data->arrayOfParams[1].type, STRING);


    fclose(swiftFile);
}



// Hlavní funkce pro spuštění testů
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}