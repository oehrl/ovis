#include <ovis/rendering/render_pass.hpp>
#include <ovis/rendering/render_pipeline.hpp>

namespace ovis {

RenderPass::RenderPass(RenderPipeline* render_pipeline, const std::string& name)
    : render_pipeline_(render_pipeline), name_(name) {}

}  // namespace ovis
