#pragma once

#include <vector>
#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

class RenderTarget;

struct RenderTargetConfigurationDescription {
  std::vector<RenderTarget*> color_attachments;
  RenderTarget* depth_attachment;
  RenderTarget* stencil_attachment;
};

class RenderTargetConfiguration : public GraphicsResource {
 public:
  RenderTargetConfiguration(GraphicsContext* context,
    const RenderTargetConfigurationDescription& description);
  virtual ~RenderTargetConfiguration() override;

 private:
  GLuint m_frame_buffer;

  void Bind();
};

}  // namespace ovis
