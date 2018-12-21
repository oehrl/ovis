#pragma once

#include <glm/vec4.hpp>
#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>
#include <ovis/graphics/texture.hpp>

namespace ovis {

class RenderTarget : public GraphicsResource {
  friend class RenderTargetConfiguration;
  
 public:
  RenderTarget(GraphicsContext* context);
  virtual ~RenderTarget() override = default;

  virtual void Clear(const glm::vec4& color) = 0;

 private:
  virtual void Attach(GLenum attachment_point) = 0;
};

}  // namespace ovis
