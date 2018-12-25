#pragma once

#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/static_mesh.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_renderer.hpp>

class SkyboxRenderer : public ovis::SceneRenderer {
  using MeshType = ovis::SimpleMesh<ovis::VertexAttribute::POSITION3D>;

 public:
  SkyboxRenderer(ovis::Scene* scene) : SceneRenderer(scene, "SkyboxRenderer") {
    RenderAfter("ClearRenderer");
    RenderBefore("GuiRenderer");
  }

  void CreateResources() override {
    ovis::SimpleMeshDescription mesh_desc;
    mesh_desc.primitive_topology = ovis::PrimitiveTopology::TRIANGLE_LIST;
    mesh_desc.vertex_count = 36;
    mesh_ = std::make_unique<MeshType>(context(), mesh_desc);
    mesh_->vertices()[0].position = glm::vec3(-1.0f, 1.0f, -1.0f);
    mesh_->vertices()[1].position = glm::vec3(-1.0f, -1.0f, -1.0f);
    mesh_->vertices()[2].position = glm::vec3(1.0f, -1.0f, -1.0f);
    mesh_->vertices()[3].position = glm::vec3(1.0f, -1.0f, -1.0f);
    mesh_->vertices()[4].position = glm::vec3(1.0f, 1.0f, -1.0f);
    mesh_->vertices()[5].position = glm::vec3(-1.0f, 1.0f, -1.0f);

    mesh_->vertices()[6].position = glm::vec3(-1.0f, -1.0f, 1.0f);
    mesh_->vertices()[7].position = glm::vec3(-1.0f, -1.0f, -1.0f);
    mesh_->vertices()[8].position = glm::vec3(-1.0f, 1.0f, -1.0f);
    mesh_->vertices()[9].position = glm::vec3(-1.0f, 1.0f, -1.0f);
    mesh_->vertices()[10].position = glm::vec3(-1.0f, 1.0f, 1.0f);
    mesh_->vertices()[11].position = glm::vec3(-1.0f, -1.0f, 1.0f);

    mesh_->vertices()[12].position = glm::vec3(1.0f, -1.0f, -1.0f);
    mesh_->vertices()[13].position = glm::vec3(1.0f, -1.0f, 1.0f);
    mesh_->vertices()[14].position = glm::vec3(1.0f, 1.0f, 1.0f);
    mesh_->vertices()[15].position = glm::vec3(1.0f, 1.0f, 1.0f);
    mesh_->vertices()[16].position = glm::vec3(1.0f, 1.0f, -1.0f);
    mesh_->vertices()[17].position = glm::vec3(1.0f, -1.0f, -1.0f);

    mesh_->vertices()[18].position = glm::vec3(-1.0f, -1.0f, 1.0f);
    mesh_->vertices()[19].position = glm::vec3(-1.0f, 1.0f, 1.0f);
    mesh_->vertices()[20].position = glm::vec3(1.0f, 1.0f, 1.0f);
    mesh_->vertices()[21].position = glm::vec3(1.0f, 1.0f, 1.0f);
    mesh_->vertices()[22].position = glm::vec3(1.0f, -1.0f, 1.0f);
    mesh_->vertices()[23].position = glm::vec3(-1.0f, -1.0f, 1.0f);

    mesh_->vertices()[24].position = glm::vec3(-1.0f, 1.0f, -1.0f);
    mesh_->vertices()[25].position = glm::vec3(1.0f, 1.0f, -1.0f);
    mesh_->vertices()[26].position = glm::vec3(1.0f, 1.0f, 1.0f);
    mesh_->vertices()[27].position = glm::vec3(1.0f, 1.0f, 1.0f);
    mesh_->vertices()[28].position = glm::vec3(-1.0f, 1.0f, 1.0f);
    mesh_->vertices()[29].position = glm::vec3(-1.0f, 1.0f, -1.0f);

    mesh_->vertices()[30].position = glm::vec3(-1.0f, -1.0f, -1.0f);
    mesh_->vertices()[31].position = glm::vec3(-1.0f, -1.0f, 1.0f);
    mesh_->vertices()[32].position = glm::vec3(1.0f, -1.0f, -1.0f);
    mesh_->vertices()[33].position = glm::vec3(1.0f, -1.0f, -1.0f);
    mesh_->vertices()[34].position = glm::vec3(-1.0f, -1.0f, 1.0f);
    mesh_->vertices()[35].position = glm::vec3(1.0f, -1.0f, 1.0f);
    mesh_->UpdateVertexBuffer();

    skybox_texture_ = scene()->resource_manager()->GetResource<ovis::Cubemap>(
        "skybox.cubemap");

    shader_program_ =
        scene()->resource_manager()->Load<ovis::ShaderProgram>("skybox.shader");
  }

  void Render() override {
    shader_program_->SetUniform(
        "ViewMatrix", scene()->camera().transform().CalculateInverseMatrix());
    shader_program_->SetUniform("ProjectionMatrix",
                                scene()->camera().CalculateProjectionMatrix());
    shader_program_->SetTexture("Skybox", skybox_texture_.get());
    mesh_->Draw(shader_program_.get());
  }

 private:
  std::unique_ptr<MeshType> mesh_;
  ovis::ResourcePointer<ovis::ShaderProgram> shader_program_;
  ovis::ResourcePointer<ovis::Cubemap> skybox_texture_;
};
