#ifndef __SILVERWEB__DETAIL__NODEIDTABLE_HPP__
#define __SILVERWEB__DETAIL__NODEIDTABLE_HPP__

#include <silverweb/detail/NodeIdInfo.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace silverweb {
  namespace detail {

    class NodeIdTable {
    public:
      NodeIdTable(): nodeIds_() { }
      NodeIdTable(const NodeIdTable&) = delete;
      ~NodeIdTable();

      NodeIdInfo* get(const std::string& text);

      NodeIdTable& operator=(const NodeIdTable&) = delete;

    private:
      std::unordered_map< std::string, NodeIdInfo* > nodeIds_;
      std::mutex sync_;
    };
    
  }
}
#endif
