#ifndef __SILVERWEB__MESSAGE_HPP__
#define __SILVERWEB__MESSAGE_HPP__

#include <silverweb/detail/MessageBody.hpp>
#include <algorithm>
#include <assert.h>
#include <string.h>

namespace silverweb {
  namespace io {
    class TransmitBuffer;
    class ReceiveBuffer;
  }

  class Message {
  public:
    Message() noexcept: p_(addRef_(detail::MessageBody::empty())) { }
    
    explicit Message(uint32_t size): Message(0, size) { }

    Message(uint32_t type, uint32_t size):
      p_(addRef_(detail::MessageBody::create(type, size))) {
    }
    
    Message(const char* payload): Message(0, payload) { }

    Message(uint32_t type, const char* payload):
        Message(type, (uint32_t)::strlen(payload), payload) {
    }
    
    Message(const std::string& payload): Message(0, payload) { }

    Message(uint32_t type, const std::string& payload):
        Message(type, payload.size(), payload.c_str()) {
    }

    template <typename Iterator>
    Message(uint32_t size, Iterator payload):
        Message((uint32_t)0, size, payload) {
    }

    template <typename Iterator>
    Message(uint32_t type, uint32_t size, Iterator payload):
        p_(addRef_(detail::MessageBody::create(type, size))) {
      std::copy_n(payload, size, p_->data);
    }
    
    template <typename Iterator>
    Message(Iterator payload, Iterator end):
        Message((uint32_t)0, payload, end) {
    }

    template <typename Iterator>
    Message(uint32_t type, Iterator payload, Iterator end):
        p_(addRef_(detail::MessageBody::create(type,
					       std::distance(payload, end)))) {
      std::copy(payload, end, p_->data);
    }
	    
    Message(const Message& other) noexcept: p_(addRef_(other.p_)) { }
    
    ~Message() noexcept { removeRef_(p_); }

    uint32_t type() const noexcept { return p_->type; }
    uint32_t size() const noexcept { return p_->size; }
    uint32_t refCnt() const noexcept { return p_->refCnt; }
    uint8_t* begin() const noexcept { return p_->data; }
    uint8_t* end() const noexcept { return p_->data + p_->size; }

    std::string str() const { return std::string(begin(), end()); }
    
    Message unique() const {
      if (p_->refCnt == 1) {
	return *this;
      } else {
	return Message(size(), begin());
      }
    }
    
    Message& operator=(const Message& other) noexcept {
      if (p_ != other.p_) {
	removeRef_(p_);
	p_ = addRef_(other.p_);
      }
      return *this;
    }

  protected:
    uint8_t* header_() const { return (uint8_t*)&p_->type; }

    static constexpr uint32_t headerSize_() {
      return detail::MessageBody::headerSize();
    }

  private:
    detail::MessageBody* p_;

    detail::MessageBody* addRef_(detail::MessageBody* p) noexcept {
      ++(p->refCnt);
      return p;
    }

    void removeRef_(detail::MessageBody* p) noexcept {
      if (!--(p->refCnt)) {
	assert(p != detail::MessageBody::empty());
	detail::MessageBody::destroy(p);
      }
    }

    friend class io::TransmitBuffer;
  };
  
}

#endif
