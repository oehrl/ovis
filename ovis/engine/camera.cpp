#include <SDL2/SDL_assert.h>
#include <glm/gtc/matrix_transform.hpp>

#include <ovis/engine/camera.hpp>

namespace ovis {

glm::mat4 Camera::CalculateProjectionMatrix() const {
  switch (projection_type_) {
    case ProjectionType::ORTHOGRAPHIC: {
      const float half_height = vertical_field_of_view_ * 0.5f;
      const float half_width = half_height * aspect_ratio_;
      return glm::ortho(-half_width, half_width, -half_height, half_height, near_clip_plane_, far_clip_plane_);
    }

    case ProjectionType::PERSPECTIVE: {
      return glm::perspectiveLH(vertical_field_of_view_, aspect_ratio_, near_clip_plane_, far_clip_plane_);
    }

    default:
      SDL_assert(false && "");
      return glm::mat4{};
  }
}

glm::mat4 Camera::CalculateViewProjectionMatrix() const {
  return CalculateProjectionMatrix() * transform_.CalculateInverseMatrix();
}

}  // namespace ovis