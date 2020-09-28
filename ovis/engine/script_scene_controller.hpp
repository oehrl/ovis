#pragma once

#include <sol/sol.hpp>

#include <ovis/engine/scene_controller.hpp>

namespace ovis {

class ScriptSceneController : public SceneController {
 public:
  ScriptSceneController(const std::string& name);

  void Play() override;
  void Stop() override;
  void Update(std::chrono::microseconds delta_time) override;

 private:
  sol::table instance_;
  sol::protected_function update_function_;
};

}  // namespace ovis
