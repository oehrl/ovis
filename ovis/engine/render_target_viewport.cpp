#include <ovis/engine/render_target_viewport.hpp>

namespace ovis {

RenderTargetViewport::RenderTargetViewport(GraphicsContext* graphics_context, ResourceManager* resource_manager,
                                           const RenderTargetViewportDescription& description)
    : description_(description) {
  RenderTargetConfigurationDescription rtt_desc;

  color_ = std::make_unique<RenderTargetTexture2D>(graphics_context, description_.color_description);
  rtt_desc.color_attachments = {color_.get()};

  if (description_.depth_description) {
    SDL_assert(description_.depth_description->texture_description.width =
                   description_.color_description.texture_description.width);
    SDL_assert(description_.depth_description->texture_description.height =
                   description_.color_description.texture_description.height);
    depth_ = std::make_unique<RenderTargetTexture2D>(graphics_context, *description_.depth_description);
    rtt_desc.depth_attachment = depth_.get();
  }

  render_target_configuration_ = std::make_unique<RenderTargetConfiguration>(graphics_context, rtt_desc);

  SetResourceManager(resource_manager);
  SetGraphicsContext(graphics_context);
}

glm::ivec2 RenderTargetViewport::GetSize() {
  return {description_.color_description.texture_description.width,
          description_.color_description.texture_description.height};
}

RenderTargetConfiguration* RenderTargetViewport::GetDefaultRenderTargetConfiguration() {
  return render_target_configuration_.get();
}

}  // namespace ovis
