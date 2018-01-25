#pragma once

namespace ovis {

class GraphicsContext;

class GraphicsResource {
 public:
  inline GraphicsContext* context() const { return context_; }
  virtual ~GraphicsResource();

 protected:
  GraphicsResource(GraphicsContext* graphics_device);

 private:
  GraphicsContext* context_;
};

}  // namespace ovis
