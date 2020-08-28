#include <memory>

#include <glm/vec2.hpp>
#include <imgui.h>

#include <ovis/core/resource.hpp>
#include <ovis/graphics/index_buffer.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/texture2d.hpp>
#include <ovis/graphics/vertex_buffer.hpp>
#include <ovis/graphics/vertex_input.hpp>
#include <ovis/engine/render_pass.hpp>

namespace ovis {

class ImGuiRenderPass : public ovis::RenderPass {
 public:
  ImGuiRenderPass(ImGuiContext* context);
  virtual ~ImGuiRenderPass();

  void CreateResources() override;

  void Render(ovis::Scene* scene) override;

 private:
  ImGuiContext* context_;
  std::unique_ptr<ovis::Texture2D> font_texture_;

  ovis::ResourcePointer<ovis::ShaderProgram> shader_program_;
  std::unique_ptr<ovis::VertexBuffer> vertex_buffer_;
  std::unique_ptr<ovis::VertexInput> vertex_input_;
  std::unique_ptr<ovis::IndexBuffer> index_buffer_;
};

}  // namespace ovis
