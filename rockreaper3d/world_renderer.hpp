#pragma once

#include <memory>
#include <rockreaper3d/world.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/vertex_buffer.hpp>
#include <ovis/graphics/vertex_input.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_renderer.hpp>

namespace rr3d {

class WorldRenderer : public ovis::SceneRenderer {
 public:
  WorldRenderer(ovis::Scene* scene, World* world);

  void CreateResources() override;
  void Render() override;

 private:
  void UpdateVertexBuffer();

  World* world_;
  std::unique_ptr<ovis::VertexBuffer> vb_;
  std::unique_ptr<ovis::ShaderProgram> sp_;
  std::unique_ptr<ovis::VertexInput> vi_;
  ovis::DrawItem draw_item_;
};

}  // namespace rr3d
