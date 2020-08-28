#include <ovis/core/log.hpp>
#include <ovis/scripting/scripting_module.hpp>

namespace ovis {

ScriptingModule::ScriptingModule() : Module("ScriptingModule") {
  sol_state_["LogE"] = [](const std::string& message) { LogE("{}", message); };
  sol_state_["LogW"] = [](const std::string& message) { LogW("{}", message); };
  sol_state_["LogI"] = [](const std::string& message) { LogI("{}", message); };
  sol_state_["LogD"] = [](const std::string& message) { LogD("{}", message); };
  sol_state_["LogV"] = [](const std::string& message) { LogV("{}", message); };

  sol_state_.script("LogD('test')");
}

ScriptingModule::~ScriptingModule() {}

std::unique_ptr<SceneController> ScriptingModule::CreateSceneController(const std::string& scene_controller_id,
                                                                        Scene*) {
  if (scene_controller_id == "") {
    return nullptr;
  } else {
    return nullptr;
  }
}

}  // namespace ovis
