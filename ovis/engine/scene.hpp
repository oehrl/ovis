#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_events.h>
#include <glm/vec2.hpp>

#include <ovis/core/down_cast.hpp>
#include <ovis/core/json.hpp>
#include <ovis/engine/camera.hpp>
#include <ovis/engine/scene_object.hpp>

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
  Scene();
  virtual ~Scene();

  inline bool is_playing() const { return is_playing_; }
  inline Camera& camera() { return camera_; }

  inline ResourceManager* resource_manager() const { return resource_manager_; }
  inline void SetResourceManager(ResourceManager* resource_manager) { resource_manager_ = resource_manager; }

  void AddController(const std::string& id);
  void RemoveController(const std::string& id);
  void ClearControllers();

  template <typename ControllerType = SceneController>
  inline ControllerType* GetController(const std::string& controller_name) const {
    static_assert(std::is_base_of<SceneController, ControllerType>::value, "");
    return down_cast<ControllerType*>(GetControllerInternal(controller_name));
  }

  SceneObject* CreateObject(const std::string& object_name);
  SceneObject* CreateObject(const std::string& object_name, const json& serialized_object);
  void DeleteObject(const std::string& object_name);
  void ClearObjects();
  SceneObject* GetObject(const std::string& object_name);
  bool ContainsObject(const std::string& object_name);

  void GetObjects(std::vector<SceneObject*>* scene_objects, bool sort_by_name = false) const;
  inline std::vector<SceneObject*> GetObjects(bool sort_by_name = false) const {
    std::vector<SceneObject*> objects;
    GetObjects(&objects, sort_by_name);
    return objects;
  }

  void GetSceneObjectsWithComponent(const std::string& component_id, std::vector<SceneObject*>* scene_objects) const;

  inline std::vector<SceneObject*> GetSceneObjectsWithComponent(const std::string& component_id) const {
    std::vector<SceneObject*> scene_objects;
    GetSceneObjectsWithComponent(component_id, &scene_objects);
    return scene_objects;
  }

  void Play();
  void Stop();

  void BeforeUpdate();
  void AfterUpdate();
  void Update(std::chrono::microseconds delta_time);

  bool ProcessEvent(const SDL_Event& event);

  void DrawImGui();

  json Serialize() const;
  void Deserialize(const json& serialized_object);

 private:
  void InvalidateControllerOrder();
  void SortControllers();
  SceneController* GetControllerInternal(const std::string& controller_name) const;

  virtual bool BeforeEventProcessing(const SDL_Event& /*event*/) { return false; }
  virtual bool AfterEventProcessing(const SDL_Event& /*event*/) { return false; }

  std::unordered_map<std::string, std::unique_ptr<SceneController>> controllers_;
  std::vector<SceneController*> controller_order_;
  bool controllers_sorted_ = false;

  std::unordered_map<std::string, std::unique_ptr<SceneObject>> objects_;
  ResourceManager* resource_manager_;
  Camera camera_;
  bool is_playing_ = false;
};

}  // namespace ovis
