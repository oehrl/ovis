#pragma once

#include <chrono>

#include <algorithm>
#include <string>
#include <vector>

#include <SDL2/SDL_events.h>

#include <ovis/core/class.hpp>

namespace ovis {

class RenderPass;
class SceneController;

class SubSystem {
  MAKE_NON_COPY_OR_MOVABLE(SubSystem);

 public:
  SubSystem(const std::string& name);
  virtual ~SubSystem();

  inline std::string name() const { return name_; }

  // virtual void BeforeEventProcessing() {}
  // virtual void AfterEventProcessing() {}

  // virtual void BeforeUpdate() {}
  // virtual void AfterUpdate() {}

  // virtual void BeforeRendering() {}
  // virtual void AfterRendering() {}

  virtual std::unique_ptr<RenderPass> CreateRenderPass(const std::string& id) {
    return nullptr;
  }

  virtual std::unique_ptr<SceneController> CreateSceneController(
      const std::string& id) {
    return nullptr;
  }

 protected:
  void RegisterRenderPass(const std::string& id);
  void RegisterSceneController(const std::string& id);

 private:
  std::string name_;
};

}  // namespace ovis
