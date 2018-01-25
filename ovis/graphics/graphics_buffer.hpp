#pragma once

#include <ovis/graphics/gl.hpp>
#include <ovis/graphics/graphics_resource.hpp>

namespace ovis {

class GraphicsBuffer : public GraphicsResource {
 protected:
  GraphicsBuffer(GraphicsContext* context);
  virtual ~GraphicsBuffer() override;

  inline GLuint name() const { return name_; }

  void Bind();

 private:
  GLuint name_;
};

}  // namespace ovis
