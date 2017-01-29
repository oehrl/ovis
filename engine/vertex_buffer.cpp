#include "graphics_device.hpp"
#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer
(
    GraphicsDevice* graphics_device,
    const VertexBufferDescription& description,
    const void* vertex_data
) :
    GraphicsBuffer(graphics_device),
    m_description(description)
{
    SDL_assert(description.vertex_size_in_bytes <= description.size_in_bytes);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, description.size_in_bytes, vertex_data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    if (graphics_device()->m_bound_array_buffer == buffer_name())
    {
        graphics_device()->BindArrayBuffer(0);
    }
}

void VertexBuffer::Write
(
    std::size_t offset_in_bytes,
    std::size_t length_in_bytes,
    const void* vertex_data
)
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset_in_bytes, length_in_bytes, vertex_data);
}

void VertexBuffer::Bind()
{
    graphics_device()->BindArrayBuffer(buffer_name());
}
