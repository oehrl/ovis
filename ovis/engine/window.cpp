#include <cassert>

#include <algorithm>

#include <ovis/core/log.hpp>
#include <ovis/core/profiling.hpp>

#include <ovis/graphics/cubemap.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/texture2d.hpp>

#include <ovis/engine/scene.hpp>
#include <ovis/engine/window.hpp>

namespace ovis {

std::vector<Window*> Window::all_windows_;

Window::Window(const std::string& title, int width, int height)
    : sdl_window_(SDL_CreateWindow(
          title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
          width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI)),
      id_(SDL_GetWindowID(sdl_window_)),
      graphics_context_(sdl_window_),
      render_pipeline_(&graphics_context_, &resource_manager_, {}),
      scene_(this),
      profiling_log_(ProfilingLog::default_log()) {
  assert(sdl_window_ != nullptr);
  all_windows_.push_back(this);
  SDL_GetWindowSize(sdl_window_, &width_, &height_);

  resource_manager_.RegisterFileLoader(
      ".texture2d", std::bind(&ovis::LoadTexture2D, &graphics_context_,
                              std::placeholders::_1, std::placeholders::_2,
                              std::placeholders::_3, std::placeholders::_4));

  resource_manager_.RegisterFileLoader(
      ".cubemap", std::bind(&ovis::LoadCubemap, &graphics_context_,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3, std::placeholders::_4));

  resource_manager_.RegisterFileLoader(
      ".shader", std::bind(&ovis::LoadShaderProgram, &graphics_context_,
                           std::placeholders::_1, std::placeholders::_2,
                           std::placeholders::_3, std::placeholders::_4));
}

Window::~Window() {
  auto it = std::find(all_windows_.begin(), all_windows_.end(), this);
  assert(it != all_windows_.end());
  std::swap(*it, all_windows_.back());
  all_windows_.pop_back();

  SDL_DestroyWindow(sdl_window_);
}

bool Window::SendEvent(const SDL_Event& event) {
  if (event.type == SDL_WINDOWEVENT) {
    switch (event.window.event) {
      case SDL_WINDOWEVENT_CLOSE:
        is_open_ = false;
        break;

      case SDL_WINDOWEVENT_RESIZED:
        width_ = event.window.data1;
        height_ = event.window.data2;
        break;
    }
  }

  return scene_.ProcessEvent(event);
}

void Window::Update(std::chrono::microseconds delta_time) {
  scene_.BeforeUpdate();
  render_pipeline_.DrawDebugUI();
  scene_.Update(delta_time);
  scene_.AfterUpdate();
}

void Window::Render() {
  render_pipeline_.Render(&scene_);
  SDL_GL_SwapWindow(sdl_window_);
  profiling_log_->AdvanceFrame();
}

}  // namespace ovis