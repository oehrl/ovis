#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include <ovis/core/json.hpp>
#include <ovis/graphics/texture.hpp>

namespace ovis {

class ResourceManager;

struct Texture2DDescription {
  std::size_t width;
  std::size_t height;
  std::size_t mip_map_count;
  TextureFormat format;
  TextureFilter filter;
};

class Texture2D : public Texture {
  friend class RenderTargetTexture2D;

 public:
  Texture2D(GraphicsContext* context, const Texture2DDescription& description, const void* pixels = nullptr);

  void GenerateMipMaps();

  void Write(std::size_t level, std::size_t x, std::size_t y, std::size_t width, std::size_t height, const void* data);

  inline const Texture2DDescription& description() const { return m_description; }

 private:
  Texture2DDescription m_description;

  virtual void Bind(int texture_unit) override;
};

bool LoadTexture2D(GraphicsContext* graphics_context, ResourceManager* resource_manager, const json& parameters,
                   const std::string& id, const std::string& directory);

}  // namespace ovis
