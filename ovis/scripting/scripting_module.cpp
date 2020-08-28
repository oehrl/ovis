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

  lua_.script(
      "ExampleController = class(\"ExampleController\")\nfunction "
      "ExampleController:Update(delta_time) LogI(\"Hello from lua dt=\" .. delta_time) "
      "end");

  // RegisterSceneController("ExampleController");
}

ScriptingModule::~ScriptingModule() {}

std::unique_ptr<SceneController> ScriptingModule::CreateSceneController(const std::string& scene_controller_id,
                                                                        Scene*) {
  if (scene_controller_id == "ExampleController") {
    return std::make_unique<ScriptSceneController>(scene_controller_id, &lua_);
  } else {
    return nullptr;
  }
}

}  // namespace ovis
