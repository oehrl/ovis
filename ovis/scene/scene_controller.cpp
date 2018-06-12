#include <algorithm>
#include <SDL_assert.h>
#include <ovis/core/log.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_controller.hpp>

namespace ovis {

SceneController::SceneController(Scene* scene, const std::string& name)
    : m_scene(scene), m_name(name) {
  SDL_assert(scene != nullptr);
  scene->AddController(this);
  LogD("Controller '", name, "' added to scene '", scene->name(), "'");
}

SceneController::~SceneController() {
  scene()->RemoveController(this);
  LogD("Controller '", name(), "' removed from scene '", scene()->name(), "'");
}

void SceneController::Update(std::chrono::microseconds /*delta_time*/) {}

bool SceneController::ProcessEvent(const SDL_Event& /*event*/) {
  return false;
}

}  // namespace ovis
