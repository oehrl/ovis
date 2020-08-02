#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>
#include <ovis/core/down_cast.hpp>
#include <ovis/scene/camera.hpp>

namespace ovis {

class SceneController;
class SceneObject;
class GraphicsContext;
class ResourceManager;

class Scene {
  friend class SceneController;
  friend class SceneObject;

 public:
  Scene(const std::string& name, const glm::uvec2& size,
        bool hide_previous = true);
  virtual ~Scene();

  inline const std::string& name() const { return m_name; }
  inline bool is_paused() const { return m_is_paused; }
  inline bool hides_previous() const { return m_hides_previous; }
  inline Camera& camera() { return camera_; }
  inline void Resize(const glm::uvec2& new_size);
  inline glm::uvec2 size() const { return size_; }

  inline ResourceManager* resource_manager() const { return resource_manager_; }
  inline void SetResourceManager(ResourceManager* resource_manager) {
    resource_manager_ = resource_manager;
  }

  template <typename ControllerType = SceneController>
  inline ControllerType* GetController(
      const std::string& controller_name) const {
    static_assert(std::is_base_of<SceneController, ControllerType>::value, "");
    return down_cast<ControllerType*>(GetControllerInternal(controller_name));
  }

  SceneObject* CreateObject(const std::string& object_name);
  void DeleteObject(const std::string& object_name);
  SceneObject* GetObject(const std::string& object_name);

  void Update(std::chrono::microseconds delta_time);

  void Resume();
  void Pause();

  bool ProcessEvent(const SDL_Event& event);

 private:
  void AddController(SceneController* controller);
  void RemoveController(SceneController* controller);

  void AddObject(SceneObject* object);
  void RemoveObject(SceneObject* object);

  SceneController* GetControllerInternal(
      const std::string& controller_name) const;

  virtual void OnUpdate(std::chrono::microseconds delta_time);
  virtual void OnResume();
  virtual void OnPause();
  virtual bool BeforeEventProcessing(const SDL_Event& /*event*/) {
    return false;
  }
  virtual bool AfterEventProcessing(const SDL_Event& /*event*/) {
    return false;
  }

  std::string m_name;
  std::unordered_map<std::string, SceneController*> m_controllers;
  std::unordered_map<std::string, SceneObject*> objects_;
  std::unordered_map<std::string, std::unique_ptr<SceneObject>> created_objects_;
  ResourceManager* resource_manager_;
  Camera camera_;
  glm::uvec2 size_;
  bool m_is_paused;
  bool m_hides_previous;
};

}  // namespace ovis
