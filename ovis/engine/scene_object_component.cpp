#include <ovis/engine/scene_object_component.hpp>

namespace ovis {

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
