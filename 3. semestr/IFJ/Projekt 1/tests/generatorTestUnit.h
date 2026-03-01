#define UNIT_TEST

#include <cstdio>
#include <cmath>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
using ::testing::HasSubstr;

extern "C" {
#include "../src/codeGenerator.c"
#include "../src/string.c"
}


class GeneratorTestUnit : public testing::Test
{
protected:

	GeneratorTestUnit() {
	}

	~GeneratorTestUnit() override {


	}

	void SetUp() override {

	}

	void TearDown() override {

	}
};

