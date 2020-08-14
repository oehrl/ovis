#pragma once

#include <chrono>

#include <algorithm>
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

class Module {
  MAKE_NON_COPY_OR_MOVABLE(Module);

 public:
  Module(const std::string& name);
  virtual ~Module();

  inline std::string name() const { return name_; }

  // virtual void BeforeEventProcessing() {}
  // virtual void AfterEventProcessing() {}

  // virtual void BeforeUpdate() {}
  // virtual void AfterUpdate() {}

  // virtual void BeforeRendering() {}
  // virtual void AfterRendering() {}

  virtual std::unique_ptr<RenderPass> CreateRenderPass(
      const std::string& id, RenderPipeline* render_pipeline) {
    return nullptr;
  }

  virtual std::unique_ptr<SceneController> CreateSceneController(
      const std::string& id, Scene* scene) {
    return nullptr;
  }

 protected:
  void RegisterRenderPass(const std::string& id);
  void RegisterSceneController(const std::string& id);

 private:
  std::string name_;
};

#define OVIS_ADD_MODULE(ModuleName)                                  \
  static_assert(std::is_base_of<::ovis::Module, ModuleName>::value); \
  namespace {                                                        \
  static ModuleName ovis_module_ModuleName;                          \
  }

}  // namespace ovis
