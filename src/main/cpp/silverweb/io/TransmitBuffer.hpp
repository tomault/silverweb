#ifndef __SILVERWEB__IO__TRANSMITBUFFER_HPP__
#define __SILVERWEB__IO__TRANSMITBUFFER_HPP__

#include <silverweb/Message.hpp>
#include <stddef.h>

namespace silverweb {
  namespace io {

    class TransmitBuffer {
    public:
      enum Result {
	NO_MESSAGE,
	IN_PROGRESS,
	SENT,
	DISCONNECT
      };
      
    public:
      TransmitBuffer(): message_(), sent_(0) { }
      TransmitBuffer(const TransmitBuffer&) = delete;
      TransmitBuffer(TransmitBuffer&& other);

      Message msg() const { return message_; }
      size_t sent() const { return sent_; }

      void put(Message msg);
      Result pump(int fd);

      TransmitBuffer& operator=(const TransmitBuffer&) = delete;
      TransmitBuffer& operator=(TransmitBuffer&& other);

    private:
      Message message_;
      size_t toSend_;
      size_t sent_;

      bool haveMessage_() const { return message_.type() || message_.size(); }
    };
    
  }
}

#endif

