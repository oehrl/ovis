#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/vertex_buffer.hpp>

namespace ovis {

VertexBuffer::VertexBuffer(GraphicsContext* context, const VertexBufferDescription& description,
                           const void* vertex_data)
    : GraphicsBuffer(context), m_description(description) {
  SDL_assert(description.vertex_size_in_bytes <= description.size_in_bytes);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, description.size_in_bytes, vertex_data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  if (context()->m_bound_array_buffer == name()) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    context()->m_bound_array_buffer = 0;
  }
}

void VertexBuffer::Write(std::size_t offset_in_bytes, std::size_t length_in_bytes, const void* vertex_data) {
  Bind();
  glBufferSubData(GL_ARRAY_BUFFER, offset_in_bytes, length_in_bytes, vertex_data);
}

void VertexBuffer::Bind() {
  if (context()->m_bound_array_buffer != name()) {
    glBindBuffer(GL_ARRAY_BUFFER, name());
    context()->m_bound_array_buffer = name();
  }
}

}  // namespace ovis
