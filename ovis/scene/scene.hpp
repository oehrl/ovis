#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL_events.h>
#include <ovis/core/down_cast.hpp>
#include <ovis/scene/camera.hpp>

namespace ovis {

class SceneController;
class SceneRenderer;
class GraphicsContext;

class Scene {
  friend class SceneController;
  friend class SceneRenderer;

 public:
  Scene(const std::string& name, bool hide_previous = true);
  virtual ~Scene();

  inline const std::string& name() const { return m_name; }
  inline bool is_paused() const { return m_is_paused; }
  inline bool hides_previous() const { return m_hides_previous; }
  inline Camera& camera() { return camera_; }

  inline GraphicsContext* context() const { return context_; }
  void SetContext(GraphicsContext* context);

  template <typename ControllerType = SceneController>
  inline ControllerType* GetController(
      const std::string& controller_name) const {
    static_assert(std::is_base_of<SceneController, ControllerType>::value, "");
    return down_cast<ControllerType*>(GetControllerInternal(controller_name));
  }

  template <typename RendererType = SceneRenderer>
  inline RendererType* GetRenderer(const std::string& renderer_name) const {
    static_assert(std::is_base_of<SceneRenderer, RendererType>::value, "");
    return down_cast<RendererType*>(GetRendererInternal(renderer_name));
  }

  void Update(std::chrono::microseconds delta_time);
  void Render(float width, float height);

  void Resume();
  void Pause();

  bool ProcessEvent(const SDL_Event& event);

 private:
  void AddController(SceneController* controller);
  void RemoveController(SceneController* controller);
  void AddRenderer(SceneRenderer* renderer);
  void RemoveRenderer(SceneRenderer* renderer);
  void SortRenderers();

  SceneController* GetControllerInternal(
      const std::string& controller_name) const;
  SceneRenderer* GetRendererInternal(const std::string& renderer_name) const;

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
  std::unordered_map<std::string, SceneRenderer*> m_renderers;
  std::vector<SceneRenderer*> m_render_order;
  GraphicsContext* context_;
  Camera camera_;
  bool m_renderers_sorted;
  bool m_is_paused;
  bool m_hides_previous;
};

}  // namespace ovis
