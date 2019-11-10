#include <cmath>
#include <iostream>
#include <SDL_assert.h>
#include <glm/gtx/string_cast.hpp>
#include <ovis/gui/gui_controller.hpp>
#include <ovis/gui/gui_renderer.hpp>
#include <ovis/math/marching_cubes.hpp>
#include <ovis/core/file.hpp>
#include <ovis/core/log.hpp>
#include <ovis/graphics/cubemap.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/graphics/static_mesh.hpp>
#include <ovis/graphics/vertex_buffer.hpp>
#include <ovis/graphics/vertex_input.hpp>
#include <ovis/scene/scene.hpp>
#include <ovis/scene/scene_controller.hpp>
#include <ovis/scene/scene_renderer.hpp>
#include <ovis/engine/engine.hpp>
#include <ovis/engine/window.hpp>

using namespace ovis;

class IconsRenderer : public SceneRenderer {
 public:
  IconsRenderer(Scene* scene) : SceneRenderer(scene, "IconsRenderer") {}

  void CreateResources() override {
    ovis::SimpleMeshDescription mesh_desc;
    mesh_desc.primitive_topology = ovis::PrimitiveTopology::TRIANGLE_STRIP;
    mesh_desc.vertex_count = 4;
    mesh_ = std::make_unique<SpriteMesh>(context(), mesh_desc);
    mesh_->vertices()[0].position = glm::vec2(-1.0f, 1.0f);
    mesh_->vertices()[1].position = glm::vec2(-1.0f, -1.0f);
    mesh_->vertices()[2].position = glm::vec2(1.0f, 1.0f);
    mesh_->vertices()[3].position = glm::vec2(1.0f, -1.0f);
    mesh_->UpdateVertexBuffer();

    shader_program_ =
        scene()->resource_manager()->Load<ovis::ShaderProgram>("sprite.shader");

    beer_ =
        scene()->resource_manager()->Load<Texture2D>("icons/beer.texture2d");
  }

  void Render() override {
    context()->Clear();

    const auto view_projection_matrix =
        scene()->camera().CalculateViewProjectionMatrix();

    Transform sprite_transform;
    sprite_transform.SetScale(400);

    shader_program_->SetUniform(
        "Transform",
        view_projection_matrix * sprite_transform.CalculateMatrix());
    shader_program_->SetTexture("SpriteTexture", beer_.get());
    mesh_->Draw(shader_program_.get());
  }

 private:
  using SpriteMesh = SimpleMesh<VertexAttribute::POSITION2D>;
  std::unique_ptr<SpriteMesh> mesh_;
  ovis::ResourcePointer<ovis::ShaderProgram> shader_program_;
  ovis::ResourcePointer<ovis::Texture2D> beer_;
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        // current_state_(State::READY_TO_SPIN),
        icons_renderer_(this) {
    camera().SetProjectionType(ProjectionType::ORTHOGRAPHIC);
    camera().SetVerticalFieldOfView(1280);
    camera().SetAspectRadio(720.0f / 1280.0f);
    camera().SetNearClipPlane(0.0f);
    camera().SetFarClipPlane(1.0f);
  }

 private:
  enum class State { READY_TO_SPIN, SPINNING };

  // State current_state_;
  IconsRenderer icons_renderer_;
};

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Init();

  Window window("Slot Machine", 1280, 720);
  window.resource_manager()->AddSearchPath(ExtractDirectory(argv[0]) +
                                           "/resources/");

  GameScene scene;
  window.PushScene(&scene);
  Run();

  Quit();
}