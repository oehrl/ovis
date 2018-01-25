#pragma once

#include <glm/vec3.hpp>

namespace ovis {

struct Plane {
  glm::vec3 axis;
  float distance;
};
static_assert(sizeof(Plane) == 16, "");

}  // namespace ovis