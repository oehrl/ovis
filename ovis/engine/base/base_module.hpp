#pragma once

#include <imgui.h>

#include <ovis/engine/module.hpp>

namespace ovis {

class BaseModule final : public Module {
 public:
  BaseModule();
  ~BaseModule() override;

  std::unique_ptr<RenderPass> CreateRenderPass(
      const std::string& render_pass_id,
      RenderPipeline* render_pipeline) override;

  std::unique_ptr<SceneController> CreateSceneController(
      const std::string& scene_controller_id, Scene* scene) override;

  std::unique_ptr<SceneObjectComponent> CreateSceneObjectComponent(
      const std::string& component_id, SceneObject* scene_object) override;

 private:
  ImGuiContext* context_;
};

}  // namespace ovis