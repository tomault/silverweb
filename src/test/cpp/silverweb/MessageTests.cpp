#include <silverweb/Message.hpp>
#include <silverweb/NodeId.hpp>
#include <gtest/gtest.h>

using namespace silverweb;

TEST(MessageTests, CreateEmpty) {
  Message msg;

  EXPECT_EQ(0, msg.size());
  EXPECT_EQ(2, msg.refCnt());
  EXPECT_EQ(msg.end(), msg.begin());
}

TEST(MessageTests, CreateUninitialized) {
  Message msg(1024);

  EXPECT_EQ(1024, msg.size());
  EXPECT_EQ(1, msg.refCnt());
  EXPECT_EQ(1024, msg.end() - msg.begin());

  // Write to uninitialized data area and check for violations in valgrind...
  for (auto p = msg.begin(); p != msg.end(); ++p) {
    *p = 0xBB;
  }
}

TEST(MessageTests, CreateFromCString) {
  const char* const payload = "Hello, World!";
  Message msg(payload);

  EXPECT_EQ(::strlen(payload), msg.size());
  EXPECT_EQ(1, msg.refCnt());
  EXPECT_EQ(::strlen(payload), msg.end() - msg.begin());
  EXPECT_EQ(std::string(payload), std::string(msg.begin(), msg.end()));
}

TEST(MessageTests, CreateFromString) {
  const std::string payload("Hello, World!");
  Message msg(payload);

  EXPECT_EQ(payload.size(), msg.size());
  EXPECT_EQ(1, msg.refCnt());
  EXPECT_EQ(payload.size(), msg.end() - msg.begin());
  EXPECT_EQ(payload, std::string(msg.begin(), msg.end()));
}

TEST(MessageTests, CreateFromBoundedSequence) {
  const uint8_t PAYLOAD[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xC0, 0xDE };
  const uint8_t* const END = PAYLOAD + sizeof(PAYLOAD);
  Message msg(PAYLOAD, END);

  EXPECT_EQ(1, msg.refCnt());
  EXPECT_EQ(sizeof(PAYLOAD), msg.size());
  EXPECT_EQ(sizeof(PAYLOAD), msg.end() - msg.begin());

  for (int i = 0; i < sizeof(PAYLOAD); ++i) {
    ASSERT_EQ(PAYLOAD[i], msg.begin()[i]);
  }
}

TEST(MessageTests, CopyConstruction) {
  const std::string payload = "Hello, World!";
  Message msg(payload);
  uint8_t* msgData = msg.begin();

  {
    Message copy(msg);

    EXPECT_EQ(2, msg.refCnt());
    EXPECT_EQ(payload.size(), msg.size());
    EXPECT_EQ(msgData, msg.begin());
    EXPECT_EQ(msgData + payload.size(), msg.end());
  

    EXPECT_EQ(2, copy.refCnt());
    EXPECT_EQ(msg.size(), copy.size());
    EXPECT_EQ(msg.begin(), copy.begin());
    EXPECT_EQ(msg.end(), copy.end());
  }

  EXPECT_EQ(1, msg.refCnt());
  EXPECT_EQ(payload.size(), msg.size());
  EXPECT_EQ(msgData, msg.begin());
  EXPECT_EQ(msgData + payload.size(), msg.end());
}

TEST(MessageTests, CopyAssignment) {
  const std::string payload = "Hello, World!";
  Message msg(payload);
  uint8_t* msgData = msg.begin();

  {
    Message copy("Moo!");
    copy = msg;

    EXPECT_EQ(2, msg.refCnt());
    EXPECT_EQ(payload.size(), msg.size());
    EXPECT_EQ(msgData, msg.begin());
    EXPECT_EQ(msgData + payload.size(), msg.end());
  
    EXPECT_EQ(2, copy.refCnt());
    EXPECT_EQ(msg.size(), copy.size());
    EXPECT_EQ(msg.begin(), copy.begin());
    EXPECT_EQ(msg.end(), copy.end());
  }

  EXPECT_EQ(1, msg.refCnt());
  EXPECT_EQ(payload.size(), msg.size());
  EXPECT_EQ(msgData, msg.begin());
  EXPECT_EQ(msgData + payload.size(), msg.end());
}

TEST(MessageTests, ToString) {
  const std::string payload = "Hello, World!";
  Message msg(payload);

  EXPECT_EQ(payload, msg.str());
}

TEST(MessageTests, Unique) {
  const std::string payload = "Hello, World!";
  Message msg(payload);
  Message copy(msg);
  Message unique(msg.unique());

  EXPECT_EQ(2, msg.refCnt());
  EXPECT_EQ(payload.size(), msg.size());
  EXPECT_EQ(payload, std::string(msg.begin(), msg.end()));

  EXPECT_EQ(2, copy.refCnt());
  EXPECT_EQ(payload.size(), copy.size());
  EXPECT_EQ(payload, std::string(copy.begin(), copy.end()));
  
  EXPECT_EQ(1, unique.refCnt());
  EXPECT_EQ(payload.size(), unique.size());
  EXPECT_EQ(payload, std::string(unique.begin(), unique.end()));

  Message msg2("Moo!");
  const uint8_t* msg2Begin = msg2.begin();
  const uint8_t* msg2End = msg2.end();

  msg2 = msg2.unique();
  EXPECT_EQ(1, msg2.refCnt());
  EXPECT_EQ(4, msg2.size());
  EXPECT_EQ(msg2Begin, msg2.begin());
  EXPECT_EQ(msg2End, msg2.end());
  EXPECT_EQ("Moo!", std::string(msg2.begin(), msg2.end()));
}
