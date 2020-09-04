#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include <ovis/core/json.hpp>
#include <ovis/graphics/texture.hpp>

namespace ovis {

class ResourceManager;

struct CubemapDescription {
  std::size_t width;
  std::size_t height;
  std::size_t mip_map_count;
  TextureFormat format;
  TextureFilter filter;
};

enum class CubemapSide {
  POSITIVE_X,
  NEGATIVE_X,
  POSITIVE_Y,
  NEGATIVE_Y,
  POSITIVE_Z,
  NEGATIVE_Z,

  RIGHT = POSITIVE_X,
  LEFT = NEGATIVE_X,
  TOP = POSITIVE_Y,
  BOTTOM = NEGATIVE_Y,
  FRONT = POSITIVE_Z,
  BACK = NEGATIVE_Z,
};

class Cubemap : public Texture {
  friend class RenderTargetCubemap;

 public:
  Cubemap(GraphicsContext* context, const CubemapDescription& description, const void* pixels = nullptr);

  void GenerateMipMaps();

  void Write(CubemapSide side, std::size_t level, std::size_t x, std::size_t y, std::size_t width, std::size_t height,
             const void* data);

  inline const CubemapDescription& description() const { return m_description; }

 private:
  CubemapDescription m_description;

  virtual void Bind(int texture_unit) override;
};

bool LoadCubemap(GraphicsContext* graphics_context, ResourceManager* resource_manager, const json& parameters,
                 const std::string& id, const std::string& directory);

}  // namespace ovis
