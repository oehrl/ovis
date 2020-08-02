#include <ovis/rendering/render_pass.hpp>
#include <ovis/rendering/render_pipeline.hpp>

namespace ovis {

RenderPass::RenderPass(const std::string& name)
    : name_(name) {}

RenderPass::~RenderPass() {
    if (render_pipeline_ != nullptr) {
        render_pipeline_->RemoveRenderPass(this);
    }
}

}  // namespace ovis
