#pragma once

#include <glm/vec2.hpp>

#include <ovis/core/resource.hpp>
#include <ovis/graphics/texture2d.hpp>
#include <ovis/engine/scene_object_component.hpp>

namespace ovis {

class SpriteComponent : public SceneObjectComponent {
 public:
  std::vector<std::string> GetPropertyNames() const override;
  PropertyType GetPropertyType(const std::string& property_name) const override;
  PropertyValue GetProperty(const std::string& property_name) const override;
  void SetProperty(const std::string& property_name, const PropertyValue& value) override;

  inline glm::vec4 color() const { return color_; }
  inline glm::vec2 size() const { return size_; }

 private:
  ResourcePointer<Texture2D> texture_;
  glm::vec2 size_;
  glm::vec4 color_;
};

}  // namespace ovis
