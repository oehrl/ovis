#pragma once

#include <unordered_map>
#include <vector>

#include <ovis/core/class.hpp>
#include <ovis/core/down_cast.hpp>

#include <ovis/graphics/render_target.hpp>
#include <ovis/graphics/render_target_texture2d.hpp>

namespace ovis {

class GraphicsContext;
class RenderPass;
class ResourceManager;
class Scene;

class RenderPipeline final {
  friend class RenderPass;
  MAKE_NON_COPY_OR_MOVABLE(RenderPipeline);

 public:
  RenderPipeline(GraphicsContext* graphics_context,
                 ResourceManager* resource_manager);
  ~RenderPipeline();

  void AddRenderPass(RenderPass* render_pass);
  void RemoveRenderPass(RenderPass* render_pass);
  template <typename RenderPassType = RenderPass>
  inline RenderPassType* GetRenderPass(
      const std::string& render_pass_name) const {
    static_assert(std::is_base_of<RenderPass, RenderPassType>::value, "");
    return down_cast<RenderPassType*>(GetRenderPassInternal(render_pass_name));
  }

  RenderTargetTexture2D* CreateRenderTarget2D(
      const std::string& id,
      const RenderTargetTexture2DDescription& description);
  RenderTarget* GetRenderTarget(const std::string& id);

  std::unique_ptr<RenderTargetConfiguration> CreateRenderTargetConfiguration(
      std::vector<std::string> color_render_target_ids,
      std::string depth_render_target_id = "");

  void Render(Scene* scene);

 private:
  void SortRenderPasses();

  RenderPass* GetRenderPassInternal(const std::string& render_pass_name) const;

  GraphicsContext* graphics_context_;
  ResourceManager* resource_manager_;
  std::unordered_map<std::string, RenderPass*> render_passes_;
  std::vector<RenderPass*> render_pass_order_;
  bool render_passes_sorted_;
  std::unordered_map<std::string, std::unique_ptr<RenderTarget>>
      render_targets_;
};

}  // namespace ovis
