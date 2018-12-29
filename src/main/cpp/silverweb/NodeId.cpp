#include "NodeId.hpp"
#include <silverweb/detail/NodeIdTable.hpp>

using namespace silverweb;
using namespace silverweb::detail;

namespace {
  NodeIdTable& getNodeIdTable() {
    static NodeIdTable nodeIdTable;
    return nodeIdTable;
  }
}

NodeIdInfo* NodeId::intern_(const std::string& text) {
  return getNodeIdTable().get(text);
}

