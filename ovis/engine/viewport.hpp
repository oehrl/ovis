#pragma once

#include <memory>
#include <ovis/core/resource_manager.hpp>

#include <ovis/engine/render_pipeline.hpp>
#include <ovis/engine/scene.hpp>

namespace ovis {

class Viewport {
 public:
  Viewport() = default;
  virtual ~Viewport() = default;

  void InitializeRenderPipeline(GraphicsContext* context, ResourceManager* resource_manager);
  virtual glm::ivec2 GetSize() = 0;

  inline Scene* scene() const { return scene_; }
  inline void SetScene(Scene* scene) { scene_ = scene; }

  virtual void Render();

 protected:
  std::unique_ptr<RenderPipeline> render_pipeline_;
  Scene* scene_ = nullptr;
};

}  // namespace ovis