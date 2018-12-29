#ifndef __SILVERWEB__NODEID_HPP__
#define __SILVERWEB__NODEID_HPP__

#include <silverweb/detail/NodeIdInfo.hpp>
#include <functional>
#include <ostream>
#include <string.h>

namespace silverweb {

  /** @brief Id for a node */
  class NodeId {
  public:
    NodeId(): NodeId("") { }
    NodeId(const std::string& text): p_(intern_(text)) { }

    const std::string& text() const { return p_->text(); }
    size_t hash() const { return p_->hashCode(); }
    int cmp(const NodeId& other) const {
      return ::strcmp(text().c_str(), other.text().c_str());
    }

    bool operator==(const NodeId& other) const { return p_ == other.p_; }
    bool operator!=(const NodeId& other) const { return p_ != other.p_; }
    bool operator<(const NodeId& other) const {
      return text() < other.text();
    }
    bool operator<=(const NodeId& other) const {
      return text() <= other.text();
    }
    bool operator>(const NodeId& other) const {
      return text() > other.text();
    }
    bool operator>=(const NodeId& other) const {
      return text() >= other.text();
    }

  private:
    detail::NodeIdInfo* p_;

    static detail::NodeIdInfo* intern_(const std::string& text);
  };

  inline std::ostream& operator<<(std::ostream& out, const NodeId& id) {
    return out << id.text();
  }
}

namespace std {

  template <>
  struct hash<silverweb::NodeId> {
    size_t operator()(const silverweb::NodeId id) const { return id.hash(); }
  };
  
}

#endif
