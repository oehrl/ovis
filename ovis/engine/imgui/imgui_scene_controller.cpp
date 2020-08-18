#include <imgui.h>

#include <ovis/engine/imgui/imgui_scene_controller.hpp>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/window.hpp>

namespace ovis {

ImGuiSceneController::ImGuiSceneController(ImGuiContext* context)
    : SceneController("ImGui"), context_(context) {
  ImGui::SetCurrentContext(context_);
}

bool ImGuiSceneController::ProcessEvent(const SDL_Event& event) {
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetCurrentContext(context_);

  switch (event.type) {
    case SDL_MOUSEWHEEL: {
      if (event.wheel.x > 0) io.MouseWheelH += 1;
      if (event.wheel.x < 0) io.MouseWheelH -= 1;
      if (event.wheel.y > 0) io.MouseWheel += 1;
      if (event.wheel.y < 0) io.MouseWheel -= 1;
      return io.WantCaptureMouse;
    }
    case SDL_MOUSEMOTION: {
      io.MousePos = ImVec2(event.motion.x, event.motion.y);
      return io.WantCaptureMouse;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
      if (event.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
      if (event.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
      return io.WantCaptureMouse;
    }
    case SDL_MOUSEBUTTONUP: {
      if (event.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
      if (event.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
      if (event.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = false;
      return io.WantCaptureMouse;
    }
    case SDL_TEXTINPUT: {
      io.AddInputCharactersUTF8(event.text.text);
      return io.WantCaptureKeyboard;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP: {
      int key = event.key.keysym.scancode;
      IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
      io.KeysDown[key] = (event.type == SDL_KEYDOWN);
      io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
      io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
      io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
#ifdef _WIN32
      io.KeySuper = false;
#else
      io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
#endif
      return io.WantCaptureKeyboard;
    }
  }
  return false;
}

}  // namespace ovis
