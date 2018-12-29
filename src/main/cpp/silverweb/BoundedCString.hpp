#ifndef __SILVERWEB__BOUNDEDCSTRING_HPP__
#define __SILVERWEB__BOUNDEDCSTRING_HPP__

#include <silverweb/CStringUtil.hpp>

namespace silverweb {

  class BoundedCString {
  public:
    BoundedCString(const char* begin, const char* end):
        begin_(begin), end_(end) {
    }
    BoundedCString(const char* s): begin_(s), end_(s + ::strlen(s)) { }
    BoundedCString(const std::string& s):
        begin_(s.c_str()), end_(begin_ + s.size()) {
    }

    size_t size() const { return end_ - begin_; }
    const char* begin() const { return begin_; }
    const char* end() const { return end_; }
    size_t hash() const { return HashCString::compute(begin_, end_); }
    int cmp(const BoundedCString& other) const {
      const size_t lThis = size();
      const size_t lOther = other.size();
      const int order =
	  ::memcmp(begin(), other.begin(), std::min(lThis, lOther));
      return order ? order : ((int64_t)lThis - (int64_t)lOther);
    }

    bool operator==(const BoundedCString& other) const { return !cmp(other); }
    bool operator!=(const BoundedCString& other) const { return cmp(other); }
    bool operator<(const BoundedCString& other) const { return cmp(other) < 0; }
    bool operator<=(const BoundedCString& other) const {
      return cmp(other) <= 0;
    }
    bool operator>(const BoundedCString& other) const { return cmp(other) > 0; }
    bool operator>=(const BoundedCString& other) const {
      return cmp(other) >= 0;
    }

  private:
    const char* begin_;
    const char* end_;
  };
}

#endif
