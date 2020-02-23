// Copyright 2019 dimakirol <your_email>

#include <gtest/gtest.h>

#include "header.hpp"
#include <gtest/gtest.h>

TEST(Example, EmptyTest)
{
  head();
  EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
