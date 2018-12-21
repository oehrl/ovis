#include <ovis/graphics/render_target_texture2d.hpp>

namespace ovis {

RenderTargetTexture2D::RenderTargetTexture2D(
  GraphicsContext* context,
  const RenderTargetTexture2DDescription& description)
    : RenderTarget(context)
    , m_texture(context, description.texture_description) {
}

void RenderTargetTexture2D::Attach(GLenum attachment_point) {
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         attachment_point,
                         GL_TEXTURE_2D,
                         m_texture.name(),
                         0);
}

}