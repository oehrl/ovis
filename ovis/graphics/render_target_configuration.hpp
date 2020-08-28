#pragma once

#include <vector>

#include <glm/vec4.hpp>

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

class RenderTarget;

struct RenderTargetConfigurationDescription {
  std::vector<RenderTarget*> color_attachments;
  RenderTarget* depth_attachment = nullptr;
  RenderTarget* stencil_attachment = nullptr;
};

class RenderTargetConfiguration : public GraphicsResource {
  friend class GraphicsContext;

 public:
  RenderTargetConfiguration(GraphicsContext* context, const RenderTargetConfigurationDescription& description);
  virtual ~RenderTargetConfiguration() override;

  void ClearColor(size_t color_attachment_index, const glm::vec4& color = {0.0f, 0.0f, 0.0f, 0.0f});
  void ClearDepth(float depth = 1.0f);

  inline std::size_t width() const { return width_; }
  inline std::size_t height() const { return height_; }

 private:
  std::vector<GLenum> draw_buffers_;
  GLuint m_frame_buffer;

  std::size_t width_;
  std::size_t height_;

  // This constructor creates the default render target configuration. Which is
  // created byt the graphics context.
  RenderTargetConfiguration(GraphicsContext* context, std::size_t width, std::size_t height_);

  void Bind();
};

}  // namespace ovis
