#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <ovis/core/log.hpp>

#include <ovis/engine/scene_object_component.hpp>

namespace ovis {

nlohmann::json SceneObjectComponent::Serialize() const {
  nlohmann::json document = nlohmann::json::object();

  const auto property_names = GetPropertyNames();
  for (const auto& property_name : property_names) {
    auto type = GetPropertyType(property_name);

    // switch (type) {
    //   case PropertyType::BOOLEAN:
    //     SDL_assert(std::holds_alternative<bool>(GetProperty(property_name)));
    //     document[property_name] = std::get<bool>(GetProperty(property_name));
    //     break;

    //   case PropertyType::STRING:
    //     SDL_assert(std::holds_alternative<std::string>(GetProperty(property_name)));
    //     document[property_name] =
    //         std::get<std::string>(GetProperty(property_name));
    //     break;

    //   case PropertyType::INTEGER:
    //     SDL_assert(std::holds_alternative<int>(GetProperty(property_name)));
    //     document[property_name] = std::get<int>(GetProperty(property_name));
    //     break;

    //   case PropertyType::FLOAT:
    //     SDL_assert(std::holds_alternative<float>(GetProperty(property_name)));
    //     document[property_name] = std::get<float>(GetProperty(property_name));
    //     break;

    //   case PropertyType::VECTOR2: {
    //     SDL_assert(std::holds_alternative<glm::vec2>(GetProperty(property_name)));
    //     const auto value = std::get<glm::vec2>(GetProperty(property_name));
    //     document[property_name] = {value.x, value.y};
    //     break;
    //   }

    //   case PropertyType::VECTOR3: {
    //     SDL_assert(std::holds_alternative<glm::vec3>(GetProperty(property_name)));
    //     const auto value = std::get<glm::vec3>(GetProperty(property_name));
    //     document[property_name] = {value.x, value.y, value.z};
    //     break;
    //   }

    //   case PropertyType::VECTOR4: {
    //     SDL_assert(std::holds_alternative<glm::vec4>(GetProperty(property_name)));
    //     const auto value = std::get<glm::vec4>(GetProperty(property_name));
    //     document[property_name] = {value.x, value.y, value.z, value.w};
    //     break;
    //   }

    //   case PropertyType::UNDEFINED:
    //     [[fallthrough]];
    //   default:
    //     break;
    // }
  }

  return document;
}

void SceneObjectComponent::Deserialize(const nlohmann::json& data) {
  for (const auto& property_name : GetPropertyNames()) {
    auto type = GetPropertyType(property_name);
    const auto& value = data[property_name];

    switch (type) {
      case PropertyType::BOOLEAN:
        if (value.is_boolean()) {
          SetProperty(property_name, value.get<bool>());
        } else {
          LogE("Invalid property type, expected boolean, got {}", value);
        }
        break;

      case PropertyType::STRING:
        if (value.is_string()) {
          SetProperty(property_name, value.get<std::string>());
        } else {
          LogE("Invalid property type, expected string, got {}", value);
        }
        break;

      case PropertyType::INTEGER:
        if (value.is_number()) {
          SetProperty(property_name, value.get<int>());
        } else {
          LogE("Invalid property type, expected integer, got {}", value);
        }
        break;

      case PropertyType::FLOAT:
        if (value.is_number()) {
          SetProperty(property_name, value.get<float>());
        } else {
          LogE("Invalid property type, expected integer, got {}", value);
        }
        break;

      case PropertyType::VECTOR2:
        if (value.is_array() && value.size() == 2) {
          SetProperty(property_name, glm::vec2(value[0], value[1]));
        } else {
          LogE(
              "Invalid property type, expected float array of length 2, got {}",
              value);
        }
        break;

      case PropertyType::VECTOR3:
        if (value.is_array() && value.size() == 3) {
          SetProperty(property_name, glm::vec3(value[0], value[1], value[2]));
        } else {
          LogE(
              "Invalid property type, expected float array of length 3, got {}",
              value);
        }
        break;

      case PropertyType::VECTOR4:
        if (value.is_array() && value.size() == 4) {
          SetProperty(property_name,
                      glm::vec4(value[0], value[1], value[2], value[3]));
        } else {
          LogE(
              "Invalid property type, expected float array of length 4, got {}",
              value);
        }
        break;

      case PropertyType::UNDEFINED:
        [[fallthrough]];
      default:
        break;
    }
  }
}

void SceneObjectComponent::DrawPropertyEditor() {
  for (const auto& property_name : GetPropertyNames()) {
    auto type = GetPropertyType(property_name);

    switch (type) {
      case PropertyType::BOOLEAN: {
        bool value = std::get<bool>(GetProperty(property_name));
        if (ImGui::Checkbox(property_name.c_str(), &value)) {
          SetProperty(property_name, value);
        }
        break;
      }

      case PropertyType::STRING: {
        ImGui::Text("%s", property_name.c_str());
        break;
      }

      case PropertyType::INTEGER: {
        ImGui::Text("%s", property_name.c_str());
        break;
      }

      case PropertyType::FLOAT: {
        float value = std::get<float>(GetProperty(property_name));
        if (ImGui::InputFloat(property_name.c_str(), &value)) {
          SetProperty(property_name, value);
        }
        break;
      }

      case PropertyType::VECTOR2: {
        glm::vec2 value = std::get<glm::vec2>(GetProperty(property_name));
        if (ImGui::InputFloat2(property_name.c_str(), glm::value_ptr(value))) {
          SetProperty(property_name, value);
        }
        break;
      }

      case PropertyType::VECTOR3: {
        glm::vec3 value = std::get<glm::vec3>(GetProperty(property_name));
        if (ImGui::InputFloat3(property_name.c_str(), glm::value_ptr(value))) {
          SetProperty(property_name, value);
        }
        break;
      }

      case PropertyType::VECTOR4: {
        glm::vec4 value = std::get<glm::vec4>(GetProperty(property_name));
        if (ImGui::InputFloat4(property_name.c_str(), glm::value_ptr(value))) {
          SetProperty(property_name, value);
        }
        break;
      }

      case PropertyType::UNDEFINED:
        [[fallthrough]];
      default:
        ImGui::Text("%s", property_name.c_str());
    }
  }
}

std::vector<std::string> SceneObjectComponent::GetRegisteredComponents() {
  std::vector<std::string> component_ids;
  component_ids.reserve(factories()->size());
  for (const auto& component_factory : *factories()) {
    component_ids.push_back(component_factory.first);
  }
  return component_ids;
}

std::unordered_map<std::string, Module*>* SceneObjectComponent::factories() {
  static auto factories = new std::unordered_map<std::string, Module*>();
  return factories;
}

}  // namespace ovis
