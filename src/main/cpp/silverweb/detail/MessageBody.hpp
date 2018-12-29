#ifndef __SILVERWEB__DETAIL__MESSAGEBODY_HPP__
#define __SILVERWEB__DETAIL__MESSAGEBODY_HPP__

#include <stddef.h>
#include <stdint.h>

namespace silverweb {
  namespace detail {

    struct MessageBody {
      uint32_t refCnt;
      uint32_t type;
      uint32_t size;
      uint8_t data[];

      static constexpr uint32_t headerSize() {
	return offsetof(MessageBody, data) - offsetof(MessageBody, type);
      }

      static MessageBody* create(uint32_t type, uint32_t size);
      static void destroy(MessageBody* body) noexcept;
      static MessageBody* empty() noexcept;
      
    private:
      MessageBody(uint32_t r = 0) noexcept:
          type(0), size(0), refCnt(r) {
      }

      MessageBody(uint32_t type_, uint32_t size_) noexcept:
	  type(type_), size(size_), refCnt(0) {
      }      
    };
    
  }
}

#endif
