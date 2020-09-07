#pragma once

#include <ovis/graphics/render_target_texture2d.hpp>
#include <ovis/engine/viewport.hpp>

namespace ovis {

struct RenderTargetViewportDescription {
  RenderTargetTexture2DDescription color_description;
  std::optional<RenderTargetTexture2DDescription> depth_description;
};

class RenderTargetViewport : public Viewport {
 public:
  RenderTargetViewport(GraphicsContext* graphics_context, ResourceManager* resource_manager,
                       const RenderTargetViewportDescription& description);

  glm::ivec2 GetSize() override;
  RenderTargetConfiguration* GetDefaultRenderTargetConfiguration() override;
  inline RenderTargetTexture2D* color_texture() { return color_.get(); }
  inline RenderTargetTexture2D* depth_texture() { return depth_.get(); }

 private:
  RenderTargetViewportDescription description_;
  std::unique_ptr<RenderTargetTexture2D> color_;
  std::unique_ptr<RenderTargetTexture2D> depth_;
  std::unique_ptr<RenderTargetConfiguration> render_target_configuration_;
};

}  // namespace ovis
