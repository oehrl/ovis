#include "graphics_buffer.hpp"

GraphicsBuffer::GraphicsBuffer(GraphicsDevice* graphics_device) :
    GraphicsResource(graphics_device),
    m_buffer_name(0)
{
    glGenBuffers(1, &m_buffer_name);
}

GraphicsBuffer::~GraphicsBuffer()
{
    glDeleteBuffers(1, &m_buffer_name);
}
