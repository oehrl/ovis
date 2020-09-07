#include <SDL_assert.h>

#include <ovis/engine/base/transform2d_component.hpp>

namespace ovis {

std::vector<std::string> Transform2DComponent::GetPropertyNames() const {
  return {"Position", "Rotation", "Scale"};
}

SceneObjectComponent::PropertyType Transform2DComponent::GetPropertyType(const std::string& property_name) const {
  if (property_name == "Position") {
    return PropertyType::VECTOR2;
  } else if (property_name == "Rotation") {
    return PropertyType::FLOAT;
  } else if (property_name == "Scale") {
    return PropertyType::VECTOR2;
  } else {
    return PropertyType::UNDEFINED;
  }
}

SceneObjectComponent::PropertyValue Transform2DComponent::GetProperty(const std::string& property_name) const {
  if (property_name == "Position") {
    return glm::vec2(transform_.translation());
  } else if (property_name == "Rotation") {
    return transform_.rotaton().z;
  } else if (property_name == "Scale") {
    return glm::vec2(transform_.scale());
  } else {
    return std::monostate{};
  }
}

void Transform2DComponent::SetProperty(const std::string& property_name, const PropertyValue& value) {
  if (property_name == "Position") {
    SDL_assert(std::holds_alternative<glm::vec2>(value));
    transform_.SetTranslation(glm::vec3(std::get<glm::vec2>(value), 0.0));
  } else if (property_name == "Rotation") {
    SDL_assert(std::holds_alternative<float>(value));
    transform_.SetRotation(glm::quat(1.0f, 0.0f, 0.0f, std::get<float>(value)));
  } else if (property_name == "Scale") {
    SDL_assert(std::holds_alternative<glm::vec2>(value));
    transform_.SetScale(glm::vec3(std::get<glm::vec2>(value), 1.0));
  }
}

}  // namespace ovis