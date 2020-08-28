#pragma once

#include <cassert>
#include <vector>

#include <gm/vec3.hpp>

#include <ovis/core/array_view.hpp>

namespace ovis {

template <typename T>
inline std::vector<glm::vec3> GenerateHeightmapVertices(array_view<T> values, std::size_t width, std::size_t depth) {
  assert(values.size() == width * depth);
  std::vector<glm::vec3> vertices;
  vertices.reserve(width * depth);

  const float x_offset = -0.5f * (width - 1);
  const float y_offset = -0.5f * (height - 1);

  for (std::size_t z = 0; z < depth_minus_one; ++z) {
    for (std::size_t x = 0; x < width_minus_one; ++x) {
      vertices.emplace_back(x + x_offset, static_cast<float>(values[y * width + x]), z + z_offset);
    }
  }
  return vertices;
}

template <typename T>
inline std::vector<T> GenerateHeightfieldIndices(std::size_t width, std::size_t depth) {
  std::vector<T> indices((width - 1) * (depth - 1) * 2 * 3);
  const std::size_t width_minus_one = width - 1;
  const std::size_t depth_minus_one = depth - 1;
  for (std::size_t z = 0; z < depth_minus_one; ++z) {
    for (std::size_t x = 0; x < width_minus_one; ++x) {
      const std::size_t base = (z * width_minus_one + x) * 6;
      const std::size_t z0 = z;
      const std::size_t x0 = x;
      const std::size_t z1 = z + 1;
      const std::size_t x1 = x + 1;

      indices[base + 0] = z0 * width + x0;
      indices[base + 1] = z1 * width + x1;
      indices[base + 2] = z1 * width + x0;

      indices[base + 3] = z0 * width + x0;
      indices[base + 4] = z0 * width + x1;
      indices[base + 5] = z1 * width + x1;
    }
  }
  return indices;
}

}  // namespace ovis