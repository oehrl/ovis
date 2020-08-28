#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/index_buffer.hpp>

namespace ovis {

IndexBuffer::IndexBuffer(GraphicsContext* context, const IndexBufferDescription& description, const void* index_data)
    : GraphicsBuffer(context), m_description(description) {
  SDL_assert(description.index_format != IndexFormat::UINT16 || description.size_in_bytes % 2 == 0);
  Bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, description.size_in_bytes, index_data, GL_STATIC_DRAW);

  switch (description.index_format) {
    case IndexFormat::UINT8:
      m_bytes_per_index = 1;
      break;

    case IndexFormat::UINT16:
      m_bytes_per_index = 2;
      break;

    case IndexFormat::UINT32:
      m_bytes_per_index = 4;
      break;

    default:
      SDL_assert(false);
  }
}

IndexBuffer::~IndexBuffer() {
  if (context()->m_bound_element_array_buffer == name()) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    context()->m_bound_element_array_buffer = 0;
  }
}

void IndexBuffer::Write(std::size_t offset_in_bytes, std::size_t length_in_bytes, const void* index_data) {
  Bind();
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset_in_bytes, length_in_bytes, index_data);
}

void IndexBuffer::Bind() {
  if (context()->m_bound_element_array_buffer != name()) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, name());
    context()->m_bound_element_array_buffer = name();
  }
}

}  // namespace ovis