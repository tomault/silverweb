#include "TransmitBuffer.hpp"
#include <pistis/exceptions/IOError.hpp>
#include <silverweb/exceptions/TransmitBufferFullError.hpp>
#include <unistd.h>

using namespace pistis::exceptions;
using namespace silverweb;
using namespace silverweb::io;

TransmitBuffer::TransmitBuffer(TransmitBuffer&& other):
    message_(other.message_), sent_(other.sent_) {
  other.message_ = Message();
  other.sent_ = 0;
}

void TransmitBuffer::put(Message msg) {
  if (haveMessage_()) {
    throw TransmitBufferFullError(PISTIS_EX_HERE);
  }
  message_ = msg;
  toSend_ = message_.size() + message_.headerSize_();
  sent_ = 0;
}

TransmitBuffer::Result TransmitBuffer::pump(int fd) {
  if (!haveMessage_()) {
    return NO_MESSAGE;
  } else {
    assert(sent_ < toSend_);
    ssize_t n = ::write(fd, message_.header_() + sent_, toSend_ - sent_);
    if (n > 0) {
      sent_ += n;
      if (sent_ < toSend_) {
	return IN_PROGRESS;
      } else {
	message_ = Message();
	toSend_ = 0;
	sent_ = 0;
	return SENT;
      }
    } else if (!n || (errno == EINTR) || (errno == EAGAIN) ||
	       (errno == EWOULDBLOCK)) {
      return IN_PROGRESS;
    } else if (errno == EPIPE) {
      return DISCONNECT;
    } else {
      throw IOError::fromSystemError("Write to socket failed (#ERR#)", errno,
				     PISTIS_EX_HERE);
    }
  }  
}

TransmitBuffer& TransmitBuffer::operator=(TransmitBuffer&& other) {
  if (this != &other) {
    message_ = other.message_;
    other.message_ = Message();
    sent_ = other.sent_;
    other.sent_ = 0;
  }
  return *this;
}
