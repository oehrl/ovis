#include <ovis/scripting/script_scene_controller.hpp>

namespace ovis {

ScriptSceneController::ScriptSceneController(const std::string& name, sol::state* lua) : SceneController(name) {
  SDL_assert(lua != nullptr);
  sol::table object = (*lua)[name];
  SDL_assert(object != sol::lua_nil);
  sol::protected_function new_function = object["new"];
  SDL_assert(new_function != sol::lua_nil);
  instance_ = new_function.call(object);
  SDL_assert(instance_ != sol::lua_nil);
}

void ScriptSceneController::Update(std::chrono::microseconds delta_time) {
  SDL_assert(instance_["Update"] != sol::lua_nil);
  instance_["Update"](instance_, delta_time.count() / 1000000.0);
}

}  // namespace ovis