#pragma once

#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/static_mesh.hpp>
#include <ovis/graphics/texture2d.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_renderer.hpp>

class WaterRenderer : public ovis::SceneRenderer {
  using MeshType = ovis::SimpleMesh<ovis::VertexAttribute::POSITION3D>;

 public:
  WaterRenderer(ovis::Scene* scene) : SceneRenderer(scene, "WaterRenderer") {
    RenderAfter("SkyboxRenderer");
    RenderBefore("GuiRenderer");
  }

  void CreateResources() override {
    ovis::SimpleMeshDescription mesh_desc;
    mesh_desc.primitive_topology = ovis::PrimitiveTopology::TRIANGLE_STRIP;
    mesh_desc.vertex_count = 4;
    mesh_ = std::make_unique<MeshType>(context(), mesh_desc);
    const float plane_size = 100.0;
    mesh_->vertices()[0].position = plane_size * glm::vec3(-1.0f, 0.0f, 1.0f);
    mesh_->vertices()[1].position = plane_size * glm::vec3(-1.0f, 0.0f, -1.0f);
    mesh_->vertices()[2].position = plane_size * glm::vec3(1.0f, 0.0f, 1.0f);
    mesh_->vertices()[3].position = plane_size * glm::vec3(1.0f, 0.0f, -1.0f);
    mesh_->UpdateVertexBuffer();

    skybox_texture_ = scene()->resource_manager()->GetResource<ovis::Cubemap>(
        "skybox.cubemap");

    shader_program_ =
        scene()->resource_manager()->Load<ovis::ShaderProgram>("water.shader");

    ocean_normals1_texture_ =
        scene()->resource_manager()->Load<ovis::Texture2D>(
            "ocean_normals1.texture2d");
  }

  void Render() override {
    shader_program_->SetUniform(
        "ViewProjectionMatrix",
        scene()->camera().CalculateViewProjectionMatrix());
    shader_program_->SetTexture("Skybox", skybox_texture_.get());
    shader_program_->SetTexture("OceanNormals1", ocean_normals1_texture_.get());
    shader_program_->SetUniform("CameraPosition",
                                scene()->camera().transform().translation());
    mesh_->Draw(shader_program_.get());
  }

 private:
  std::unique_ptr<MeshType> mesh_;
  ovis::ResourcePointer<ovis::ShaderProgram> shader_program_;
  ovis::ResourcePointer<ovis::Cubemap> skybox_texture_;
  ovis::ResourcePointer<ovis::Texture2D> ocean_normals1_texture_;
};
