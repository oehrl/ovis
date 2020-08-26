#pragma once

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

// TODO: change to pixel format
enum class TextureFormat {
  RGB_UINT8,
  RGBA_UINT8,
  RGBA_FLOAT32,
  RGBA_S3TC_DXT1,
  RGBA_S3TC_DXT5,
  DEPTH_UINT16,
  DEPTH_UINT24,
  DEPTH_FLOAT32,
};

inline bool IsTextureFormatCompressed(TextureFormat format) {
  return format == TextureFormat::RGBA_S3TC_DXT1 || format == TextureFormat::RGBA_S3TC_DXT5;
}

enum class TextureFilter {
  POINT,
  BILINEAR,
  TRILINEAR,
};

class Texture : public GraphicsResource {
  friend class UniformBuffer;

 public:
  Texture(GraphicsContext* context);
  virtual ~Texture() override;

 protected:
  inline GLuint name() const { return name_; }

 private:
  GLuint name_;

  virtual void Bind(int texture_unit) = 0;
};

}  // namespace ovis
