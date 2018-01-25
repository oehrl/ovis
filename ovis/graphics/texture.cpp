#include <ovis/graphics/texture.hpp>

namespace ovis {

Texture::Texture(GraphicsContext* context) : GraphicsResource(context) {
  glGenTextures(1, &name_);
}

Texture::~Texture() {
  glDeleteTextures(1, &name_);
}

}  // namespace ovis