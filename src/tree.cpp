#include <memory>
#include <tree.hpp>

void Tree::add(std::unique_ptr<Tree> newNode) {
  this->nodes.push_back(std::move(newNode));
}
