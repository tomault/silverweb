#ifndef __SILVERWEB__DETAIL__NODEIDINFO_HPP__
#define __SILVERWEB__DETAIL__NODEIDINFO_HPP__

#include <silverweb/CStringUtil.hpp>
#include <string>

namespace silverweb {
  namespace detail {

    class NodeIdInfo {
    public:
      NodeIdInfo(const std::string& text):
	  text_(text), hashCode_(HashCString::compute(text)) {
      }
      NodeIdInfo(std::string&& text):
	  text_(std::move(text)), hashCode_(HashCString::compute(text_)) {
      }

      const std::string& text() const { return text_; }
      size_t hashCode() const { return hashCode_; }

    private:
      std::string text_;
      size_t hashCode_;
    };
    
  }
}
#endif
