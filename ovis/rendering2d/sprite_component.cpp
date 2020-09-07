#include <SDL_assert.h>
#include <ovis/rendering2d/sprite_component.hpp>

namespace ovis {

std::vector<std::string> SpriteComponent::GetPropertyNames() const {
  return {"Size", "Color", "Texture"};
}

SceneObjectComponent::PropertyType SpriteComponent::GetPropertyType(const std::string& property_name) const {
  if (property_name == "Size") {
    return PropertyType::VECTOR2;
  } else if (property_name == "Color") {
    return PropertyType::COLOR;
  } else if (property_name == "Texture") {
    return PropertyType::STRING;
  } else {
    return PropertyType::UNDEFINED;
  }
}

SceneObjectComponent::PropertyValue SpriteComponent::GetProperty(const std::string& property_name) const {
  if (property_name == "Size") {
    return size_;
  } else if (property_name == "Color") {
    return color_;
  } else if (property_name == "Texture") {
    return std::string{};
  } else {
    return std::monostate{};
  }
}

void SpriteComponent::SetProperty(const std::string& property_name, const PropertyValue& value) {
  if (property_name == "Size") {
    SDL_assert(std::holds_alternative<glm::vec2>(value));
    size_ = std::get<glm::vec2>(value);
  } else if (property_name == "Color") {
    SDL_assert(std::holds_alternative<glm::vec4>(value));
    color_ = std::get<glm::vec4>(value);
  } else if (property_name == "Texture") {
  }
}

}  // namespace ovis