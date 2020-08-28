#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/render_pipeline.hpp>

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
  registered_render_passes.reserve(factories()->size());
  for (const auto& render_pass_factory : *factories()) {
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

std::unordered_map<std::string, Module*>* RenderPass::factories() {
  static std::unordered_map<std::string, Module*>* factories = new std::unordered_map<std::string, Module*>();
  return factories;
}

}  // namespace ovis
