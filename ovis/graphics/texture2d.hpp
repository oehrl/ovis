#pragma once

#include <cstdlib>
#include <memory>
#include <string>
#include <ovis/graphics/texture.hpp>

namespace ovis {

struct Texture2DDescription {
  std::size_t width;
  std::size_t height;
  std::size_t mip_map_count;
  TextureFormat format;
  TextureFilter filter;
};

class Texture2D : public Texture {
 public:
  Texture2D(GraphicsContext* context, const Texture2DDescription& description,
            const void* pixels = nullptr);

  void GenerateMipMaps();

  void Write(std::size_t level, std::size_t x, std::size_t y, std::size_t width,
             std::size_t height, const void* data);

  inline const Texture2DDescription& description() const {
    return m_description;
  }

 private:
  Texture2DDescription m_description;

  virtual void Bind(int texture_unit) override;
};

// std::unique_ptr<Texture2D> LoadTexture(const std::string& filename);

}  // namespace ovis
