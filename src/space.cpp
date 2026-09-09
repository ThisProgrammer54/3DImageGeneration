#include <memory>
#include <space.hpp>

Object::Object(std::unique_ptr<Tree> t) : start(std::move(t)){};
