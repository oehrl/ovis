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
#include "water_renderer.hpp"

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

  void Update(std::chrono::microseconds delta_time) override {
    const auto transform = scene()->camera().transform().CalculateMatrix();

    glm::vec3 local_move_vector = glm::vec3(0.0);

    if (move_forward_) {
      local_move_vector += glm::vec3(0.0, 0.0, 1.0);
    }
    if (move_backward_) {
      local_move_vector -= glm::vec3(0.0, 0.0, 1.0);
    }
    if (move_left_) {
      local_move_vector -= glm::vec3(1.0, 0.0, 0.0);
    }
    if (move_right_) {
      local_move_vector += glm::vec3(1.0, 0.0, 0.0);
    }

    const auto global_move_vector =
        transform * glm::vec4(local_move_vector, 0.0);

    if (left_button_down_) {
      scene()->camera().transform().Translate(
          glm::vec3(global_move_vector) * (delta_time.count() / 1000000.0f));
    }
  }

  bool ProcessEvent(const SDL_Event& event) override {
    switch (event.type) {
      case SDL_MOUSEMOTION:
        if (left_button_down_) {
          yaw_ += event.motion.xrel * 0.01f;
          pitch_ -= event.motion.yrel * 0.01f;
          scene()->camera().transform().SetYawPitchRoll(yaw_, pitch_, 0.0f);
          return true;
        }
        return false;

      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == 1) {
          left_button_down_ = true;
          return true;
        }
        return false;

      case SDL_MOUSEBUTTONUP:
        if (event.button.button == 1) {
          left_button_down_ = false;
          return true;
        }
        return false;

      case SDL_KEYDOWN:
        if (event.key.keysym.scancode == SDL_SCANCODE_W) {
          move_forward_ = true;
          return true;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
          move_backward_ = true;
          return true;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_A) {
          move_left_ = true;
          return true;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_D) {
          move_right_ = true;
          return true;
        }
        return false;

      case SDL_KEYUP:
        if (event.key.keysym.scancode == SDL_SCANCODE_W) {
          move_forward_ = false;
          return true;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
          move_backward_ = false;
          return true;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_A) {
          move_left_ = false;
          return true;
        } else if (event.key.keysym.scancode == SDL_SCANCODE_D) {
          move_right_ = false;
          return true;
        }
        return false;
    }
    return false;
  }

 private:
  bool left_button_down_ = false;
  bool move_forward_ = false;
  bool move_backward_ = false;
  bool move_left_ = false;
  bool move_right_ = false;
  float yaw_ = 0.0f;
  float pitch_ = 0.0f;
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        clear_renderer_(this),
        skybox_renderer_(this),
        water_renderer_(this),
        // gui_renderer_(this),
        // gui_controller_(this),
        movement_controller_(this) {}

 private:
  ClearRenderer clear_renderer_;
  SkyboxRenderer skybox_renderer_;
  // GuiRenderer gui_renderer_;
  // GuiController gui_controller_;
  WaterRenderer water_renderer_;
  MovementController movement_controller_;
};

int main(int argc, char* argv[]) {
  (void)argc;
  Init();

  Window window("Water", 1280, 720);
  window.resource_manager()->AddSearchPath(ExtractDirectory(argv[0]) +
                                           "/resources/");

  window.resource_manager()->Load("skybox.cubemap");

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