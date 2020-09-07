#pragma once

#include <ovis/core/resource.hpp>
#include <ovis/graphics/static_mesh.hpp>
#include <ovis/engine/render_pass.hpp>

namespace ovis {

class SpriteRenderer : public RenderPass {
 public:
  SpriteRenderer();

  void CreateResources() override;

  void Render(Scene* scene) override;

 private:
  ovis::ResourcePointer<ovis::ShaderProgram> shader_program_;
  std::unique_ptr<ovis::VertexBuffer> vertex_buffer_;
  std::unique_ptr<ovis::VertexInput> vertex_input_;
};

}  // namespace ovis