#include <silverweb/BoundedCString.hpp>
#include <silverweb/CStringUtil.hpp>
#include <gtest/gtest.h>

using namespace silverweb;

TEST(BoundedCStringTests, CreateFromEndpoints) {
  const char TEXT[] = "moo";
  const char* const END = TEXT + sizeof(TEXT) - 1;
  BoundedCString s(TEXT, END);

  EXPECT_EQ(sizeof(TEXT) - 1, s.size());
  EXPECT_EQ(TEXT, s.begin());
  EXPECT_EQ(END, s.end());
  EXPECT_EQ(HashCString::compute(TEXT, END), s.hash());
}

TEST(BoundedCStringTests, CreateFromZeroTerminated) {
  const char* const TEXT = "penguin";
  BoundedCString s(TEXT);
  
  EXPECT_EQ(::strlen(TEXT), s.size());
  EXPECT_EQ(TEXT, s.begin());
  EXPECT_EQ(TEXT + ::strlen(TEXT), s.end());
  EXPECT_EQ(HashCString::compute(TEXT), s.hash());
}

TEST(BoundedCStringTests, CreateFromString) {
  const std::string TEXT = "penguin";
  BoundedCString s(TEXT);
  
  EXPECT_EQ(TEXT.size(), s.size());
  EXPECT_EQ(TEXT.c_str(), s.begin());
  EXPECT_EQ(TEXT.c_str() + TEXT.size(), s.end());
  EXPECT_EQ(HashCString::compute(TEXT), s.hash());
}

TEST(BoundedCStringTests, Compare) {
  BoundedCString s1("moo");
  BoundedCString s2("moon");
  BoundedCString s3("moo");
  BoundedCString s4("penguin");

  EXPECT_GT(0, s1.cmp(s4));
  EXPECT_LT(0, s4.cmp(s1));
  EXPECT_GT(0, s1.cmp(s2));
  EXPECT_LT(0, s2.cmp(s1));
  EXPECT_EQ(0, s1.cmp(s3));
}

TEST(BoundedCStringTests, RelationalOperators) {
  BoundedCString s1("moo");
  BoundedCString s2("penguin");
  BoundedCString s3("moo");

  EXPECT_TRUE(s1 < s2);
  EXPECT_FALSE(s1 < s3);
  EXPECT_FALSE(s2 < s1);

  EXPECT_TRUE(s1 <= s2);
  EXPECT_TRUE(s1 <= s3);
  EXPECT_FALSE(s2 <= s1);

  EXPECT_TRUE(s2 > s1);
  EXPECT_FALSE(s3 > s1);
  EXPECT_FALSE(s1 > s2);

  EXPECT_TRUE(s2 >= s1);
  EXPECT_TRUE(s3 >= s1);
  EXPECT_FALSE(s1 >= s2);

  EXPECT_TRUE(s1 == s3);
  EXPECT_FALSE(s1 == s2);

  EXPECT_TRUE(s1 != s2);
  EXPECT_FALSE(s1 != s3);
}

