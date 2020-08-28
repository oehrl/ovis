#include <imgui.h>

#include <ovis/engine/viewport.hpp>

namespace ovis {

void Viewport::InitializeRenderPipeline(GraphicsContext* context, ResourceManager* resource_manager) {
  render_pipeline_ = std::make_unique<RenderPipeline>(context, resource_manager);
}

void Viewport::Render() {
  const glm::ivec2 viewport_size = GetSize();
  ImGui::GetIO().DisplaySize.x = viewport_size.x;
  ImGui::GetIO().DisplaySize.y = viewport_size.y;

  ImGui::NewFrame();
  if (scene_ != nullptr) {
    scene_->DrawImGui();
  }
  render_pipeline_->DrawImGui();
  ImGui::EndFrame();

  render_pipeline_->Render(scene_);
}

}  // namespace ovis