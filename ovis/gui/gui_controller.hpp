#pragma once

#include "ovis/scene/scene_controller.hpp"

namespace ovis {

class GuiController final : public SceneController {
  friend class GuiRenderer;

 public:
  GuiController(Scene* scene);
  virtual ~GuiController() override;

  bool ProcessEvent(const SDL_Event& event) override;

 private:
};

}  // namespace ovis
