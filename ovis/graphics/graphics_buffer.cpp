#include <ovis/graphics/graphics_buffer.hpp>

#include <SDL_assert.h>

namespace ovis {

GraphicsBuffer::GraphicsBuffer(GraphicsContext* context)
    : GraphicsResource(context), name_(0) {
  glGenBuffers(1, &name_);
  SDL_assert(name_ != 0);
}

GraphicsBuffer::~GraphicsBuffer() {
  // Bound check is done in derived classes
  glDeleteBuffers(1, &name_);
}

}  // namespace ovis
