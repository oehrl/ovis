#include <algorithm>

#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>

#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>

namespace ovis {

SceneController::SceneController(const std::string& name)
    : m_name(name)
#if OVIS_ENABLE_BUILT_IN_PROFILING
      ,
      update_profiler_(name + "Update")
#endif
{
}

SceneController::~SceneController() {
}

void SceneController::Update(std::chrono::microseconds /*delta_time*/) {}

bool SceneController::ProcessEvent(const SDL_Event& /*event*/) {
  return false;
}

std::unordered_map<std::string, Module*>*
SceneController::scene_controller_factories() {
  static auto scene_controller_factories =
      new std::unordered_map<std::string, Module*>();
  return scene_controller_factories;
}

}  // namespace ovis
