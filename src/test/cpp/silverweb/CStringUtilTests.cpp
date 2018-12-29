#include <silverweb/CStringUtil.hpp>
#include <gtest/gtest.h>

using namespace silverweb;

TEST(CStringUtilTests, Hash) {
  static const char MOO[] = "moo";
  HashCString hashString;
  EXPECT_EQ(193488590, hashString("cow"));
  EXPECT_EQ(193499472, hashString(MOO, MOO + sizeof(MOO)));
}

TEST(CStringUtilTests, Equal) {
  CStringsEqual areEqual;
  EXPECT_TRUE(areEqual("cow", "cow"));
  EXPECT_FALSE(areEqual("cow", "penguin"));
}

