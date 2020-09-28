#pragma once

#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <SDL2/SDL_events.h>

#include <ovis/core/class.hpp>
#include <ovis/engine/lua.hpp>
#include <ovis/engine/scene_object.hpp>

namespace ovis {

class RenderPass;
class SceneController;
class Viewport;
class Scene;
class SceneObject;
class SceneObjectComponent;

class Module {
  MAKE_NON_COPY_OR_MOVABLE(Module);

  friend class RenderPass;
  friend class Viewport;
  friend class Scene;
  friend class SceneController;
  friend class SceneObject;
  friend class SceneObjectComponent;
  friend class Lua;

 public:
  Module(const std::string& name);
  virtual ~Module();

  inline std::string name() const { return name_; }

  using RenderPassFactoryFunction = std::function<std::unique_ptr<RenderPass>(Viewport*)>;
  using SceneControllerFactoryFunction = std::function<std::unique_ptr<SceneController>(Scene*)>;
  using SceneObjectComponentFactoryFunction = std::function<std::unique_ptr<SceneObjectComponent>(SceneObject*)>;
  using ResourceLoadingFunction = std::function<std::unique_ptr<SceneObjectComponent>()>;

  static void RegisterToLua();

 protected:
  static void RegisterRenderPass(const std::string& id, const RenderPassFactoryFunction& factory_function);
  static void RegisterSceneController(const std::string& id, const SceneControllerFactoryFunction& factory_function);

  template <typename T>
  static void RegisterSceneObjectComponent(const std::string& id,
                                           const SceneObjectComponentFactoryFunction& factory_function);

  static void DeregisterRenderPass(const std::string& id);
  static void DeregisterSceneController(const std::string& id);
  static void DeregisterSceneObjectComponent(const std::string& id);

  static bool IsRenderPassRegistered(const std::string& id);
  static bool IsSceneControllerRegistered(const std::string& id);
  static bool IsSceneObjectComponentRegistered(const std::string& id);

 private:
  std::string name_;

  static std::map<std::string, RenderPassFactoryFunction>* render_pass_factory_functions();
  static std::map<std::string, SceneControllerFactoryFunction>* scene_controller_factory_functions();
  static std::map<std::string, SceneObjectComponentFactoryFunction>* scene_object_component_factory_functions();

  // Wrapper object for Lua
  struct SceneObjectComponentsWrapper {
    SceneObject* object;

    static SceneObjectComponentsWrapper FromObject(SceneObject* object) { return {object}; }
  };
};

template <typename T>
void Module::RegisterSceneObjectComponent(const std::string& id,
                                          const SceneObjectComponentFactoryFunction& factory_function) {
  if (!scene_object_component_factory_functions()->insert(std::make_pair(id, factory_function)).second) {
    LogE("The scene object component '{}' was already registered", id);
  } else {
    sol::usertype<SceneObjectComponentsWrapper> lua_type = Lua::state["SceneObjectComponentsWrapper"];
    lua_type[id] =
        sol::property([id](SceneObjectComponentsWrapper components) { return components.object->GetComponent<T>(id); });
  }
}

}  // namespace ovis
