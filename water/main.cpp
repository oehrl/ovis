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

#include "skybox_renderer.hpp"

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

class MovementController : public SceneController {
 public:
  MovementController(Scene* scene)
      : SceneController(scene, "MovementController") {}

  bool ProcessEvent(const SDL_Event& event) override {
    switch (event.type) {
      case SDL_MOUSEMOTION:
        if (left_button_down_) {
          yaw_ -= event.motion.xrel * 0.01f;
          pitch_ += event.motion.yrel * 0.01f;
          scene()->camera().transform().SetYawPitchRoll(yaw_, pitch_, 0.0f);
        }
        return true;

      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == 1) {
          left_button_down_ = true;
        }
        return true;

      case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1) {
          left_button_down_ = false;
        }
        return true;
    }
    return false;
  }

 private:
  bool left_button_down_ = false;
  float yaw_ = 0.0f;
  float pitch_ = 0.0f;
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        clear_renderer_(this),
        skybox_renderer_(this),
        // gui_renderer_(this),
        // gui_controller_(this),
        movement_controller_(this) {}

 private:
  ClearRenderer clear_renderer_;
  SkyboxRenderer skybox_renderer_;
  // GuiRenderer gui_renderer_;
  // GuiController gui_controller_;
  MovementController movement_controller_;
};

int main() {
  Init();

  Window window("Water", 1280, 720);
  window.resource_manager()->AddSearchPath("");

  // window.resource_manager()->RegisterFileLoader(
  //     ".txt",
  //     [](ovis::ResourceManager* resource_manager, const std::string&
  //     filename) {
  //       resource_manager->RegisterResource<std::string>(
  //           filename, ovis::LoadTextFile(filename));
  //       return true;
  //     });

  // window.resource_manager()->Load("test.txt");

  GameScene scene;
  window.PushScene(&scene);
  Run();

  Quit();
}