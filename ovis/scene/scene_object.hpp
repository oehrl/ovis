#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <ovis/core/class.hpp>
#include <ovis/scene/scene_component.hpp>

namespace ovis {

class Scene;

class SceneObject {
  MAKE_NON_COPYABLE(SceneObject);

 public:
  SceneObject(Scene* scene, const std::string& name);
  ~SceneObject();

  inline std::string name() const { return name_; }

  template <typename T, typename... Args>
  T& AddComponent(Args&&... arguments) {
    if (components_.count(typeid(T)) == 0) {
      components_.insert(std::make_pair(
          typeid(T),
          std::make_unique<SceneComponent>(std::move<Args>(arguments)...)));
    } else {
      return nullptr;
    }
  }

  template <typename T>
  void RemoveComponent() {
    components_.erase(typeid(T));
  }

 private:
  Scene* scene_;
  std::string name_;
  std::unordered_map<std::type_index, std::unique_ptr<SceneComponentBase>>
      components_;
};

}  // namespace ovis
