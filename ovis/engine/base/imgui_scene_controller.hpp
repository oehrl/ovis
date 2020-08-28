#pragma once

#include <imgui.h>

#include <ovis/engine/scene_controller.hpp>

namespace ovis {

class ImGuiSceneController : public ovis::SceneController {
 public:
  ImGuiSceneController(ImGuiContext* context);

  bool ProcessEvent(const SDL_Event& event) override;

 private:
  ImGuiContext* context_;
};

}  // namespace ovis
