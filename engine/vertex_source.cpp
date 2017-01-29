#include "graphics_device.hpp"
#include "shader_program.hpp"
#include "vertex_buffer.hpp"
#include "vertex_source.hpp"
#include "range.hpp"

VertexSource::VertexSource
(
    GraphicsDevice* graphics_device,
    const VertexSourceDescription& description
) :
    GraphicsResource(graphics_device),
    m_description(description)
{
    for (const auto& vertex_attribute : description.vertex_attributes)
    {
        SDL_assert(vertex_attribute.buffer_index < description.vertex_buffers.size());
    }
    
    m_attribute_gl_descriptions.resize(graphics_device->m_caps.max_vertex_attribs);
    for (auto& attribute : m_attribute_gl_descriptions)
    {
        attribute.enabled = false;
    }
    
    for (const auto& vertex_attribute : m_description.vertex_attributes)
    {
        GLint location = m_description.shader_program->GetAttributeLocation(vertex_attribute.name);
        if (location >= 0)
        {
            VertexBuffer* vertex_buffer = m_description.vertex_buffers[vertex_attribute.buffer_index];
        
            auto& attribute = m_attribute_gl_descriptions[location];
            attribute.offset = reinterpret_cast<const GLvoid*>(vertex_attribute.offset_in_bytes);
            attribute.stride = vertex_buffer->description().vertex_size_in_bytes;
            attribute.array_buffer = vertex_buffer->buffer_name();
            
            switch (vertex_attribute.type)
            {
                case FLOAT32:
                    attribute.type = GL_FLOAT;
                    attribute.size = 1;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case FLOAT32_VECTOR2:
                    attribute.type = GL_FLOAT;
                    attribute.size = 2;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case FLOAT32_VECTOR3:
                    attribute.type = GL_FLOAT;
                    attribute.size = 3;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case FLOAT32_VECTOR4:
                    attribute.type = GL_FLOAT;
                    attribute.size = 4;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT8:
                    attribute.type = GL_BYTE;
                    attribute.size = 1;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT8_VECTOR2:
                    attribute.type = GL_BYTE;
                    attribute.size = 2;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT8_VECTOR3:
                    attribute.type = GL_BYTE;
                    attribute.size = 3;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT8_VECTOR4:
                    attribute.type = GL_BYTE;
                    attribute.size = 4;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT8:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 1;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT8_VECTOR2:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 2;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT8_VECTOR3:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 3;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT8_VECTOR4:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 4;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT8_NORM:
                    attribute.type = GL_BYTE;
                    attribute.size = 1;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT8_NORM_VECTOR2:
                    attribute.type = GL_BYTE;
                    attribute.size = 2;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT8_NORM_VECTOR3:
                    attribute.type = GL_BYTE;
                    attribute.size = 3;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT8_NORM_VECTOR4:
                    attribute.type = GL_BYTE;
                    attribute.size = 4;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT8_NORM:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 1;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT8_NORM_VECTOR2:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 2;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT8_NORM_VECTOR3:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 3;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT8_NORM_VECTOR4:
                    attribute.type = GL_UNSIGNED_BYTE;
                    attribute.size = 4;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT16:
                    attribute.type = GL_SHORT;
                    attribute.size = 1;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT16_VECTOR2:
                    attribute.type = GL_SHORT;
                    attribute.size = 2;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT16_VECTOR3:
                    attribute.type = GL_SHORT;
                    attribute.size = 3;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT16_VECTOR4:
                    attribute.type = GL_SHORT;
                    attribute.size = 4;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT16:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 1;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT16_VECTOR2:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 2;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT16_VECTOR3:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 3;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case UINT16_VECTOR4:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 4;
                    attribute.normalized = GL_FALSE;
                    break;
                    
                case INT16_NORM:
                    attribute.type = GL_SHORT;
                    attribute.size = 1;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT16_NORM_VECTOR2:
                    attribute.type = GL_SHORT;
                    attribute.size = 2;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT16_NORM_VECTOR3:
                    attribute.type = GL_SHORT;
                    attribute.size = 3;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case INT16_NORM_VECTOR4:
                    attribute.type = GL_SHORT;
                    attribute.size = 4;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT16_NORM:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 1;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT16_NORM_VECTOR2:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 2;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT16_NORM_VECTOR3:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 3;
                    attribute.normalized = GL_TRUE;
                    break;
                    
                case UINT16_NORM_VECTOR4:
                    attribute.type = GL_UNSIGNED_SHORT;
                    attribute.size = 4;
                    attribute.normalized = GL_TRUE;
                    break;
            }
        
            attribute.enabled = true;
        }
    }
}

VertexSource::~VertexSource()
{
}

void VertexSource::Bind()
{
    for (auto gl_desc : IndexRange<GLuint>(m_attribute_gl_descriptions))
    {
        if (gl_desc.value().enabled)
        {
            graphics_device()->BindArrayBuffer(gl_desc.value().array_buffer);
            glVertexAttribPointer
            (
                gl_desc.index(),
                gl_desc.value().size,
                gl_desc.value().type,
                gl_desc.value().normalized,
                gl_desc.value().stride,
                gl_desc.value().offset
            );
            graphics_device()->EnableVertexAttribArray(gl_desc.index());
        } else
        {
            graphics_device()->DisableVertexAttribArray(gl_desc.index());
        }
    }
}
