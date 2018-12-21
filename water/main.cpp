#include <cmath>
#include <iostream>
#include <SDL_assert.h>
#include <glm/gtx/string_cast.hpp>
#include <ovis/gui/gui_controller.hpp>
#include <ovis/gui/gui_renderer.hpp>
#include <ovis/math/marching_cubes.hpp>
#include <ovis/core/file.hpp>
#include <ovis/core/log.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/shader_program.hpp>
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
 public:
  WaterRenderer(Scene* scene) : SceneRenderer(scene, "WaterRenderer") {
    RenderAfter("ClearRenderer");
    RenderBefore("GuiRenderer");
  }

  void CreateResources() override {
    VertexBufferDescription vb_desc;
    vb_desc.size_in_bytes = 0;
    vb_ = std::make_unique<VertexBuffer>(context(), vb_desc);
  }

  void Render() override {}

 private:
  std::unique_ptr<VertexBuffer> vb_;
  std::unique_ptr<ShaderProgram> sp_;
  std::unique_ptr<VertexInput> vi_;
  std::vector<int> level_;
  DrawItem draw_item_;
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        clear_renderer_(this),
        gui_renderer_(this),
        gui_controller_(this) {}

 private:
  ClearRenderer clear_renderer_;
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