#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ovis {

class Transform {
 public:
  inline const glm::vec3& translation() const { return translation_; }
  inline void SetTranslation(const glm::vec3& new_translation) { translation_ = new_translation; }
  inline void Translate(const glm::vec3& offset) { translation_ += offset; }

  inline glm::vec3 scale() const { return scale_; }
  inline void SetScale(glm::vec3 new_scale) { scale_ = new_scale; }
  inline void SetScale(float new_uniform_scale) { scale_ = {new_uniform_scale, new_uniform_scale, new_uniform_scale}; }
  inline void Scale(glm::vec3 scale) { scale_ *= scale; }
  inline void Scale(float uniform_scale) { scale_ *= uniform_scale; }

  inline glm::quat rotaton() const { return rotation_; }
  inline void SetRotation(glm::quat new_rotation) { rotation_ = new_rotation; }
  inline void Rotate(glm::quat rotation_offset) { rotation_ = rotation_offset * rotation_; }
  inline void Rotate(glm::vec3 axis, float angle_in_radians) { Rotate(glm::angleAxis(angle_in_radians, axis)); }
  inline void SetYawPitchRoll(float yaw, float pitch, float roll) {
    rotation_ = glm::angleAxis(yaw, glm::vec3{0.0f, 1.0f, 0.0f}) * glm::angleAxis(pitch, glm::vec3{-1.0f, 0.0f, 0.0f}) *
                glm::angleAxis(roll, glm::vec3{0.0f, 0.0f, 1.0f});
  }

  glm::mat4 CalculateMatrix() const;
  glm::mat4 CalculateInverseMatrix() const;

 private:
  glm::vec3 translation_ = {0.0, 0.0, 0.0};
  glm::vec3 scale_ = {1.0, 1.0, 1.0};
  glm::quat rotation_;
};

}  // namespace ovis
