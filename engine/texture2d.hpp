#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include "texture.hpp"

enum class TextureFormat
{
    RGB_UINT8
};

struct Texture2DDescription
{
    std::size_t width;
    std::size_t height;
    std::size_t mip_map_count;
    TextureFormat format;
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

private:
    virtual void Bind(Uint32 texture_unit) override;
};

std::unique_ptr<Texture2D> LoadTexture(const std::string& filename);
