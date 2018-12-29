#include <silverweb/detail/MessageBody.hpp>
#include <silverweb/NodeId.hpp>
#include <gtest/gtest.h>

using namespace silverweb;
using namespace silverweb::detail;

TEST(MessageBodyTests, CreateAndDestroy) {
  NodeId nodeId("ushi");
  MessageBody* body = MessageBody::create(1, 1024);

  EXPECT_EQ(1, body->type);
  EXPECT_EQ(1024, body->size);
  EXPECT_EQ(0, body->refCnt);
  // Write data and check for boundary violations in valgrind...
  for (uint32_t i = 0; i < body->size; ++i) {
    body->data[i] = 0xAA;
  }

  MessageBody::destroy(body);
}

TEST(MessageBodyTests, Empty) {
  MessageBody* body = MessageBody::empty();
  EXPECT_EQ(0, body->type);
  EXPECT_EQ(0, body->size);
  EXPECT_EQ(1, body->refCnt);

  // Don't destroy the empty message...
}
