#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include "texture.hpp"

enum class TextureFormat
{
    RGB_UINT8,
    RGBA_UINT8,
};

enum class TextureFilter
{
    POINT,
    BILINEAR,
    TRILINEAR,
};

struct Texture2DDescription
{
    std::size_t width;
    std::size_t height;
    std::size_t mip_map_count;
    TextureFormat format;
    TextureFilter filter;
};

class Texture2D :
    public Texture
{
public:
    Texture2D(
        GraphicsDevice* graphics_device,
        const Texture2DDescription& description,
        const void* pixels = nullptr
    );
    
    void GenerateMipMaps();
    
    void Write(
        std::size_t level,
        std::size_t x,
        std::size_t y,
        std::size_t width,
        std::size_t height,
        const void* data
    );
    
    inline const Texture2DDescription& description() const
    {
        return m_description;
    }

private:
    Texture2DDescription m_description;

    virtual void Bind(Uint32 texture_unit) override;
};

std::unique_ptr<Texture2D> LoadTexture(const std::string& filename);
