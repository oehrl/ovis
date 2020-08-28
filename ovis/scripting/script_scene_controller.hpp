#pragma once

#include <sol/sol.hpp>

#include <ovis/engine/scene_controller.hpp>

namespace ovis {

class ScriptSceneController : public SceneController {
 public:
  ScriptSceneController(const std::string& name, sol::state* lua);

  void Update(std::chrono::microseconds delta_time) override;

 private:
  sol::table instance_;
};

}  // namespace ovis
