#pragma once

#include <memory>
#include <optional>
#include <set>
#include <vector>

#include <SDL2/SDL.h>

#include <ovis/core/class.hpp>
#include <ovis/math/rect.hpp>
#include <ovis/graphics/blend_state.hpp>
#include <ovis/graphics/depth_buffer_state.hpp>
#include <ovis/graphics/gl.hpp>

namespace ovis {

class GraphicsResource;
class IndexBuffer;
class RenderTargetConfiguration;
class ShaderProgram;
class UniformBuffer;
class VertexBuffer;
class VertexInput;
class Texture;
class RenderTargetConfiguration;

enum class PrimitiveTopology {
  POINTS = GL_POINTS,
  LINE_LIST = GL_LINES,
  LINE_STRIP = GL_LINE_STRIP,
  TRIANGLE_LIST = GL_TRIANGLES,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
};

struct DrawItem {
  ShaderProgram* shader_program = nullptr;
  VertexInput* vertex_input = nullptr;
  IndexBuffer* index_buffer = nullptr;
  PrimitiveTopology primitive_topology = PrimitiveTopology::TRIANGLE_LIST;
  Uint32 start = 0;
  Uint32 count = 3;
  Uint32 base_vertex = 0;
  DepthBufferState depth_buffer_state;
  BlendState blend_state;
  std::optional<Rect<int>> scissor_rect;
  RenderTargetConfiguration* render_target_configuration = nullptr;
  bool enable_culling = false;
};

class GraphicsContext final {
  friend class GraphicsResource;
  friend class IndexBuffer;
  friend class RenderTargetConfiguration;
  friend class ShaderProgram;
  friend class Texture2D;
  friend class Cubemap;
  friend class UniformBuffer;
  friend class VertexBuffer;
  friend class VertexInput;

  MAKE_NON_COPY_OR_MOVABLE(GraphicsContext);

 public:
  GraphicsContext(SDL_Window* window);
  ~GraphicsContext();

  void Draw(const DrawItem& draw_item);

  inline RenderTargetConfiguration* default_render_target_configuration() const {
    return m_default_render_target_configuration.get();
  }

 private:
  SDL_Window* window_;
  SDL_GLContext m_context;
  std::set<GraphicsResource*> m_graphics_resources;
  std::unique_ptr<RenderTargetConfiguration> m_default_render_target_configuration;

  struct {
    GLint max_vertex_attribs;
    GLint num_texture_units;
    GLint num_vertex_texture_units;
  } m_caps;

  GLuint m_bound_frame_buffer;
  GLuint m_bound_array_buffer;
  GLuint m_bound_element_array_buffer;
  GLuint m_bound_program;
  GLuint m_active_texture_unit;
  DepthBufferState depth_buffer_state_;
  BlendState blend_state_;
  std::vector<bool> m_vertex_attrib_array_states;
  std::vector<GLuint> m_bound_textures;
  bool scissoring_enabled_;
  Rect<int> current_scissor_rect_;
  bool culling_enabled_ = false;
  int x1, x2, x3;  // TODO: figure out why these three padding members are
                   // necessary oO
  Rect<int> current_viewport_;

  inline void BindTexture(GLenum texture_type, GLuint texture_name, GLuint texture_unit) {
    SDL_assert(texture_unit < m_bound_textures.size());
    if (m_bound_textures[texture_unit] != texture_name) {
      ActivateTextureUnit(texture_unit);
      glBindTexture(texture_type, texture_name);
      m_bound_textures[texture_unit] = texture_name;
    }
  }

  inline void ActivateTextureUnit(GLuint texture_unit) {
    if (m_active_texture_unit != texture_unit) {
      glActiveTexture(GL_TEXTURE0 + texture_unit);
      m_active_texture_unit = texture_unit;
    }
  }

  inline void EnableVertexAttribArray(GLuint index) {
    SDL_assert(index < m_vertex_attrib_array_states.size());
    if (!m_vertex_attrib_array_states[index]) {
      glEnableVertexAttribArray(index);
      m_vertex_attrib_array_states[index] = true;
    }
  }

  inline void DisableVertexAttribArray(GLuint index) {
    SDL_assert(index < m_vertex_attrib_array_states.size());
    if (m_vertex_attrib_array_states[index]) {
      glDisableVertexAttribArray(index);
      m_vertex_attrib_array_states[index] = false;
    }
  }
};

}  // namespace ovis
