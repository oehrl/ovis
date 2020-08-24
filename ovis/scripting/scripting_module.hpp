#pragma once

#include <lua.hpp>

#include <ovis/engine/module.hpp>

namespace ovis {

class ScriptingModule final : public Module {
 public:
  ScriptingModule();
  ~ScriptingModule() override;

  std::unique_ptr<SceneController> CreateSceneController(
      const std::string& scene_controller_id, Scene* scene) override;

 private:
  lua_State* lua_state_; 
};

}  // namespace ovis