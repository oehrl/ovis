#include <ovis/engine/base/base_module.hpp>
#include <ovis/engine/base/imgui_render_pass.hpp>
#include <ovis/engine/base/imgui_scene_controller.hpp>
#include <ovis/engine/base/transform2d_component.hpp>

namespace ovis {

BaseModule::BaseModule() : Module("BaseModule") {
  IMGUI_CHECKVERSION();
  context_ = ImGui::CreateContext();

#if !OVIS_EMSCRIPTEN
  ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
#endif
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::GetIO().IniFilename = "/user/imgui.ini";

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

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

  RegisterRenderPass("ImGui");
  RegisterSceneController("ImGui");
  RegisterSceneObjectComponent("Transform2D");
}

BaseModule::~BaseModule() {
  ImGui::DestroyContext(context_);
}

std::unique_ptr<RenderPass> BaseModule::CreateRenderPass(const std::string& render_pass_id, RenderPipeline*) {
  if (render_pass_id == "ImGui") {
    return std::make_unique<ImGuiRenderPass>(context_);
  } else {
    return nullptr;
  }
}

std::unique_ptr<SceneController> BaseModule::CreateSceneController(const std::string& scene_controller_id, Scene*) {
  if (scene_controller_id == "ImGui") {
    return std::make_unique<ImGuiSceneController>(context_);
  } else {
    return nullptr;
  }
}

std::unique_ptr<SceneObjectComponent> BaseModule::CreateSceneObjectComponent(const std::string& component_id,
                                                                             SceneObject* scene_object) {
  if (component_id == "Transform2D") {
    return std::make_unique<Transform2DComponent>();
  } else {
    return nullptr;
  }
}

}  // namespace ovis
