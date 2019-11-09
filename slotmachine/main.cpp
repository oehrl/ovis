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

  void Render() override { context()->Clear(); }
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 1024}),
        // current_state_(State::READY_TO_SPIN),
        icons_renderer_(this) {}

 private:
  enum class State { READY_TO_SPIN, SPINNING };

  // State current_state_;
  IconsRenderer icons_renderer_;
};

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  Init();

  Window window("Slot Machine", 1280, 1024);
  window.resource_manager()->AddSearchPath(ExtractDirectory(argv[0]) +
                                           "/resources/");

  GameScene scene;
  window.PushScene(&scene);
  Run();

  Quit();
}