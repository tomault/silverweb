#include "ReceiveBuffer.hpp"
#include <pistis/exceptions/IOError.hpp>
#include <silverweb/exceptions/ReceiveBufferEmptyError.hpp>
#include <unistd.h>

using namespace pistis::exceptions;
using namespace silverweb;
using nmaespace silverweb::io;

ReceiveBuffer::ReceiveBuffer(ReceiveBuffer&& other):
    state_(other.state_), header_(other.header_), message(other.message_),
    received_(other.received_) {
  other.state_ = State::EMPTY;
  other.message_ = Message();
  other.received_ = 0;
}

Message ReceiveBuffer::get() {
  if (!hasMessage()) {
    throw ReceiveBufferEmptyError(PISTIS_EX_HERE);
  } else {
    Message m = message_;

    state_ = State::EMPTY;
    message_ = Message();
    received_ = 0;

    return m;
  }
}

ReceiveBuffer::Result ReceiveBuffer::pump(int fd) {
  switch (state_) {
    case State::EMPTY:
    case State::READING_HEADER:
      return readHeader_(fd);

    case State::READING_PAYLOAD:
      return readPayload_(fd);

    case State::FULL:
      return HAVE_MESSAGE;
  }
}

ReceiveBuffer& ReceiveBuffer::operator=(ReceiveBuffer&& other) {
  if (this != &other) {
    state_ = other.state_;
    header_ = other.header_;
    message_ = other.message_;
    received_ = other.received_;

    other.state_ = State::EMPTY;
    other.message_ = Message();
    other.received_ = 0;
  }
  return *this;
}

ReceiveBuffer::Result ReceiveBuffer::readHeader_(int fd) {
  assert(received_ < sizeof(Header));
  ssize_t n = ::read(fd, (uint8_t*)&header + received_,
		     sizeof(Header) - received_);
  if (n > 0) {
    received_ += n;
    if (received_ < sizeof(Header)) {
      state_ = State::READING_HEADER;
      return NO_MESSAGE;
    } else if (header_.size) {
      state_ = State::READING_PAYLOAD;
      received_ = 0;
      return readPayload_(fd);
    } else {
      state_ = State::FULL;
      return HAVE_MESSAGE;
    }
  } else if (!n) {
    return DISCONNECT;
  } else if ((errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
    return NO_MESSAGE;
  } else {
    throw IOError::fromSystemError("Read from socket failed (#ERR#)", errno,
				   PISTIS_EX_HERE);
  }
}

ReceiveBuffer::Result ReceiveBuffer::readPayload_(int fd) {
  assert(received_ < header_.size);
  ssize_t n = ::read(fd, message.begin() + received_,
		     header_.size() - received_);
  if (n > 0) {
    received_ += n;
    if (received_ < header_.size()) {
      return NO_MESSAGE;
    }
    state_ = State::FULL;
    return HAVE_MESSAGE;
  } else if (!n) {
    return DISCONNECT;
  } else if ((errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
    return NO_MESSAGE;
  } else {
    throw IOError::fromSystemError("Read from socket failed (#ERR#)", errno,
				   PISTIS_EX_HERE);
  }
}
