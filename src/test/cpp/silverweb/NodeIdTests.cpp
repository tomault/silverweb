#include <silverweb/NodeId.hpp>
#include <gtest/gtest.h>

using namespace silverweb;

TEST(NodeIdTests, Create) {
  NodeId nodeId("moo");

  EXPECT_EQ("moo", nodeId.text());
  EXPECT_EQ(HashCString::compute("moo"), nodeId.hash());
}

TEST(NodeIdTests, Compare) {
  NodeId id1("cow");
  NodeId id2("penguin");
  NodeId id3("cow");

  EXPECT_GT(0, id1.cmp(id2));
  EXPECT_LT(0, id2.cmp(id1));
  EXPECT_EQ(0, id1.cmp(id3));
  EXPECT_EQ(0, id3.cmp(id1));
}

TEST(NodeIdTests, RelationalOperations) {
  NodeId id1("cow");
  NodeId id2("penguin");
  NodeId id3("cow");

  EXPECT_TRUE(id1 < id2);
  EXPECT_FALSE(id1 < id3);
  EXPECT_FALSE(id2 < id1);

  EXPECT_TRUE(id1 <= id2);
  EXPECT_TRUE(id1 <= id3);
  EXPECT_FALSE(id2 <= id1);
  
  EXPECT_TRUE(id2 > id1);
  EXPECT_FALSE(id1 > id3);
  EXPECT_FALSE(id1 > id1);

  EXPECT_TRUE(id2 >= id2);
  EXPECT_TRUE(id1 >= id3);
  EXPECT_FALSE(id1 >= id2);

  EXPECT_TRUE(id1 == id3);
  EXPECT_FALSE(id1 == id2);

  EXPECT_TRUE(id1 != id2);
  EXPECT_FALSE(id1 != id3);
}
