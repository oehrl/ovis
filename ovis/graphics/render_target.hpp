#pragma once

#include <glm/vec4.hpp>

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>
#include <ovis/graphics/texture.hpp>

namespace ovis {

class RenderTarget : public GraphicsResource {
  friend class RenderTargetConfiguration;

 public:
  RenderTarget(GraphicsContext* context) : GraphicsResource(context) {}
  virtual ~RenderTarget() override = default;

  virtual size_t GetWidth() const = 0;
  virtual size_t GetHeight() const = 0;

 private:
  virtual void Attach(GLenum attachment_point) = 0;
};

}  // namespace ovis
