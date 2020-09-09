#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <ovis/core/class.hpp>
#include <ovis/core/json.hpp>
#include <ovis/engine/scene_object_component.hpp>

namespace ovis {

class Scene;

class SceneObject {
  MAKE_NON_COPYABLE(SceneObject);

 public:
  SceneObject(Scene* scene, const std::string& name);
  ~SceneObject();

  inline std::string name() const { return name_; }

  SceneObjectComponent* AddComponent(const std::string& component_id);
  bool HasComponent(const std::string& component_id) const;

  template <typename ComponentType = SceneObjectComponent>
  ComponentType* GetComponent(const std::string& component_id) {
    auto component = components_.find(component_id);
    if (component == components_.end()) {
      return nullptr;
    } else {
      return down_cast<ComponentType*>(component->second.get());
    }
  }

  void GetComponentIds(std::vector<std::string>* component_ids) const;
  inline std::vector<std::string> GetComponentIds() const {
    std::vector<std::string> component_ids;
    GetComponentIds(&component_ids);
    return component_ids;
  }
  void RemoveComponent(const std::string& component_id);

  json Serialize() const;
  void Deserialize(const json& serialized_object);

  static void RegisterToLua();

 private:
  Scene* scene_;
  std::string name_;
  std::unordered_map<std::string, std::unique_ptr<SceneObjectComponent>> components_;
};

}  // namespace ovis
