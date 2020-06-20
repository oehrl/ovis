#include <ovis/rendering/geometry_pass.hpp>

namespace ovis {

GeometryPass::GeometryPass(RenderPipeline* render_pipeline)
    : RenderPass(render_pipeline, "GeometryPass") {}

}  // namespace ovis
