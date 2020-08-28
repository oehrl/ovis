#pragma once

#include <sol/sol.hpp>

#include <ovis/engine/module.hpp>

namespace ovis {

class ScriptingModule final : public Module {
 public:
  ScriptingModule();
  ~ScriptingModule() override;

  std::unique_ptr<SceneController> CreateSceneController(const std::string& scene_controller_id, Scene* scene) override;

 private:
  sol::state sol_state_;
};

}  // namespace ovis