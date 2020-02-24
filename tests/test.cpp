// Copyright 2019 dimakirol <your_email>

#include "header.hpp"
#include <gtest/gtest.h>

TEST(Example, EmptyTest)
{
  my_little_hash He_Who_Cannot_Be_Named;
   He_Who_Cannot_Be_Named.zaraza()
  EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
