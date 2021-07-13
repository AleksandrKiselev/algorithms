#pragma once
#include <gtest/gtest.h>

TEST(HelloTest, BasicAssertion) {
	EXPECT_STRNE("hello", "world");
	EXPECT_EQ(6 * 7, 42);
}
