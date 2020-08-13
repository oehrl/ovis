#include <cassert>

#include <ovis/core/log.hpp>
#include <ovis/engine/render_pipeline.hpp>
#include <ovis/engine/sub_system.hpp>

namespace ovis {

SubSystem::SubSystem(const std::string& name) : name_(name) {
}

SubSystem::~SubSystem() {
}

void SubSystem::RegisterRenderPass(const std::string& id) {
  if (!RenderPipeline::render_pass_factories()
           ->insert(std::make_pair(id, this))
           .second) {
    LogE("The render pass '{}' was already registered", id);
  }
}

void SubSystem::RegisterSceneController(const std::string& id) {}

}  // namespace ovis