#include <rockreaper3d/world.hpp>

namespace rr3d {

World::World() {
  data_.resize(width_ * height_ * depth_, 10);
  for (std::size_t z = 0; z < depth_; ++z) {
    for (std::size_t y = 0; y < height_; ++y) {
      for (std::size_t x = 0; x < width_; ++x) {
        const std::size_t i = x + width_ * (y + z * height_);

        // data_[i] = (x % 2) * (y % 2) * (z % 2) * 10;
        if (x == 0 || x == width_ - 1 || y == 0 || y == height_ - 1 || z == 0 ||
            z == depth_ - 1) {
          data_[i] = 0;
        } else {
          data_[i] = rand() % 11;
        }
      }
    }
  }
}

}  // namespace rr3d