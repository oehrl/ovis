#pragma once

#include <memory>

#include <ovis/core/resource_manager.hpp>
#include <ovis/graphics/render_target_configuration.hpp>
#include <ovis/graphics/render_target_texture2d.hpp>
#include <ovis/engine/render_pass.hpp>
#include <ovis/engine/scene.hpp>

namespace ovis {

class Viewport {
 public:
  Viewport() = default;
  virtual ~Viewport() = default;

  virtual glm::ivec2 GetSize() = 0;
  virtual RenderTargetConfiguration* GetDefaultRenderTargetConfiguration() = 0;

  inline Scene* scene() const { return scene_; }
  inline void SetScene(Scene* scene) { scene_ = scene; }

  void AddRenderPass(const std::string& render_pass_id);
  void RemoveRenderPass(const std::string& render_pass_id);
  template <typename RenderPassType = RenderPass>
  inline RenderPassType* GetRenderPass(const std::string& render_pass_name) const {
    static_assert(std::is_base_of<RenderPass, RenderPassType>::value, "");
    return down_cast<RenderPassType*>(GetRenderPassInternal(render_pass_name));
  }

  RenderTargetTexture2D* CreateRenderTarget2D(const std::string& id,
                                              const RenderTargetTexture2DDescription& description);
  RenderTarget* GetRenderTarget(const std::string& id);

  std::unique_ptr<RenderTargetConfiguration> CreateRenderTargetConfiguration(
      std::vector<std::string> color_render_target_ids, std::string depth_render_target_id = "");

  virtual void DrawImGui() {}
  virtual void Render(bool render_gui = true);

 protected:
  void SetGraphicsContext(GraphicsContext* graphics_context);
  void SetResourceManager(ResourceManager* resource_manager);

 private:
  void SortRenderPasses();
  RenderPass* GetRenderPassInternal(const std::string& render_pass_name) const;

  GraphicsContext* graphics_context_ = nullptr;
  ResourceManager* resource_manager_ = nullptr;
  Scene* scene_ = nullptr;
  std::unordered_map<std::string, std::unique_ptr<RenderPass>> render_passes_;
  std::vector<RenderPass*> render_pass_order_;
  bool render_passes_sorted_;
  std::unordered_map<std::string, std::unique_ptr<RenderTarget>> render_targets_;
};

}  // namespace ovis