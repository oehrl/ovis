#pragma once

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/render_target.hpp>
#include <ovis/graphics/texture2d.hpp>

namespace ovis {

struct RenderTargetTexture2DDescription {
  Texture2DDescription texture_description;
};

class RenderTargetTexture2D : public RenderTarget {
 public:
  RenderTargetTexture2D(GraphicsContext* context,
                        const RenderTargetTexture2DDescription& description);
  virtual ~RenderTargetTexture2D() override;

  void Clear(const glm::vec4& color) override;

  inline Texture2D* texture() { return &m_texture; }

 private:
  Texture2D m_texture;

  void Attach(GLenum attachment_point) override;
};

}  // namespace ovis
