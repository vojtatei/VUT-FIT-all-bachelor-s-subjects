#define UNIT_TEST

#include <cstdio>
#include <cmath>
#include <gtest/gtest.h>

extern "C" {
#include "../src/parser.c"
#include "../src/scanner.c"
#include "../src/string.c"
#include "../src/symtable.c"
#include "../src/symstack.c"
#include "../src/codeGenerator.c"
#include "../src/expression.c"
#include "../src/stack.c"
}

class ParserTestUnit : public testing::Test
{
protected:
	FILE *swiftFile;
	String *string;

	void SetUp() override {

	}

	void TearDown() override {
	}
};

