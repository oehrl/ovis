#include <cassert>

#include <ovis/core/log.hpp>

#include <ovis/engine/module.hpp>
#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/scene_controller.hpp>

namespace ovis {

Module::Module(const std::string& name) : name_(name) {}

Module::~Module() {}

void Module::RegisterRenderPass(const std::string& id) {
  if (!RenderPass::render_pass_factories()
           ->insert(std::make_pair(id, this))
           .second) {
    LogE("The render pass '{}' was already registered", id);
  }
}

void Module::RegisterSceneController(const std::string& id) {
  if (!SceneController::scene_controller_factories()
           ->insert(std::make_pair(id, this))
           .second) {
    LogE("The scene controller '{}' was already registered", id);
  }
}

}  // namespace ovis