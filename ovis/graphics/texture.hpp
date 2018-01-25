#pragma once

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

enum class TextureFormat {
  RGB_UINT8,
  RGBA_UINT8,
};

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
