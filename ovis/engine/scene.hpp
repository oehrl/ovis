#pragma once

#include <chrono>

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL_events.h>

#include <glm/vec2.hpp>

#include <ovis/core/down_cast.hpp>

#include <ovis/engine/camera.hpp>

namespace ovis {

class SceneController;
class SceneObject;
class GraphicsContext;
class ResourceManager;
class Window;

class Scene {
  friend class SceneController;
  friend class SceneObject;

 public:
  Scene(Window* window);
  virtual ~Scene();

  inline bool is_paused() const { return m_is_paused; }
  inline Camera& camera() { return camera_; }
  inline Window* window() { return window_; }

  inline ResourceManager* resource_manager() const { return resource_manager_; }
  inline void SetResourceManager(ResourceManager* resource_manager) {
    resource_manager_ = resource_manager;
  }

  void AddRegisteredControllers();
  void AddController(const std::string& id);
  void RemoveController(const std::string& id);

  template <typename ControllerType = SceneController>
  inline ControllerType* GetController(
      const std::string& controller_name) const {
    static_assert(std::is_base_of<SceneController, ControllerType>::value, "");
    return down_cast<ControllerType*>(GetControllerInternal(controller_name));
  }

  SceneObject* CreateObject(const std::string& object_name);
  void DeleteObject(const std::string& object_name);
  SceneObject* GetObject(const std::string& object_name);

  template <typename ComponentType>
  void GetSceneObjectsWithComponent(
      std::vector<SceneObject*>* scene_objects) const;

  template <typename ComponentType>
  inline std::vector<SceneObject*> GetSceneObjectsWithComponent() const {
    std::vector<SceneObject*> scene_objects;
    GetSceneObjectsWithComponent<ComponentType>(&scene_objects);
    return scene_objects;
  }

  void BeforeUpdate();
  void AfterUpdate();
  void Update(std::chrono::microseconds delta_time);

  void Resume();
  void Pause();

  bool ProcessEvent(const SDL_Event& event);

 private:
  void AddObject(SceneObject* object);
  void RemoveObject(SceneObject* object);

  SceneController* GetControllerInternal(
      const std::string& controller_name) const;

  virtual bool BeforeEventProcessing(const SDL_Event& /*event*/) {
    return false;
  }
  virtual bool AfterEventProcessing(const SDL_Event& /*event*/) {
    return false;
  }

  std::unordered_map<std::string, std::unique_ptr<SceneController>> m_controllers;
  std::unordered_map<std::string, SceneObject*> objects_;
  std::unordered_map<std::string, std::unique_ptr<SceneObject>>
      created_objects_;
  ResourceManager* resource_manager_;
  Window* window_;
  Camera camera_;
  bool m_is_paused;
};

}  // namespace ovis

#include <ovis/engine/scene_object.hpp>

namespace ovis {

template <typename ComponentType>
inline void Scene::GetSceneObjectsWithComponent(
    std::vector<SceneObject*>* scene_objects) const {
  SDL_assert(scene_objects != nullptr);
  scene_objects->clear();

  for (auto& name_object_pair : objects_) {
    SceneObject* scene_object = name_object_pair.second;
    if (scene_object->HasComponent<ComponentType>()) {
      scene_objects->push_back(scene_object);
    }
  }
}

}  // namespace ovis
