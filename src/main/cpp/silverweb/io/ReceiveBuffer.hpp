#ifndef __SILVERWEB__IO__RECEIVEBUFFER_HPP__
#define __SILVERWEB__IO__RECEIVEBUFFER_HPP__

#include <silverweb/Message.hpp>
#include <stddef.h>

namespace silverweb {
  namespace io {

    class ReceiveBuffer {
    public:
      enum Result {
	NO_MESSAGE,
	HAVE_MESSAGE,
	DISCONNECT
      };
	
    public:
      ReceiveBuffer():
	  state_(State::EMPTY), header_(), message_(), received_(0) {
      }
      ReceiveBuffer(const ReceiveBuffer&) = delete;
      ReceiveBuffer(ReceiveBuffer&& other);

      bool hasMessage() const { return state_ == State::FULL; }
      Message get();
      Result pump(int fd);

      ReceiveBuffer& operator=(const ReceiveBuffer&) = delete;
      ReceiveBuffer& operator=(ReceiveBuffer&& other);

    private:
      struct Header {
	uint32_t type;
	uint32_t size;
      };

      enum class State {
	EMPTY,
	READING_HEADER,
	READING_PAYLOAD,
	FULL
      };
      
    private:
      State state_;
      Header header_;
      Message message_;
      size_t received_;

      Result readHeader_(int fd);
      Result readPayload_(int fd);
    };
    
  }
}

#endif
