#include <SDL2/SDL_assert.h>

#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

GraphicsResource::~GraphicsResource() {
  SDL_assert(context_->m_graphics_resources.count(this) == 1);
  context_->m_graphics_resources.erase(this);
}

GraphicsResource::GraphicsResource(GraphicsContext* context) : context_(context) {
  SDL_assert(context != nullptr);
  SDL_assert(context->m_graphics_resources.count(this) == 0);
  context->m_graphics_resources.insert(this);
}

}  // namespace ovis
