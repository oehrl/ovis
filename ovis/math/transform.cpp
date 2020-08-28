#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <ovis/core/log.hpp>
#include <ovis/math/transform.hpp>

namespace ovis {

glm::mat4 Transform::CalculateMatrix() const {
  const auto translation_matrix = glm::translate(translation_);
  const auto rotation_matrix = glm::mat4_cast(rotation_);
  const auto scale_matrix = glm::scale(scale_);
  return translation_matrix * rotation_matrix * scale_matrix;
}

glm::mat4 Transform::CalculateInverseMatrix() const {
  const auto translation_matrix = glm::translate(-translation_);
  const auto rotation_matrix = glm::transpose(glm::mat4_cast(rotation_));
  const auto scale_matrix = glm::scale(1.0f / scale_);
  return scale_matrix * rotation_matrix * translation_matrix;
}

}  // namespace ovis