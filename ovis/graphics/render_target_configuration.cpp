#include <ovis/core/range.hpp>

#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/render_target.hpp>
#include <ovis/graphics/render_target_configuration.hpp>

namespace ovis {

RenderTargetConfiguration::RenderTargetConfiguration(
    GraphicsContext* context,
    const RenderTargetConfigurationDescription& description)
    : GraphicsResource(context) {
  glGenFramebuffers(1, &m_frame_buffer);

  Bind();
  draw_buffers_.reserve(description.color_attachments.size());
  for (auto color_attachment : IndexRange(description.color_attachments)) {
    if (color_attachment.value() != nullptr) {
      color_attachment.value()->Attach(GL_COLOR_ATTACHMENT0 +
                                       color_attachment.index());
      width_ = color_attachment.value()->GetWidth();
      height_ = color_attachment.value()->GetHeight();
      draw_buffers_.push_back(GL_COLOR_ATTACHMENT0 + color_attachment.index());
    } else {
      draw_buffers_.push_back(GL_NONE);
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
  if (m_frame_buffer != 0) {
    if (context()->m_bound_frame_buffer == m_frame_buffer) {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      context()->m_bound_frame_buffer = 0;
    }
    glDeleteFramebuffers(1, &m_frame_buffer);
  }
}

void RenderTargetConfiguration::ClearColor(const glm::vec4& clear_color) {
  if (context()->scissoring_enabled_) {
    glDisable(GL_SCISSOR_TEST);
    context()->scissoring_enabled_ = false;
  }
  Bind();
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void RenderTargetConfiguration::ClearDepth(float depth) {
  if (context()->scissoring_enabled_) {
    glDisable(GL_SCISSOR_TEST);
    context()->scissoring_enabled_ = false;
  }
  Bind();
  glClearDepth(depth);
  glClear(GL_DEPTH_BUFFER_BIT);
}

RenderTargetConfiguration::RenderTargetConfiguration(GraphicsContext* context,
                                                     std::size_t width,
                                                     std::size_t height)
    : GraphicsResource(context),
      m_frame_buffer(0),
      width_(width),
      height_(height),
      draw_buffers_({GL_COLOR_ATTACHMENT0}) {}

void RenderTargetConfiguration::Bind() {
  if (context()->m_bound_frame_buffer != m_frame_buffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);
    context()->m_bound_frame_buffer = m_frame_buffer;
    glDrawBuffers(draw_buffers_.size(), draw_buffers_.data());
  }
}

}  // namespace ovis