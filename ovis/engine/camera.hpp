#pragma once

#include <ovis/math/transform.hpp>

namespace ovis {

enum class ProjectionType {
  ORTHOGRAPHIC,
  PERSPECTIVE,
};

class Camera {
 public:
  inline void SetProjectionType(ProjectionType projection_type) { projection_type_ = projection_type; }
  inline ProjectionType projection_type() const { return projection_type_; }

  inline void SetVerticalFieldOfView(float vertical_field_of_view) { vertical_field_of_view_ = vertical_field_of_view; }
  inline float vertical_field_of_view() const { return vertical_field_of_view_; }

  inline void SetAspectRadio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }
  inline float aspect_ratio() const { return aspect_ratio_; }

  inline void SetNearClipPlane(float near_clip_plane) { near_clip_plane_ = near_clip_plane; }
  inline float near_clip_plane() const { return near_clip_plane_; }

  inline void SetFarClipPlane(float far_clip_plane) { far_clip_plane_ = far_clip_plane; }
  inline float far_clip_plane() const { return far_clip_plane_; }

  inline void SetTransform(const Transform& transform) { transform_ = transform; }
  inline Transform& transform() { return transform_; }
  inline const Transform& transform() const { return transform_; }

  void LookAt(const glm::vec3& target, const glm::vec3& up = {0.0f, 1.0f, 0.0f});

  glm::mat4 CalculateProjectionMatrix() const;
  glm::mat4 CalculateViewProjectionMatrix() const;

 private:
  Transform transform_;
  ProjectionType projection_type_ = ProjectionType::PERSPECTIVE;
  float vertical_field_of_view_ = glm::radians(90.0f);
  float aspect_ratio_ = 1.0f;
  float near_clip_plane_ = 0.1f;
  float far_clip_plane_ = 1000.0f;
};

}  // namespace ovis