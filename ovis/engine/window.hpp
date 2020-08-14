#pragma once

#include <chrono>

#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL_video.h>

#include <ovis/core/class.hpp>
#include <ovis/core/resource_manager.hpp>

#include <ovis/graphics/graphics_context.hpp>

#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/render_pipeline.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>

namespace ovis {

class GraphicsDevice;
class ProfilingLog;

struct WindowDescription {
  std::string title;
  int width = 1280;
  int height = 720;
  std::vector<std::string> resource_search_paths;
  std::vector<std::string> render_passes = RenderPass::GetRegisteredRenderPasses();
  std::vector<std::string> scene_controllers =
      SceneController::GetRegisteredControllers();
};

class Window {
  MAKE_NON_COPY_OR_MOVABLE(Window);

 public:
  Window(const WindowDescription& description);
  ~Window();

  inline static const std::vector<Window*>& all_windows() {
    return all_windows_;
  }
  inline static Window* GetWindowById(Uint32 id) {
    for (auto window : all_windows()) {
      if (window->id() == id) {
        return window;
      }
    }
    return nullptr;
  }
  inline SDL_Window* sdl_window() const { return sdl_window_; }
  inline Uint32 id() const { return id_; }
  inline bool is_open() const { return is_open_; }
  inline GraphicsContext* context() { return &graphics_context_; }
  inline ResourceManager* resource_manager() { return &resource_manager_; }
  inline RenderPipeline* render_pipeline() { return &render_pipeline_; }
  inline Scene* scene() { return &scene_; }
  inline int width() const { return width_; }
  inline int height() const { return height_; }

  bool SendEvent(const SDL_Event& event);
  void Update(std::chrono::microseconds delta_time);
  void Render();

 private:
  static std::vector<Window*> all_windows_;
  SDL_Window* sdl_window_;
  Uint32 id_;
  bool is_open_ = true;
  int width_;
  int height_;

  GraphicsContext graphics_context_;
  RenderPipeline render_pipeline_;
  ResourceManager resource_manager_;
  Scene scene_;
  ProfilingLog* profiling_log_;
};

}  // namespace ovis