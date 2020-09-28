#include <ovis/engine/lua.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/script_scene_controller.hpp>

namespace ovis {

ScriptSceneController::ScriptSceneController(const std::string& name) : SceneController(name) {
  sol::table object = Lua::state[name];
  SDL_assert(object != sol::lua_nil);
  sol::protected_function new_function = object["new"];
  SDL_assert(new_function != sol::lua_nil);
  instance_ = new_function.call(object);
  SDL_assert(instance_ != sol::lua_nil);
  update_function_ = instance_["Update"];
  SDL_assert(update_function_ != sol::lua_nil);
}

void ScriptSceneController::Play() {
  instance_["scene"] = scene();
  if (instance_["Play"] != sol::lua_nil) {
    instance_["Play"](instance_);
  }
}

void ScriptSceneController::Stop() {
  instance_["scene"] = scene();
  if (instance_["Stop"] != sol::lua_nil) {
    instance_["Stop"](instance_);
  }
}

void ScriptSceneController::Update(std::chrono::microseconds delta_time) {
  instance_["scene"] = scene();
  SDL_assert(update_function_ != sol::lua_nil);
  sol::protected_function_result pfr = update_function_.call(instance_, delta_time.count() / 1000000.0);
}

}  // namespace ovis