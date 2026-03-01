#define UNIT_TEST

#include <cstdio>
#include <cmath>
#include <gtest/gtest.h>
#include <cstring>

extern "C" {
#include "../src/scanner.c"
#include "../src/string.c"
}

class ScannerTest : public testing::Test
{
protected:
	FILE *stream;
	String *string;

	ScannerTest() {

	}

	~ScannerTest() override {
		/*
		string->len = 0;
		string->allocSize = 0;
		freeStr(string);
		string = NULL;
		*/
	}

	void SetUp() override {

	}

	void TearDown() override {

	}
};

