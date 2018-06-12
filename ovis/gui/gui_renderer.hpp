#pragma once

#include "imgui.h"
#include "ovis/graphics/index_buffer.hpp"
#include "ovis/graphics/shader_program.hpp"
#include "ovis/graphics/texture2d.hpp"
#include "ovis/graphics/vertex_buffer.hpp"
#include "ovis/graphics/vertex_input.hpp"
#include "ovis/scene/scene_renderer.hpp"

namespace ovis {

class GuiRenderer final : public SceneRenderer {
 public:
  GuiRenderer(Scene* scene);
  ~GuiRenderer() override;

  void CreateResources() override;
  void Render() override;

 private:
  void UpdateVertexBuffer(const ImVector<ImDrawVert>& vertices);
  void UpdateIndexBuffer(const ImVector<ImDrawIdx>& indices);

  ImGuiContext* context_;
  std::unique_ptr<ShaderProgram> shader_;
  std::unique_ptr<VertexBuffer> vertices_;
  std::unique_ptr<VertexInput> vertex_input_;
  std::unique_ptr<IndexBuffer> indices_;
  std::unique_ptr<Texture2D> font_atlas_texture_;
};

}  // namespace ovis
