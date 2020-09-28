#include <middleclass.hpp>

#include <ovis/engine/lua.hpp>
#include <ovis/engine/module.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/engine/script_scene_controller.hpp>

namespace ovis {

sol::state Lua::state;
Event<void(const std::string&)> Lua::on_error;

void Lua::SetupEnvironment() {
  state.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::math);
  state.require_script("class", middleclass::SOURCE);

  state["LogE"] = [](const std::string& message) { LogE("{}", message); };
  state["LogW"] = [](const std::string& message) { LogW("{}", message); };
  state["LogI"] = [](const std::string& message) { LogI("{}", message); };
  state["LogD"] = [](const std::string& message) { LogD("{}", message); };
  state["LogV"] = [](const std::string& message) { LogV("{}", message); };

  state["OvisErrorHandler"] = [](const std::string& message) { on_error.Invoke(message); };
  sol::protected_function::set_default_handler(state["OvisErrorHandler"]);

  sol::usertype<glm::vec2> vector2_type =
      state.new_usertype<glm::vec2>("Vector2", sol::constructors<glm::vec2(), glm::vec2(float, float)>());
  vector2_type["x"] = &glm::vec2::x;
  vector2_type["y"] = &glm::vec2::y;
  vector2_type[sol::meta_function::unary_minus] = [](const glm::vec2& vector) { return -vector; };
  vector2_type[sol::meta_function::addition] = [](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs + rhs; };
  vector2_type[sol::meta_function::subtraction] = [](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs - rhs; };
  vector2_type[sol::meta_function::multiplication] =
      sol::overload([](double lhs, const glm::vec2& rhs) { return static_cast<float>(lhs) * rhs; },
                    [](const glm::vec2& lhs, double rhs) { return lhs * static_cast<float>(rhs); });
  vector2_type[sol::meta_function::division] = [](const glm::vec2& lhs, double rhs) {
    return lhs / static_cast<float>(rhs);
  };
  vector2_type[sol::meta_function::to_string] = [](const glm::vec2& vector) {
    return "(" + std::to_string(vector.x) + "," + std::to_string(vector.y) + ")";
  };
  vector2_type[sol::meta_function::length] = [](const glm::vec2& vector) { return 2; };
  vector2_type["length"] = sol::property([](const glm::vec2& vector) { return glm::length(vector); });
  vector2_type["lengthSq"] = sol::property([](const glm::vec2& vector) { return glm::dot(vector, vector); });

  sol::usertype<glm::vec3> vector3_type =
      state.new_usertype<glm::vec3>("Vector3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>());
  vector3_type["x"] = &glm::vec3::x;
  vector3_type["y"] = &glm::vec3::y;
  vector3_type["z"] = &glm::vec3::z;
  vector3_type[sol::meta_function::unary_minus] = [](const glm::vec3& vector) { return -vector; };
  vector3_type[sol::meta_function::addition] = [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs + rhs; };
  vector3_type[sol::meta_function::subtraction] = [](const glm::vec3& lhs, const glm::vec3& rhs) { return lhs - rhs; };
  vector3_type[sol::meta_function::multiplication] =
      sol::overload([](double lhs, const glm::vec3& rhs) { return static_cast<float>(lhs) * rhs; },
                    [](const glm::vec3& lhs, double rhs) { return lhs * static_cast<float>(rhs); });
  vector3_type[sol::meta_function::division] = [](const glm::vec3& lhs, double rhs) {
    return lhs / static_cast<float>(rhs);
  };
  vector3_type[sol::meta_function::to_string] = [](const glm::vec3& vector) {
    return "(" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "," + std::to_string(vector.z) + ")";
  };
  vector3_type[sol::meta_function::length] = [](const glm::vec3& vector) { return 3; };
  vector3_type["length"] = sol::property([](const glm::vec3& vector) { return glm::length(vector); });
  vector3_type["lengthSq"] = sol::property([](const glm::vec3& vector) { return glm::dot(vector, vector); });

  sol::usertype<glm::vec4> vector4_type =
      state.new_usertype<glm::vec4>("Vector4", sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>());
  vector4_type["x"] = &glm::vec4::x;
  vector4_type["y"] = &glm::vec4::y;
  vector4_type["z"] = &glm::vec4::z;
  vector4_type["w"] = &glm::vec4::w;
  vector4_type[sol::meta_function::unary_minus] = [](const glm::vec4& vector) { return -vector; };
  vector4_type[sol::meta_function::addition] = [](const glm::vec4& lhs, const glm::vec4& rhs) { return lhs + rhs; };
  vector4_type[sol::meta_function::subtraction] = [](const glm::vec4& lhs, const glm::vec4& rhs) { return lhs - rhs; };
  vector4_type[sol::meta_function::multiplication] =
      sol::overload([](double lhs, const glm::vec4& rhs) { return static_cast<float>(lhs) * rhs; },
                    [](const glm::vec4& lhs, double rhs) { return lhs * static_cast<float>(rhs); });
  vector4_type[sol::meta_function::division] = [](const glm::vec4& lhs, double rhs) {
    return lhs / static_cast<float>(rhs);
  };
  vector4_type[sol::meta_function::to_string] = [](const glm::vec4& vector) {
    return "(" + std::to_string(vector.x) + "," + std::to_string(vector.y) + "," + std::to_string(vector.z) + "," +
           std::to_string(vector.w) + ")";
  };
  vector4_type[sol::meta_function::length] = [](const glm::vec4& vector) { return 4; };
  vector4_type["length"] = sol::property([](const glm::vec4& vector) { return glm::length(vector); });
  vector4_type["lengthSq"] = sol::property([](const glm::vec4& vector) { return glm::dot(vector, vector); });

  sol::usertype<Scene> scene_type = state.new_usertype<Scene>("Scene");
  scene_type["CreateObject"] = static_cast<SceneObject* (Scene::*)(const std::string&)>(&Scene::CreateObject);
  scene_type["DeleteObject"] = &Scene::DeleteObject;
  scene_type["GetObject"] = &Scene::GetObject;

  Module::RegisterToLua();
  SceneObject::RegisterToLua();
}

sol::protected_function_result Lua::AddSceneController(const std::string& code, const std::string& id) {
  if (Module::IsSceneControllerRegistered(id)) {
    Module::DeregisterSceneController(id);
  }
  Module::RegisterSceneController(id, [id](Scene*) { return std::make_unique<ScriptSceneController>(id); });
  return state.do_string(code, "=" + id);
}

// bool LoadScript(ResourceManager* resource_manager, const json& parameters, const std::string& id,
//                 const std::string& directory) {
//   if (!parameters.contains("type")) {
//     LogE("Script has no type!");
//     return false;
//   }
//   if (!parameters.contains("type")) {
//     LogE("Script has no type!");
//     return false;
//   }

//   const std::string type = parameters["type"];
//   if (type == "scene_controller") {
//     const std::string source_filename = directory + '/' + id + ".lua";
//     state.script_file(source_filename);
//     auto source_code = LoadTextFile(source_filename);
//     if (!source_code) {
//       LogE("Cannot open source file: '{}'", source_filename);
//       return false;
//     } else {
//       state.script(*source_code);
//       const std::string controller_id = id.substr(0, id.find('.'));
//       Module::RegisterSceneController(
//           controller_id, [controller_id](Scene*) { return std::make_unique<ScriptSceneController>(controller_id); });
//       return true;
//     }
//   } else {
//     LogE("Invalid script type: '{}'", type);
//     return false;
//   }
// }

}  // namespace ovis
