#include <ovis/core/range.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/vertex_buffer.hpp>
#include <ovis/graphics/vertex_input.hpp>

namespace ovis {

VertexInput::VertexInput(GraphicsContext* context, const VertexInputDescription& description)
    : GraphicsResource(context), m_description(description) {
  for (const auto& vertex_attribute : description.vertex_attributes) {
    SDL_assert(vertex_attribute.buffer_index < description.vertex_buffers.size());
  }

  m_attribute_gl_descriptions.resize(context->m_caps.max_vertex_attribs);
  for (auto& attribute : m_attribute_gl_descriptions) {
    attribute.enabled = false;
  }

  for (const auto& vertex_attribute : m_description.vertex_attributes) {
    // GLint location = m_description.shader_program->GetAttributeLocation(
    //     vertex_attribute.name);
    VertexBuffer* vertex_buffer = m_description.vertex_buffers[vertex_attribute.buffer_index];

    auto& attribute = m_attribute_gl_descriptions[vertex_attribute.location];
    attribute.offset = reinterpret_cast<const GLvoid*>(vertex_attribute.offset_in_bytes);
    attribute.stride = vertex_buffer->description().vertex_size_in_bytes;
    attribute.array_buffer = vertex_buffer->name();

    switch (vertex_attribute.type) {
      case VertexAttributeType::FLOAT32:
        attribute.type = GL_FLOAT;
        attribute.size = 1;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::FLOAT32_VECTOR2:
        attribute.type = GL_FLOAT;
        attribute.size = 2;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::FLOAT32_VECTOR3:
        attribute.type = GL_FLOAT;
        attribute.size = 3;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::FLOAT32_VECTOR4:
        attribute.type = GL_FLOAT;
        attribute.size = 4;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT8:
        attribute.type = GL_BYTE;
        attribute.size = 1;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT8_VECTOR2:
        attribute.type = GL_BYTE;
        attribute.size = 2;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT8_VECTOR3:
        attribute.type = GL_BYTE;
        attribute.size = 3;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT8_VECTOR4:
        attribute.type = GL_BYTE;
        attribute.size = 4;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT8:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 1;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT8_VECTOR2:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 2;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT8_VECTOR3:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 3;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT8_VECTOR4:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 4;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT8_NORM:
        attribute.type = GL_BYTE;
        attribute.size = 1;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT8_NORM_VECTOR2:
        attribute.type = GL_BYTE;
        attribute.size = 2;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT8_NORM_VECTOR3:
        attribute.type = GL_BYTE;
        attribute.size = 3;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT8_NORM_VECTOR4:
        attribute.type = GL_BYTE;
        attribute.size = 4;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT8_NORM:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 1;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT8_NORM_VECTOR2:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 2;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT8_NORM_VECTOR3:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 3;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT8_NORM_VECTOR4:
        attribute.type = GL_UNSIGNED_BYTE;
        attribute.size = 4;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT16:
        attribute.type = GL_SHORT;
        attribute.size = 1;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT16_VECTOR2:
        attribute.type = GL_SHORT;
        attribute.size = 2;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT16_VECTOR3:
        attribute.type = GL_SHORT;
        attribute.size = 3;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT16_VECTOR4:
        attribute.type = GL_SHORT;
        attribute.size = 4;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT16:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 1;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT16_VECTOR2:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 2;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT16_VECTOR3:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 3;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::UINT16_VECTOR4:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 4;
        attribute.normalized = GL_FALSE;
        break;

      case VertexAttributeType::INT16_NORM:
        attribute.type = GL_SHORT;
        attribute.size = 1;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT16_NORM_VECTOR2:
        attribute.type = GL_SHORT;
        attribute.size = 2;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT16_NORM_VECTOR3:
        attribute.type = GL_SHORT;
        attribute.size = 3;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::INT16_NORM_VECTOR4:
        attribute.type = GL_SHORT;
        attribute.size = 4;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT16_NORM:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 1;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT16_NORM_VECTOR2:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 2;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT16_NORM_VECTOR3:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 3;
        attribute.normalized = GL_TRUE;
        break;

      case VertexAttributeType::UINT16_NORM_VECTOR4:
        attribute.type = GL_UNSIGNED_SHORT;
        attribute.size = 4;
        attribute.normalized = GL_TRUE;
        break;
    }

    attribute.enabled = true;
  }
}

VertexInput::~VertexInput() {}

void VertexInput::Bind() {
  for (auto gl_desc : IndexRange<GLuint>(m_attribute_gl_descriptions)) {
    if (gl_desc.value().enabled) {
      if (context()->m_bound_array_buffer != gl_desc->array_buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, gl_desc->array_buffer);
        context()->m_bound_array_buffer = gl_desc->array_buffer;
      }
      glVertexAttribPointer(gl_desc.index(), gl_desc->size, gl_desc->type, gl_desc->normalized, gl_desc->stride,
                            gl_desc->offset);
      context()->EnableVertexAttribArray(gl_desc.index());
    } else {
      context()->DisableVertexAttribArray(gl_desc.index());
    }
  }
}

}  // namespace ovis
