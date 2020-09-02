#pragma once

#include <set>

#include <sol/sol.hpp>

#include <ovis/engine/module.hpp>

namespace ovis {

class ScriptingModule final : public Module {
 public:
  ScriptingModule();
  ~ScriptingModule() override;

  void AddSceneController(const std::string& id, const std::string& source);
  void RemoveSceneController(const std::string& id);

 private:
  sol::state lua_;
  std::set<std::string> registered_scene_controllers_;
};

}  // namespace ovis