#include "MessageBody.hpp"
#include <exception>
#include <memory>
#include <stdlib.h>

using namespace silverweb::detail;

static_assert(
    (offsetof(MessageBody, data) - offsetof(MessageBody, type)) == 8,
    "MessageBody type and size fields not packed tightly"
);

MessageBody* MessageBody::create(uint32_t type, uint32_t size) {
  MessageBody* body =
      (MessageBody*)::malloc(offsetof(MessageBody, data) + size);
  if (!body) {
    throw std::bad_alloc();
  }
  new(body) MessageBody(type, size);
  return body;
};

void MessageBody::destroy(MessageBody* body) noexcept {
  body->~MessageBody();
  ::free(body);
}

MessageBody* MessageBody::empty() noexcept {
  static MessageBody EMPTY(1);
  return &EMPTY;
}
