#include "graphics_device.hpp"
#include "index_buffer.hpp"

IndexBuffer::IndexBuffer
(
    GraphicsDevice* graphics_device,
    const IndexBufferDescription& description,
    const void* index_data
) :
    GraphicsBuffer(graphics_device),
    m_description(description)
{
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

        default:
            SDL_assert(false);
    }
}

IndexBuffer::~IndexBuffer()
{
    if (graphics_device()->m_bound_element_array_buffer == buffer_name())
    {
        graphics_device()->BindElementArrayBuffer(0);
    }
}

void IndexBuffer::Write
(
    std::size_t offset_in_bytes,
    std::size_t length_in_bytes,
    const void* index_data
)
{
    Bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset_in_bytes, length_in_bytes, index_data);
}

void IndexBuffer::Bind()
{
    graphics_device()->BindElementArrayBuffer(buffer_name());
}
