#include <ovis/graphics/render_target_configuration.hpp>

#include <ovis/core/range.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/render_target.hpp>

namespace ovis {

RenderTargetConfiguration::RenderTargetConfiguration(
  GraphicsContext* context,
  const RenderTargetConfigurationDescription& description) 
    : GraphicsResource(context) {
  glGenFramebuffers(1, &m_frame_buffer);

  Bind();
  for (auto color_attachment : IndexRange(description.color_attachments)) {
    if (color_attachment.value() != nullptr) {
      color_attachment.value()->Attach(GL_COLOR_ATTACHMENT0 +
                                       color_attachment.index());
    }
  }
  if (description.depth_attachment != nullptr) {
    description.depth_attachment->Attach(GL_DEPTH_ATTACHMENT);
  }
  if (description.stencil_attachment != nullptr) {
    description.stencil_attachment->Attach(GL_STENCIL_ATTACHMENT);
  }

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  SDL_assert(status == GL_FRAMEBUFFER_COMPLETE);
}

RenderTargetConfiguration::~RenderTargetConfiguration() {
  if (context()->m_bound_frame_buffer == m_frame_buffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    context()->m_bound_frame_buffer = 0;
  }
}

void RenderTargetConfiguration::Bind() {
  if (context()->m_bound_frame_buffer != m_frame_buffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
    context()->m_bound_frame_buffer = m_frame_buffer;
  }
}

}