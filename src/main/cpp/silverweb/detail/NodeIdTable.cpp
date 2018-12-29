#include "NodeIdTable.hpp"
#include <stdlib.h>

using namespace silverweb;
using namespace silverweb::detail;

NodeIdTable::~NodeIdTable() {
  for (auto& i : nodeIds_) {
    delete i.second;
  }
}

NodeIdInfo* NodeIdTable::get(const std::string& text) {
  std::unique_lock<std::mutex> sync_;
  auto i = nodeIds_.find(text);
  if (i != nodeIds_.end()) {
    return i->second;
  } else {
    NodeIdInfo* info = new NodeIdInfo(text);
    nodeIds_.insert(std::make_pair(info->text(), info));
    return info;
  }
}
