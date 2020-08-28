#pragma once

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace ovis {

struct Triangle {
  Triangle() = default;
  Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) : vertices{v0, v1, v2} {}

  glm::vec3 vertices[3];
};

// The vertices are assumed to be counter clockwise.
// The result is not normalized!
// \see CalculateNormalizedNormal
inline glm::vec3 CalculateNormal(Triangle triangle) {
  return glm::cross(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[0]);
}

inline glm::vec3 CalculateNormalizedNormal(Triangle triangle) {
  return glm::normalize(CalculateNormal(triangle));
}

}  // namespace ovis