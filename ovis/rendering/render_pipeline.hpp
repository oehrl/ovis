#pragma once

#include <unordered_map>
#include <vector>
#include <ovis/core/down_cast.hpp>
#include <ovis/core/class.hpp>

namespace ovis {

class GraphicsContext;
class RenderPass;
class Scene;

class RenderPipeline {
  MAKE_NON_COPY_OR_MOVABLE(RenderPipeline);

 public:
  RenderPipeline(GraphicsContext* graphics_context);

  void AddRenderPass(RenderPass* render_pass);
  void RemoveRenderPass(RenderPass* render_pass);
  template <typename RenderPassType = RenderPass>
  inline RenderPassType* GetRenderPass(
      const std::string& render_pass_name) const {
    static_assert(std::is_base_of<RenderPass, RenderPassType>::value, "");
    return down_cast<RenderPassType*>(GetRenderPassInternal(render_pass_name));
  }

  void Render(Scene* scene);

 private:
  void SortRenderPasses();

  RenderPass* GetRenderPassInternal(const std::string& render_pass_name) const;

  GraphicsContext* graphics_context_;
  std::unordered_map<std::string, RenderPass*> render_passes_;
  std::vector<RenderPass*> render_pass_order_;
  bool render_passes_sorted_;
};

}  // namespace ovis
