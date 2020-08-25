#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>

#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_object.hpp>

namespace ovis {

SceneObject::SceneObject(Scene* scene, const std::string& name)
    : scene_(scene), name_(name) {
  SDL_assert(scene_ != nullptr);
}

SceneObject::~SceneObject() {}

SceneObjectComponent* SceneObject::AddComponent(
    const std::string& component_id) {
  if (HasComponent(component_id)) {
    LogE("Object '{}' already has the component '{}'.", name_, component_id);
    return nullptr;
  } else {
    auto factory = SceneObjectComponent::factories()->find(component_id);
    if (factory == SceneObjectComponent::factories()->end()) {
      LogE("Component not registered: '{}'", component_id);
      return nullptr;
    } else {
      auto component =
          factory->second->CreateSceneObjectComponent(component_id, this);
      SDL_assert(component != nullptr);
      return components_
          .insert(std::make_pair(component_id, std::move(component)))
          .first->second.get();
    }
  }
}

bool SceneObject::HasComponent(const std::string& component_id) const {
  return components_.count(component_id) != 0;
}

SceneObjectComponent* SceneObject::GetComponent(
    const std::string& component_id) {
  auto component = components_.find(component_id);
  if (component == components_.end()) {
    return nullptr;
  } else {
    return component->second.get();
  }
}

void SceneObject::GetComponents(std::vector<std::string>* component_ids) {
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

nlohmann::json SceneObject::Serialize() const {
  nlohmann::json document = {{"name", name_}};
  return document;
}

}  // namespace ovis
