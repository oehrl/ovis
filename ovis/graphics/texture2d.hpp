#pragma once

#include <cstdlib>

#include <memory>
#include <string>

#include <rapidjson/document.h>

#include <ovis/core/file.hpp>

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
  Texture2D(GraphicsContext* context, const Texture2DDescription& description);
  Texture2D(GraphicsContext* context, const Texture2DDescription& description,
            const void* pixels, size_t size_in_bytes);
  Texture2D(GraphicsContext* context, const Texture2DDescription& description,
            const std::vector<Blob>& mip_map_data);

  void GenerateMipMaps();

  void Write(std::size_t level, std::size_t x, std::size_t y, std::size_t width,
             std::size_t height, const void* data);

  inline const Texture2DDescription& description() const {
    return description_;
  }

 private:
  Texture2DDescription description_;

  void Initialize(GLenum* internal_format, GLenum* source_format,
                  GLenum* source_type);

  virtual void Bind(int texture_unit) override;
};

bool LoadTexture2D(GraphicsContext* graphics_context,
                   ResourceManager* resource_manager,
                   const rapidjson::Document& parameters, const std::string& id,
                   const std::string& directory);

}  // namespace ovis
