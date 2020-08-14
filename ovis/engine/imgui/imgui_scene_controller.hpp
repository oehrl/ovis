#pragma once

#include <imgui.h>

#include <ovis/engine/scene_controller.hpp>

namespace ovis {

class ImGuiSceneController : public ovis::SceneController {
 public:
  ImGuiSceneController(ImGuiContext* context);

  void BeforeUpdate() override;
  void AfterUpdate() override;
  void Update(std::chrono::microseconds delta_time) override;
  bool ProcessEvent(const SDL_Event& event) override;

 private:
  ImGuiContext* context_;
};

}  // namespace ovis
