#include <ovis/engine/imgui/imgui_scene_controller.hpp>

#include <imgui.h>
#include <ovis/engine/scene.hpp>
#include <ovis/engine/window.hpp>

namespace ovis {

ImGuiSceneController::ImGuiSceneController(ovis::Scene* scene,
                                           ImGuiContext* context)
    : SceneController(scene, "ImGui"), context_(context) {
  ImGui::SetCurrentContext(context_);

  // Setup back-end capabilities flags
  ImGuiIO& io = ImGui::GetIO();
  //   io.BackendFlags |=
  //       ImGuiBackendFlags_HasMouseCursors;  // We can honor GetMouseCursor()
  //                                           // values (optional)
  //   io.BackendFlags |=
  //       ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos
  //                                          // requests (optional, rarely
  //                                          used)

  // Keyboard mapping. ImGui will use those indices to peek into the
  // io.KeysDown[] array.
  io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
  io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
  io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
  io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
  io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
  io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
  io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
  io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
  io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
  io.KeyMap[ImGuiKey_KeyPadEnter] = SDL_SCANCODE_KP_ENTER;
  io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
  io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
  io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
  io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
  io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
  io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
}

void ImGuiSceneController::BeforeUpdate() {
  ImGui::SetCurrentContext(context_);
  ImGui::GetIO().DisplaySize.x = scene()->window()->width();
  ImGui::GetIO().DisplaySize.y = scene()->window()->height();
  ImGui::NewFrame();
}

void ImGuiSceneController::AfterUpdate() {
  SDL_assert(ImGui::GetCurrentContext() == context_);
  ImGui::EndFrame();
}

void ImGuiSceneController::Update(std::chrono::microseconds delta_time) {
  // int mx, my;
  // Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
  // io.MouseDown[0] =
  //     g_MousePressed[0] ||
  //     (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) !=
  //         0;  // If a mouse press event came, always pass it as "mouse held
  //         this
  //             // frame", so we don't miss click-release events that are
  //             shorter
  //             // than 1 frame.
  // io.MouseDown[1] =
  //     g_MousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) !=
  //     0;
  // io.MouseDown[2] =
  //     g_MousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) !=
  //     0;
  // g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;

  // if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
  // io.MousePos = ImVec2((float)mx, (float)my);
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
