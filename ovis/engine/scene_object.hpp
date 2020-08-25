#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <nlohmann/json.hh>

#include <ovis/core/class.hpp>

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
  SceneObjectComponent* GetComponent(const std::string& component_id);
  void GetComponentIds(std::vector<std::string>* component_ids) const;
  inline std::vector<std::string> GetComponentIds() const {
    std::vector<std::string> component_ids;
    GetComponentIds(&component_ids);
    return component_ids;
  }
  void RemoveComponent(const std::string& component_id);

  nlohmann::json Serialize() const;
  void Deserialize(const nlohmann::json& serialized_object);

 private:
  Scene* scene_;
  std::string name_;
  std::unordered_map<std::string, std::unique_ptr<SceneObjectComponent>>
      components_;
};

}  // namespace ovis
