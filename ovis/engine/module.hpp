#pragma once

#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <SDL2/SDL_events.h>

#include <ovis/core/class.hpp>

namespace ovis {

class RenderPass;
class SceneController;
class RenderPipeline;
class Scene;
class SceneObject;
class SceneObjectComponent;

class Module {
  MAKE_NON_COPY_OR_MOVABLE(Module);

  friend class RenderPass;
  friend class RenderPipeline;
  friend class Scene;
  friend class SceneController;
  friend class SceneObject;
  friend class SceneObjectComponent;

 public:
  Module(const std::string& name);
  virtual ~Module();

  inline std::string name() const { return name_; }

  using RenderPassFactoryFunction = std::function<std::unique_ptr<RenderPass>(RenderPipeline*)>;
  using SceneControllerFactoryFunction = std::function<std::unique_ptr<SceneController>(Scene*)>;
  using SceneObjectComponentFactoryFunction = std::function<std::unique_ptr<SceneObjectComponent>(SceneObject*)>;
  using ResourceLoadingFunction = std::function<std::unique_ptr<SceneObjectComponent>()>;

 protected:
  void RegisterRenderPass(const std::string& id, const RenderPassFactoryFunction& factory_function);
  void RegisterSceneController(const std::string& id, const SceneControllerFactoryFunction& factory_function);
  void RegisterSceneObjectComponent(const std::string& id, const SceneObjectComponentFactoryFunction& factory_function);
  void RegisterFileLoader(const std::string& extension, const ResourceLoadingFunction& resource_loading_function);

  void DeregisterRenderPass(const std::string& id);
  void DeregisterSceneController(const std::string& id);
  void DeregisterSceneObjectComponent(const std::string& id);

 private:
  std::string name_;

  static std::map<std::string, RenderPassFactoryFunction>* render_pass_factory_functions();
  static std::map<std::string, SceneControllerFactoryFunction>* scene_controller_factory_functions();
  static std::map<std::string, SceneObjectComponentFactoryFunction>* scene_object_component_factory_functions();
};

}  // namespace ovis
