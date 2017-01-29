#include "graphics_device.hpp"
#include "graphics_resource.hpp"

GraphicsResource::~GraphicsResource()
{
    SDL_assert(m_graphics_device->m_graphics_resources.count(this) == 1);
    m_graphics_device->m_graphics_resources.erase(this);
}

GraphicsResource::GraphicsResource(GraphicsDevice* graphics_device) :
    m_graphics_device(graphics_device)
{
    SDL_assert(graphics_device != nullptr);
    SDL_assert(graphics_device->m_graphics_resources.count(this) == 0);
    graphics_device->m_graphics_resources.insert(this);
}
