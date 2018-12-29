#include <silverweb/detail/NodeIdTable.hpp>
#include <silverweb/CStringUtil.hpp>
#include <gtest/gtest.h>

using namespace silverweb;
using namespace silverweb::detail;

TEST(NodeIdTableTests, Get) {
  NodeIdTable table;
  NodeIdInfo* id1 = table.get("cow");

  EXPECT_EQ("cow", id1->text());
  EXPECT_EQ(HashCString::compute("cow"), id1->hashCode());

  NodeIdInfo* id2 = table.get("penguin");
  EXPECT_EQ("penguin", id2->text());
  EXPECT_EQ(HashCString::compute("penguin"), id2->hashCode());

  NodeIdInfo* id3 = table.get("cow");
  EXPECT_EQ(id1, id3);
}

