#include <algorithm>

#include <SDL2/SDL_assert.h>

#include <ovis/core/log.hpp>

#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>

namespace ovis {

SceneController::SceneController(Scene* scene, const std::string& name)
    : m_scene(scene),
      m_name(name)
#if OVIS_ENABLE_BUILT_IN_PROFILING
      , update_profiler_(name + "Update")
#endif
{
  SDL_assert(scene != nullptr);
  scene->AddController(this);
  LogD("Controller '{}' added to scene '{}'", name, scene->name());
}

SceneController::~SceneController() {
  scene()->RemoveController(this);
  LogD("Controller '{}' remove from scene '{}'", name(), m_scene->name());
}

void SceneController::Update(std::chrono::microseconds /*delta_time*/) {}

bool SceneController::ProcessEvent(const SDL_Event& /*event*/) {
  return false;
}

}  // namespace ovis
