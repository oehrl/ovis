#include <ovis/engine/imgui/imgui_module.hpp>
#include <ovis/engine/imgui/imgui_render_pass.hpp>
#include <ovis/engine/imgui/imgui_scene_controller.hpp>

namespace ovis {

DearImGuiModule::DearImGuiModule() : Module("DearImGuiModule") {
  IMGUI_CHECKVERSION();
  context_ = ImGui::CreateContext();

#if !OVIS_EMSCRIPTEN
  ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
#endif
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  RegisterRenderPass("ImGui");
  RegisterSceneController("ImGui");
}

DearImGuiModule::~DearImGuiModule() {
  ImGui::DestroyContext(context_);
}

std::unique_ptr<RenderPass> DearImGuiModule::CreateRenderPass(
    const std::string& render_pass_id, RenderPipeline*) {
  if (render_pass_id == "ImGui") {
    return std::make_unique<ImGuiRenderPass>(context_);
  } else {
    return nullptr;
  }
}

std::unique_ptr<SceneController> DearImGuiModule::CreateSceneController(
    const std::string& scene_controller_id, Scene*) {
  if (scene_controller_id == "ImGui") {
    return std::make_unique<ImGuiSceneController>(context_);
  } else {
    return nullptr;
  }
}

}  // namespace ovis
