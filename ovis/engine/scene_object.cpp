#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>
#include <ovis/engine/module.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_object.hpp>

namespace ovis {

SceneObject::SceneObject(Scene* scene, const std::string& name) : scene_(scene), name_(name) {
  SDL_assert(scene_ != nullptr);
}

SceneObject::~SceneObject() {}

SceneObjectComponent* SceneObject::AddComponent(const std::string& component_id) {
  if (HasComponent(component_id)) {
    LogE("Object '{}' already has the component '{}'.", name_, component_id);
    return nullptr;
  } else {
    auto factory = Module::scene_object_component_factory_functions()->find(component_id);
    if (factory == Module::scene_object_component_factory_functions()->end()) {
      LogE("Component not registered: '{}'", component_id);
      return nullptr;
    } else {
      auto component = factory->second(this);
      SDL_assert(component != nullptr);
      return components_.insert(std::make_pair(component_id, std::move(component))).first->second.get();
    }
  }
}

bool SceneObject::HasComponent(const std::string& component_id) const {
  return components_.count(component_id) != 0;
}

SceneObjectComponent* SceneObject::GetComponent(const std::string& component_id) {
  auto component = components_.find(component_id);
  if (component == components_.end()) {
    return nullptr;
  } else {
    return component->second.get();
  }
}

void SceneObject::GetComponentIds(std::vector<std::string>* component_ids) const {
  SDL_assert(component_ids != nullptr);

  component_ids->clear();
  component_ids->reserve(components_.size());
  for (const auto& component : components_) {
    component_ids->push_back(component.first);
  }
}

void SceneObject::RemoveComponent(const std::string& component_id) {
  if (components_.erase(component_id) == 0) {
    LogE("Failed to erase component '{}' from '{}': component does not exist.");
  }
}

json SceneObject::Serialize() const {
  json serialized_object = json::object();
  for (const auto& component : components_) {
    serialized_object["components"][component.first] = json::object();
  }
  return serialized_object;
}

void SceneObject::Deserialize(const json& serialized_object) {
  SDL_assert(serialized_object.is_object());
  for (const auto& component : serialized_object) {
    SDL_assert(component.is_string());
  }
}

}  // namespace ovis
