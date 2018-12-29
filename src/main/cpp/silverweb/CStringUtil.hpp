#ifndef __SILVERWEB__CSTRINGUTIL_HPP__
#define __SILVERWEB__CSTRINGUTIL_HPP__

#include <string>
#include <string.h>
#include <stddef.h>

namespace silverweb {

  struct HashCString {
    constexpr HashCString() { }
    
    size_t operator()(const char* s) const { return HashCString::compute(s); }
    size_t operator()(const char* s, const char* end) const {
      return HashCString::compute(s);
    }

    static constexpr size_t compute(const char* s) {
      size_t h = 5381;
      for (const char* p = s; *p; ++p) {
	h = (h << 5) + h + (*p);
      }
      return h;
    }

    static constexpr size_t compute(const char* s, const char* end) {
      size_t h = 5381;
      for (const char* p = s; p != end; ++p) {
	h = (h << 5) + h + (*p);
      }
      return h;
    }

    static size_t compute(const std::string& s) {
      return compute(s.c_str());
    }
  };

  struct CStringsEqual {
    size_t operator()(const char* left, const char* right) {
      return !::strcmp(left, right);
    }
  };

}

#endif
