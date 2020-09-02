#include <algorithm>

#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>
#include <ovis/engine/module.hpp>
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

SceneController::~SceneController() {}

void SceneController::Update(std::chrono::microseconds /*delta_time*/) {}

bool SceneController::ProcessEvent(const SDL_Event& /*event*/) {
  return false;
}

std::vector<std::string> SceneController::GetRegisteredControllers() {
  std::vector<std::string> registered_controllers;
  registered_controllers.reserve(Module::scene_controller_factory_functions()->size());
  for (const auto& controller_factory : *Module::scene_controller_factory_functions()) {
    registered_controllers.push_back(controller_factory.first);
  }
  return registered_controllers;
}

void SceneController::UpdateBefore(const std::string& controller_name) {
  update_before_list_.insert(controller_name);
}

void SceneController::UpdateAfter(const std::string& controller_name) {
  update_after_list_.insert(controller_name);
}

}  // namespace ovis
