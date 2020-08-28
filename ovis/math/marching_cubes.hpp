#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <glm/common.hpp>
#include <glm/vec3.hpp>

#include <ovis/math/triangle.hpp>

namespace ovis {

namespace detail {

extern const std::int16_t EDGE_TABLE[256];
extern const std::int8_t TRIANGLE_TABLE[256][16];
extern const glm::vec3 CELL_POSITIONS[8];

//    4-----5
//  / |   / |
// 7-----6  |
// |  0--|--1
// | /   | /
// 3-----2
template <typename T>
inline void PolygonizeCell(T c0, T c1, T c2, T c3, T c4, T c5, T c6, T c7, T isolevel, const glm::vec3& offset,
                           std::vector<Triangle>* positions) {
  assert(positions != nullptr);

  int cube_index = 0;
  if (c0 < isolevel) cube_index |= 1 << 0;
  if (c1 < isolevel) cube_index |= 1 << 1;
  if (c2 < isolevel) cube_index |= 1 << 2;
  if (c3 < isolevel) cube_index |= 1 << 3;
  if (c4 < isolevel) cube_index |= 1 << 4;
  if (c5 < isolevel) cube_index |= 1 << 5;
  if (c6 < isolevel) cube_index |= 1 << 6;
  if (c7 < isolevel) cube_index |= 1 << 7;

  if (EDGE_TABLE[cube_index] == 0) {
    return;
  }

  const auto interpolate = [isolevel](const glm::vec3& p1, const glm::vec3& p2, T v1, T v2) {
    const float t = static_cast<float>(isolevel - v1) / static_cast<float>(v2 - v1);
    return glm::mix(p1, p2, t);
  };

  std::array<glm::vec3, 12> vertices;
  if (EDGE_TABLE[cube_index] & 1) vertices[0] = interpolate(CELL_POSITIONS[0], CELL_POSITIONS[1], c0, c1);
  if (EDGE_TABLE[cube_index] & 2) vertices[1] = interpolate(CELL_POSITIONS[1], CELL_POSITIONS[2], c1, c2);
  if (EDGE_TABLE[cube_index] & 4) vertices[2] = interpolate(CELL_POSITIONS[2], CELL_POSITIONS[3], c2, c3);
  if (EDGE_TABLE[cube_index] & 8) vertices[3] = interpolate(CELL_POSITIONS[3], CELL_POSITIONS[0], c3, c0);
  if (EDGE_TABLE[cube_index] & 16) vertices[4] = interpolate(CELL_POSITIONS[4], CELL_POSITIONS[5], c4, c5);
  if (EDGE_TABLE[cube_index] & 32) vertices[5] = interpolate(CELL_POSITIONS[5], CELL_POSITIONS[6], c5, c6);
  if (EDGE_TABLE[cube_index] & 64) vertices[6] = interpolate(CELL_POSITIONS[6], CELL_POSITIONS[7], c6, c7);
  if (EDGE_TABLE[cube_index] & 128) vertices[7] = interpolate(CELL_POSITIONS[7], CELL_POSITIONS[4], c7, c4);
  if (EDGE_TABLE[cube_index] & 256) vertices[8] = interpolate(CELL_POSITIONS[0], CELL_POSITIONS[4], c0, c4);
  if (EDGE_TABLE[cube_index] & 512) vertices[9] = interpolate(CELL_POSITIONS[1], CELL_POSITIONS[5], c1, c5);
  if (EDGE_TABLE[cube_index] & 1024) vertices[10] = interpolate(CELL_POSITIONS[2], CELL_POSITIONS[6], c2, c6);
  if (EDGE_TABLE[cube_index] & 2048) vertices[11] = interpolate(CELL_POSITIONS[3], CELL_POSITIONS[7], c3, c7);

  for (unsigned index = 0; TRIANGLE_TABLE[cube_index][index] != -1; index += 3) {
    const std::int8_t vertex_indices[3] = {TRIANGLE_TABLE[cube_index][index], TRIANGLE_TABLE[cube_index][index + 1],
                                           TRIANGLE_TABLE[cube_index][index + 2]};
    positions->emplace_back(vertices[vertex_indices[0]] + offset, vertices[vertex_indices[1]] + offset,
                            vertices[vertex_indices[2]] + offset);
  }
}
}  // namespace detail

template <typename T>
inline std::vector<Triangle> MarchingCubes(const std::vector<T>& grid, const std::size_t width,
                                           const std::size_t height, const std::size_t depth, T isolevel) {
  std::vector<Triangle> triangles;
  // TODO: find a good value for reserve!

  const std::size_t width_minus_one = width - 1;
  const std::size_t height_minus_one = height - 1;
  const std::size_t depth_minus_one = depth - 1;
  const std::size_t width_times_height = width * height;
  const glm::vec3 base_offset = -glm::vec3{0.5f * width_minus_one, 0.5f * height_minus_one, 0.5f * depth_minus_one};

  for (std::size_t z = 0; z < depth_minus_one; ++z) {
    for (std::size_t y = 0; y < height_minus_one; ++y) {
      for (std::size_t x = 0; x < width_minus_one; ++x) {
        const std::size_t x0 = x;
        const std::size_t x1 = x + 1;
        const std::size_t y0 = y;
        const std::size_t y1 = y + 1;
        const std::size_t z0 = z;
        const std::size_t z1 = z + 1;
        const glm::vec3 offset = {x, y, z};

        //    4-----5
        //  / |   / |
        // 7-----6  |
        // |  0--|--1
        // | /   | /
        // 3-----2
        detail::PolygonizeCell(
            grid[x0 + y0 * width + z1 * width_times_height], grid[x1 + y0 * width + z1 * width_times_height],
            grid[x1 + y0 * width + z0 * width_times_height], grid[x0 + y0 * width + z0 * width_times_height],
            grid[x0 + y1 * width + z1 * width_times_height], grid[x1 + y1 * width + z1 * width_times_height],
            grid[x1 + y1 * width + z0 * width_times_height], grid[x0 + y1 * width + z0 * width_times_height], isolevel,
            base_offset + offset, &triangles);
      }
    }
  }
  return triangles;
}

}  // namespace ovis