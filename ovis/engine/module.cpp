#include <cassert>

#include <ovis/core/log.hpp>
#include <ovis/engine/module.hpp>
#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/scene_controller.hpp>
#include <ovis/engine/scene_object_component.hpp>

namespace ovis {

Module::Module(const std::string& name) : name_(name) {}

Module::~Module() {}

void Module::RegisterRenderPass(const std::string& id) {
  if (!RenderPass::factories()->insert(std::make_pair(id, this)).second) {
    LogE("The render pass '{}' was already registered", id);
  }
}

void Module::RegisterSceneController(const std::string& id) {
  if (!SceneController::factories()->insert(std::make_pair(id, this)).second) {
    LogE("The scene controller '{}' was already registered", id);
  }
}

void Module::RegisterSceneObjectComponent(const std::string& id) {
  if (!SceneObjectComponent::factories()->insert(std::make_pair(id, this)).second) {
    LogE("The scene object component '{}' was already registered", id);
  }
}

}  // namespace ovis