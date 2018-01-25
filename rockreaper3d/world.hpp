#pragma once

#include <vector>
#include <ovis/math/triangle.hpp>

namespace rr3d {

class World {
 public:
  World();

  std::vector<ovis::Triangle> triangles() const;
  inline std::size_t width() const { return width_; }
  inline std::size_t height() const { return height_; }
  inline std::size_t depth() const { return depth_; }

 private:
  const std::size_t width_  = 16;
  const std::size_t height_ = 16;
  const std::size_t depth_  = 16;
  std::vector<int> data_;
  std::vector<ovis::Triangle> triangles_;
};

}  // namespace rr3d
