#include <ovis/engine/module.hpp>
#include <ovis/engine/render_pass.hpp>

namespace ovis {

RenderPass::RenderPass(const std::string& name)
    : name_(name)
#if OVIS_ENABLE_BUILT_IN_PROFILING
      ,
      cpu_render_profiler_(name + "::Render")
#endif
{
}

std::vector<std::string> RenderPass::GetRegisteredRenderPasses() {
  std::vector<std::string> registered_render_passes;
  registered_render_passes.reserve(Module::render_pass_factory_functions()->size());
  for (const auto& render_pass_factory : *Module::render_pass_factory_functions()) {
    registered_render_passes.push_back(render_pass_factory.first);
  }
  return registered_render_passes;
}

void RenderPass::RenderBefore(const std::string& render_pass_name) {
  render_before_list_.insert(render_pass_name);
}

void RenderPass::RenderAfter(const std::string& render_pass_name) {
  render_after_list_.insert(render_pass_name);
}

}  // namespace ovis
