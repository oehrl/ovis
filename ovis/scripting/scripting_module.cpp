#include <middleclass.hpp>

#include <ovis/core/log.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/scripting/script_scene_controller.hpp>
#include <ovis/scripting/scripting_module.hpp>

namespace ovis {

ScriptingModule::ScriptingModule() : Module("ScriptingModule") {
  lua_.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string);
  lua_.require_script("class", middleclass::SOURCE);

  lua_["LogE"] = [](const std::string& message) { LogE("{}", message); };
  lua_["LogW"] = [](const std::string& message) { LogW("{}", message); };
  lua_["LogI"] = [](const std::string& message) { LogI("{}", message); };
  lua_["LogD"] = [](const std::string& message) { LogD("{}", message); };
  lua_["LogV"] = [](const std::string& message) { LogV("{}", message); };
}

ScriptingModule::~ScriptingModule() {}

void ScriptingModule::AddSceneController(const std::string& id, const std::string& source) {
  lua_.script(source);
  RegisterSceneController(id, [this, id](Scene*) { return std::make_unique<ScriptSceneController>(id, &lua_); });
  registered_scene_controllers_.insert(id);
}

void ScriptingModule::RemoveSceneController(const std::string& id) {
  if (registered_scene_controllers_.count(id) != 0) {
    lua_[id] = sol::lua_nil;
    registered_scene_controllers_.erase(id);
  } else {
    ovis::LogE("'{}' was not registered by the scripting module.", id);
  }
}

}  // namespace ovis
