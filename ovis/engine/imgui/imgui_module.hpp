#pragma once

#include <imgui.h>

#include <ovis/engine/module.hpp>

namespace ovis {

class DearImGuiModule final : public Module {
 public:
  DearImGuiModule();
  ~DearImGuiModule() override;

  std::unique_ptr<RenderPass> CreateRenderPass(
      const std::string& render_pass_id, RenderPipeline* render_pipeline) override;

  std::unique_ptr<SceneController> CreateSceneController(
      const std::string& scene_controller_id, Scene* scene) override;

 private:
  ImGuiContext* context_;
};

}  // namespace ovis