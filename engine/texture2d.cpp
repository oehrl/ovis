#include <SDL_assert.h>
#include <SDL_surface.h>

#include "application.hpp"
#include "graphics_device.hpp"
#include "texture2d.hpp"

Texture2D::Texture2D(
    GraphicsDevice* graphics_device,
    const Texture2DDescription& description,
    const void* pixels
) :
    Texture(graphics_device)
{
    Bind(0);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        static_cast<GLsizei>(description.width),
        static_cast<GLsizei>(description.height),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::Bind(Uint32 texture_unit)
{
    graphics_device()->BindTexture(GL_TEXTURE_2D, texture_name(), texture_unit);
}

std::unique_ptr<Texture2D> LoadTexture(const std::string& filename)
{
    
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
    
    if (surface->format->format != SDL_PIXELFORMAT_RGB24)
    {
        SDL_Surface* new_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
        SDL_FreeSurface(surface);
        surface = new_surface;
    }
    SDL_assert(surface->format->format == SDL_PIXELFORMAT_RGB24);
    
    SDL_LockSurface(surface);
    
    Texture2DDescription tex_desc;
    tex_desc.width = surface->w;
    tex_desc.height = surface->h;
    tex_desc.mip_map_count = 1;
    tex_desc.format = TextureFormat::RGB_UINT8;
    
    std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>(
        graphics_device(),
        tex_desc,
        surface->pixels
    );
    
    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);
    
    return std::move(texture);
}
