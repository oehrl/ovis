#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <SDL_video.h>
#include <ovis/core/class.hpp>
#include <ovis/core/resource_manager.hpp>
#include <ovis/graphics/graphics_context.hpp>

namespace ovis {

class Scene;
class GraphicsDevice;

class Window {
  MAKE_NON_COPY_OR_MOVABLE(Window);

 public:
  Window(const std::string& title, int width = 1280, int height = 720);
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

  bool SendEvent(const SDL_Event& event);
  void Update(std::chrono::microseconds delta_time);
  void Render();

  void PushScene(Scene* scene);
  void ReplaceScene(Scene* scene);
  void PopScene();

 private:
  static std::vector<Window*> all_windows_;
  SDL_Window* sdl_window_;
  Uint32 id_;
  bool is_open_ = true;
  int width_;
  int height_;

  ResourceManager resource_manager_;
  GraphicsContext graphics_context_;

  std::vector<Scene*> scene_stack_;
};

}  // namespace ovis