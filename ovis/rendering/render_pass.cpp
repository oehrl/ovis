#include <ovis/rendering/render_pass.hpp>
#include <ovis/rendering/render_pipeline.hpp>

namespace ovis {

RenderPass::RenderPass(const std::string& name) : name_(name) {}

RenderPass::~RenderPass() {
  if (render_pipeline_ != nullptr) {
    render_pipeline_->RemoveRenderPass(this);
  }
}

void RenderPass::RenderBefore(const std::string& render_pass_name) {
  render_before_list_.insert(render_pass_name);
}

void RenderPass::RenderAfter(const std::string& render_pass_name) {
  render_after_list_.insert(render_pass_name);
}

}  // namespace ovis
