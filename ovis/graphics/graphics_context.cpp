#include <ovis/core/log.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/index_buffer.hpp>
#include <ovis/graphics/render_target_configuration.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/vertex_input.hpp>

namespace ovis {

GraphicsContext::GraphicsContext(SDL_Window* window)
    : window_(window),
      m_context(nullptr),
      m_bound_array_buffer(0),
      m_bound_element_array_buffer(0),
      m_bound_program(0),
      m_active_texture_unit(0),
      scissoring_enabled_(false) {
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

#if !defined(__IPHONEOS__) && !defined(__EMSCRIPTEN__)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

  m_context = SDL_GL_CreateContext(window);
  SDL_assert(m_context != nullptr);
  SDL_GL_MakeCurrent(window, m_context);

  LogI("OpenGL version: {}", glGetString(GL_VERSION));

#if _WIN32
  glewInit();
#endif

  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_caps.max_vertex_attribs);
  SDL_assert(m_caps.max_vertex_attribs >= 8);
  m_vertex_attrib_array_states.resize(m_caps.max_vertex_attribs, false);

  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_caps.num_texture_units);
  SDL_assert(m_caps.num_texture_units >= 8);
  m_bound_textures.resize(m_caps.num_texture_units, 0);

  glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_caps.num_vertex_texture_units);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int drawable_width;
  int drawable_height;
  SDL_GL_GetDrawableSize(window, &drawable_width, &drawable_height);
  LogD("SDL_GL_GetDrawableSize {}x{}", drawable_width, drawable_height);
  current_viewport_ = {0, 0, drawable_width, drawable_height};

  m_default_render_target_configuration.reset(new RenderTargetConfiguration(this, drawable_width, drawable_height));

#if !defined(__IPHONEOS__) && !defined(__EMSCRIPTEN__)
  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);
#endif
}

GraphicsContext::~GraphicsContext() {
  m_default_render_target_configuration.reset();
  SDL_assert(m_graphics_resources.size() == 0);
}

void GraphicsContext::Draw(const DrawItem& draw_item) {
  SDL_assert(draw_item.shader_program != nullptr);
  // SDL_assert(draw_item.vertex_input != nullptr);

  ApplyBlendState(&blend_state_, draw_item.blend_state);
  ApplyDepthBufferState(&depth_buffer_state_, draw_item.depth_buffer_state);

  // TODO: make this more efficient as it's done on every draw call
  int drawable_width;
  int drawable_height;
  SDL_GL_GetDrawableSize(window_, &drawable_width, &drawable_height);
  m_default_render_target_configuration->width_ = drawable_width;
  m_default_render_target_configuration->height_ = drawable_height;

  auto targets = draw_item.render_target_configuration != nullptr ? draw_item.render_target_configuration
                                                                  : default_render_target_configuration();
  targets->Bind();
  if (targets->width() != current_viewport_.width || targets->height() != current_viewport_.height) {
    glViewport(0, 0, targets->width(), targets->height());
    current_viewport_.width = targets->width();
    current_viewport_.height = targets->height();
  }

  if (draw_item.scissor_rect.has_value() != scissoring_enabled_) {
    if (draw_item.scissor_rect) {
      glEnable(GL_SCISSOR_TEST);
      scissoring_enabled_ = true;
      if (*draw_item.scissor_rect != current_scissor_rect_) {
        glScissor(draw_item.scissor_rect->left, draw_item.scissor_rect->top, draw_item.scissor_rect->width,
                  draw_item.scissor_rect->height);
        current_scissor_rect_ = *draw_item.scissor_rect;
      }
    } else {
      glDisable(GL_SCISSOR_TEST);
      scissoring_enabled_ = false;
    }
  }

  if (culling_enabled_ != draw_item.enable_culling) {
    if (draw_item.enable_culling) {
      glEnable(GL_CULL_FACE);
    } else {
      glDisable(GL_CULL_FACE);
    }
    culling_enabled_ = draw_item.enable_culling;
  }

  draw_item.shader_program->Bind();
  if (draw_item.vertex_input != nullptr) {
    draw_item.vertex_input->Bind();
  }
  const GLenum primitive_topology = static_cast<GLenum>(draw_item.primitive_topology);

  if (draw_item.index_buffer == nullptr) {
#ifdef DEBUG
    glValidateProgram(draw_item.shader_program->m_program_name);
    GLint validation_status = 0;
    glGetProgramiv(draw_item.shader_program->m_program_name, GL_VALIDATE_STATUS, &validation_status);
    SDL_assert(validation_status == GL_TRUE);
#endif

    glDrawArrays(primitive_topology, draw_item.start, draw_item.count);
  } else {
    draw_item.index_buffer->Bind();

#ifdef DEBUG
    glValidateProgram(draw_item.shader_program->m_program_name);
    GLint validation_status = 0;
    glGetProgramiv(draw_item.shader_program->m_program_name, GL_VALIDATE_STATUS, &validation_status);
    SDL_assert(validation_status == GL_TRUE);
#endif

    const GLenum index_type = static_cast<GLenum>(draw_item.index_buffer->description().index_format);
    const auto index_offset_in_bytes = draw_item.start * draw_item.index_buffer->bytes_per_index();
#if !OVIS_EMSCRIPTEN
    glDrawElementsBaseVertex(primitive_topology, draw_item.count, index_type,
                             reinterpret_cast<GLvoid*>(index_offset_in_bytes), draw_item.base_vertex);
#else
    SDL_assert(draw_item.base_vertex == 0);
    glDrawElements(primitive_topology, draw_item.count, index_type, reinterpret_cast<GLvoid*>(index_offset_in_bytes));
#endif
  }
}

}  // namespace ovis
