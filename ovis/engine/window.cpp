#include <cassert>
#include <algorithm>
#include <ovis/core/log.hpp>
#include <ovis/graphics/cubemap.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/engine/window.hpp>

namespace ovis {

std::vector<Window*> Window::all_windows_;

Window::Window(const std::string& title, int width, int height)
    : sdl_window_(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, width, height,
                                   SDL_WINDOW_OPENGL)),
      id_(SDL_GetWindowID(sdl_window_)),
      graphics_context_(sdl_window_) {
  assert(sdl_window_ != nullptr);
  all_windows_.push_back(this);
  SDL_GetWindowSize(sdl_window_, &width_, &height_);

  resource_manager_.RegisterFileLoader(
      ".cubemap", std::bind(&ovis::LoadCubemap, &graphics_context_,
                            std::placeholders::_1, std::placeholders::_2));

  resource_manager_.RegisterFileLoader(
      ".shader", std::bind(&ovis::LoadShaderProgram, &graphics_context_,
                           std::placeholders::_1, std::placeholders::_2));
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

  if (scene_stack_.size() == 0) {
    return false;
  }

  assert(scene_stack_.back() != nullptr);
  return scene_stack_.back()->ProcessEvent(event);
}

void Window::Update(std::chrono::microseconds delta_time) {
  for (Scene* scene : scene_stack_) {
    scene->Update(delta_time);
  }
}

void Window::Render() {
  if (scene_stack_.size() > 0) {
    scene_stack_.back()->Render(width_, height_);
  }
  SDL_GL_SwapWindow(sdl_window_);
}

void Window::PushScene(Scene* scene) {
  if (scene_stack_.size() > 0) {
    scene_stack_.back()->Pause();
  }
  scene_stack_.push_back(scene);
  scene->SetResourceManager(
      resource_manager());  // must happen before calling set context!
  scene->SetContext(context());
  scene->Resume();
}

void Window::ReplaceScene(Scene* scene) {
  // TODO: optimize so that the scene below the top scene does not get paused
  //       and then immediately resumed
  PopScene();
  PushScene(scene);
}

void Window::PopScene() {
  assert(scene_stack_.size() > 0);

  scene_stack_.back()->Pause();
  scene_stack_.back()->SetContext(nullptr);
  scene_stack_.pop_back();
  if (scene_stack_.size() > 0) {
    scene_stack_.back()->Resume();
  }
}

}  // namespace ovis