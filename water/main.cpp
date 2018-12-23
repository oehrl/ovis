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

class ClearRenderer : public SceneRenderer {
 public:
  ClearRenderer(Scene* scene) : SceneRenderer(scene, "ClearRenderer") {
    RenderBefore("GuiRenderer");
  }

  void Render() override {
    SDL_assert(context() != nullptr);
    this->context()->Clear();
  }

 private:
  std::unique_ptr<VertexBuffer> vb_;
  std::unique_ptr<ShaderProgram> sp_;
  std::unique_ptr<VertexInput> vi_;
  std::vector<int> level_;
  DrawItem draw_item_;
};

class WaterRenderer : public SceneRenderer {
  using MeshType = SimpleMesh<VertexAttribute::POSITION2D>;

 public:
  WaterRenderer(Scene* scene) : SceneRenderer(scene, "WaterRenderer") {
    RenderAfter("ClearRenderer");
    RenderBefore("GuiRenderer");
  }

  void CreateResources() override {
    ovis::ShaderProgramDescription sp_desc;
    sp_desc.vertex_shader_source =
        "#define M_PI 3.1415926535897932384626433832795\n"
        "in vec2 a_Position;\n"
        "out vec3 vs_TexCoord;\n"
        "void main() {\n"
        "  vs_TexCoord = vec3(a_Position, 1.0);\n"
        "  gl_Position = vec4(a_Position, 0.0, 1.0);\n"
        "}\n";
    sp_desc.fragment_shader_source =
        "uniform samplerCube u_Skybox;\n"
        "in vec3 vs_TexCoord;\n"
        "void main() {\n"
        "  gl_FragColor = textureCube(u_Skybox, vs_TexCoord);\n"
        "}\n";
    shader_program_ = std::make_unique<ovis::ShaderProgram>(context(), sp_desc);

    SimpleMeshDescription mesh_desc;
    mesh_desc.primitive_topology = PrimitiveTopology::TRIANGLE_STRIP;
    mesh_desc.shader_program = shader_program_.get();
    mesh_desc.vertex_count = 4;
    mesh_ = std::make_unique<MeshType>(context(), mesh_desc);
    mesh_->vertices()[0].position = glm::vec2(-1.0, 1.0);
    mesh_->vertices()[1].position = glm::vec2(-1.0, -1.0);
    mesh_->vertices()[2].position = glm::vec2(1.0, 1.0);
    mesh_->vertices()[3].position = glm::vec2(1.0, -1.0);
    mesh_->UpdateVertexBuffer();

    scene()->resource_manager()->Load(
        "/Users/Simon/Documents/ovis-cpp/resources/skybox.cubemap");
    skybox_texture_ = scene()->resource_manager()->GetResource<Cubemap>(
        "/Users/Simon/Documents/ovis-cpp/resources/skybox.cubemap");
  }

  void Render() override {
    shader_program_->SetTexture("Skybox", skybox_texture_->get());
    mesh_->Draw();
  }

 private:
  std::unique_ptr<ShaderProgram> shader_program_;
  std::unique_ptr<MeshType> mesh_;
  std::shared_ptr<Resource<Cubemap>> skybox_texture_;
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        clear_renderer_(this),
        water_renderer_(this),
        gui_renderer_(this),
        gui_controller_(this) {}

 private:
  ClearRenderer clear_renderer_;
  WaterRenderer water_renderer_;
  GuiRenderer gui_renderer_;
  GuiController gui_controller_;
};

int main() {
  Init();

  Window window("Water", 1280, 720);

  window.resource_manager()->RegisterFileLoader(
      ".txt",
      [](ovis::ResourceManager* resource_manager, const std::string& filename) {
        resource_manager->RegisterResource<std::string>(
            filename, ovis::LoadTextFile(filename));
        return true;
      });

  // window.resource_manager()->Load("test.txt");

  GameScene scene;
  window.PushScene(&scene);
  Run();

  Quit();
}