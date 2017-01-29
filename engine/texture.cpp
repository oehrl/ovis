#include "texture.hpp"

Texture::Texture(GraphicsDevice* graphics_device) :
    GraphicsResource(graphics_device)
{
    glGenTextures(1, &m_texture_name);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture_name);
}
