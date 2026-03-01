#define UNIT_TEST

#include <cstdio>
#include <cmath>
#include <gtest/gtest.h>

extern "C" {
#include "../src/string.c"
}

class StringTestUnit : public testing::Test
{
protected:
	FILE *stream;
	String *string;

	StringTestUnit() {
		string = (String *)malloc(sizeof(String));

		initStr(string);
	}

	~StringTestUnit() override {


	}

	void SetUp() override {

	}

	void TearDown() override {

	}
};

