#define UNIT_TEST

#include <cstdio>
#include <cmath>
#include <gtest/gtest.h>

extern "C" {
#include "../src/scanner.c"
#include "../src/string.c"
}

class ScannerTestUnit : public testing::Test
{
protected:
	FILE *stream;
	String *string;

	ScannerTestUnit() {
		string = (String *)malloc(sizeof(String));

		initStr(string);
	}

	~ScannerTestUnit() override {


	}

	void SetUp() override {

	}

	void TearDown() override {

	}
};

