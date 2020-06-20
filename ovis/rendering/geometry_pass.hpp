#pragma once

#include <ovis/rendering/render_pass.hpp>

namespace ovis {

class GeometryPass : public RenderPass {
 public:
  GeometryPass(RenderPipeline* render_pipeline);

  void Render() override {}
};

}  // namespace ovis
