#include <ovis/scripting/script_scene_controller.hpp>

namespace ovis {

ScriptSceneController::ScriptSceneController(const std::string& name, sol::state* lua) : SceneController(name) {
  SDL_assert(lua != nullptr);
  sol::table object = (*lua)[name];
  sol::protected_function new_function = object["new"];
  instance_ = new_function.call(object);
}

void ScriptSceneController::Update(std::chrono::microseconds delta_time) {
  instance_["Update"](instance_, delta_time.count() / 1000000.0);
}

}  // namespace ovis