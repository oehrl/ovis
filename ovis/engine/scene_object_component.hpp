#pragma once

#include <string>
#include <variant>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <ovis/core/class.hpp>
#include <ovis/core/json.hpp>

namespace ovis {

class SceneObjectComponent {
  MAKE_NON_COPY_OR_MOVABLE(SceneObjectComponent);

 public:
  SceneObjectComponent() = default;
  virtual ~SceneObjectComponent() = default;

  enum class PropertyType {
    UNDEFINED,
    BOOLEAN,
    STRING,
    INTEGER,
    FLOAT,
    VECTOR2,
    VECTOR3,
    VECTOR4,
    COLOR,
  };
  using PropertyValue = std::variant<std::monostate, bool, std::string, int, float, glm::vec2, glm::vec3, glm::vec4>;

  virtual std::vector<std::string> GetPropertyNames() const = 0;
  virtual PropertyType GetPropertyType(const std::string& property_name) const = 0;
  virtual PropertyValue GetProperty(const std::string& property_name) const = 0;
  virtual void SetProperty(const std::string& property_name, const PropertyValue& value) = 0;

  virtual json Serialize() const;
  virtual void Deserialize(const json& data);

  virtual bool DrawEditorForProperties();
  virtual bool DrawEditorForProperty(const std::string& property_name);

  static std::vector<std::string> GetRegisteredComponents();
};

}  // namespace ovis
