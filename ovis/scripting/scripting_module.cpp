#include <ovis/scripting/scripting_module.hpp>

namespace ovis {

ScriptingModule::ScriptingModule() : Module("ScriptingModule") {
}

ScriptingModule::~ScriptingModule() {
}

std::unique_ptr<SceneController> ScriptingModule::CreateSceneController(
    const std::string& scene_controller_id, Scene*) {
  if (scene_controller_id == "") {
    return nullptr;
  } else {
    return nullptr;
  }
}

}  // namespace ovis
