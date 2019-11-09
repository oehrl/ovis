#include "ovis/gui/gui_controller.hpp"
#include "imgui.h"

namespace ovis {

namespace {

int GetMouseButtonIndex(uint8_t button) {
  switch (button) {
    case SDL_BUTTON_LEFT:
      return 0;
    case SDL_BUTTON_RIGHT:
      return 1;
    case SDL_BUTTON_MIDDLE:
      return 2;
    case SDL_BUTTON_X1:
      return 3;
    case SDL_BUTTON_X2:
      return 4;
  }
  assert(false);
  return 0;
}

}  // namespace

GuiController::GuiController(Scene* scene)
    : SceneController(scene, "GuiController") {}

GuiController::~GuiController() {}

bool GuiController::ProcessEvent(const SDL_Event& event) {
  ImGuiIO& gui_io = ImGui::GetIO();

  switch (event.type) {
    case SDL_MOUSEMOTION:
      gui_io.MousePos.x = event.motion.x;
      gui_io.MousePos.y = event.motion.y;
      return true;

    case SDL_MOUSEBUTTONDOWN:
      gui_io.MousePos.x = event.button.x;
      gui_io.MousePos.y = event.button.y;
      gui_io.MouseDown[GetMouseButtonIndex(event.button.button)] = true;
      return true;

    case SDL_MOUSEBUTTONUP:
      gui_io.MousePos.x = event.button.x;
      gui_io.MousePos.y = event.button.y;
      gui_io.MouseDown[GetMouseButtonIndex(event.button.button)] = false;
      return true;

    case SDL_MOUSEWHEEL:
      if (event.wheel.y > 0) {
        gui_io.MouseWheel += 1;
      } else if (event.wheel.y < 0) {
        gui_io.MouseWheel -= 1;
      }
      return true;
  }
  return false;
}

}  // namespace ovis