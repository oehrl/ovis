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
  key_down_function_ = instance_["OnKeyDown"];
  key_up_function_ = instance_["OnKeyUp"];
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
  if (update_function_ != sol::lua_nil) {
    sol::protected_function_result pfr = update_function_.call(instance_, delta_time.count() / 1000000.0);
  }
}

bool ScriptSceneController::ProcessEvent(const SDL_Event& event) {
  auto key_to_string = [](SDL_Scancode scancode) {
    // clang-format off
    switch (scancode) {
      case SDL_SCANCODE_0: return "0";
      case SDL_SCANCODE_1: return "1";
      case SDL_SCANCODE_2: return "2";
      case SDL_SCANCODE_3: return "3";
      case SDL_SCANCODE_4: return "4";
      case SDL_SCANCODE_5: return "5";
      case SDL_SCANCODE_6: return "6";
      case SDL_SCANCODE_7: return "7";
      case SDL_SCANCODE_8: return "8";
      case SDL_SCANCODE_9: return "9";
      
      case SDL_SCANCODE_A: return "A";
      case SDL_SCANCODE_B: return "B";
      case SDL_SCANCODE_C: return "C";
      case SDL_SCANCODE_D: return "D";
      case SDL_SCANCODE_E: return "E";
      case SDL_SCANCODE_F: return "F";
      case SDL_SCANCODE_G: return "G";
      case SDL_SCANCODE_H: return "H";
      case SDL_SCANCODE_I: return "I";
      case SDL_SCANCODE_J: return "J";
      case SDL_SCANCODE_K: return "K";
      case SDL_SCANCODE_L: return "L";
      case SDL_SCANCODE_M: return "M";
      case SDL_SCANCODE_N: return "N";
      case SDL_SCANCODE_O: return "O";
      case SDL_SCANCODE_P: return "P";
      case SDL_SCANCODE_Q: return "Q";
      case SDL_SCANCODE_R: return "R";
      case SDL_SCANCODE_S: return "S";
      case SDL_SCANCODE_T: return "T";
      case SDL_SCANCODE_U: return "U";
      case SDL_SCANCODE_V: return "V";
      case SDL_SCANCODE_W: return "W";
      case SDL_SCANCODE_X: return "X";
      case SDL_SCANCODE_Y: return "Y";
      case SDL_SCANCODE_Z: return "Z";
      
      case SDL_SCANCODE_TAB: return "Tab";
      case SDL_SCANCODE_LCTRL: return "ControlLeft";
      case SDL_SCANCODE_RCTRL: return "ControlRight";
      case SDL_SCANCODE_LSHIFT: return "ShiftLeft";
      case SDL_SCANCODE_RSHIFT: return "ShiftRight";
      case SDL_SCANCODE_LALT: return "AltLeft";
      case SDL_SCANCODE_RALT: return "AltRight";
      case SDL_SCANCODE_LGUI: return "MetaLeft";
      case SDL_SCANCODE_RGUI: return "MetaRight";
      case SDL_SCANCODE_SPACE: return "Space";

      default: return "";
    }
    // clang-format on
  };

  if (event.type == SDL_KEYDOWN && key_down_function_) {
    sol::table event_data = Lua::state.create_table();
    event_data["key"] = key_to_string(event.key.keysym.scancode);
    event_data["repeat"] = event.key.repeat;
    sol::protected_function_result pfr = key_down_function_(instance_, event_data);
    return true;
  } else if (event.type == SDL_KEYUP && key_up_function_) {
    sol::table event_data = Lua::state.create_table();
    event_data["key"] = key_to_string(event.key.keysym.scancode);
    event_data["repeat"] = event.key.repeat;
    sol::protected_function_result pfr = key_up_function_(instance_, event_data);
    return true;
  } else {
    return false;
  }
}

}  // namespace ovis