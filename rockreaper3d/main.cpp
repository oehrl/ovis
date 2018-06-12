#include <cmath>
#include <iostream>
#include <SDL_assert.h>
#include <glm/gtx/string_cast.hpp>
#include <ovis/gui/gui_controller.hpp>
#include <ovis/gui/gui_renderer.hpp>
#include <ovis/math/marching_cubes.hpp>
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

class TargetCamera {
 public:
  void MoveHorizontally(float delta_angle_in_radians) {
    yaw_ += delta_angle_in_radians;
  }
  void MoveVertically(float delta_angle_in_radians) {
    pitch_ = glm::clamp(pitch_ + delta_angle_in_radians,
                        -0.5f * glm::pi<float>(), 0.5f * glm::pi<float>());
  }

  glm::vec3 CalculatePosition() {
    return target_ -
           distance_to_target_ * glm::vec3{std::cos(pitch_) * std::sin(yaw_),
                                           std::sin(pitch_),
                                           std::cos(pitch_) * std::cos(yaw_)};
  }

  inline float distance_to_target() const { return distance_to_target_; }
  inline void SetDistanceToTarget(float distance_to_target) {
    distance_to_target_ = distance_to_target;
  }

  void Set(Camera* camera) {
    assert(camera != nullptr);
    Transform& transform = camera->transform();
    transform.SetTranslation(CalculatePosition());
    transform.SetYawPitchRoll(yaw_, pitch_, 0.0f);
    // LogV(glm::to_string(CalculatePosition()));
  }

 private:
  glm::vec3 target_         = {0.0f, 0.0f, 0.0f};
  float distance_to_target_ = 1.0f;
  float pitch_              = 0;
  float yaw_                = 0;
};

class CameraController : public SceneController {
 public:
  CameraController(Scene* scene) : SceneController(scene, "CameraController") {
    camera_.SetDistanceToTarget(20.0f);
  }

  void Update(std::chrono::microseconds) override {
    camera_.Set(&scene()->camera());

    // if (ImGui::Button("Hello")) {
    //   LogI("Button Clicked!");
    // }
    ImGui::ShowDemoWindow();
  }

  bool ProcessEvent(const SDL_Event& event) override {
    switch (event.type) {
      case SDL_MOUSEWHEEL:
        LogV("CameraController::ProcessEvent(SDL_MOUSEWHEEL, ", event.wheel.x,
             ", ", event.wheel.y, ")");
        camera_.MoveHorizontally(-event.wheel.x * 0.001f);
        camera_.MoveVertically(-event.wheel.y * 0.001f);
        return false;

      default:
        return false;
    }
  }

 private:
  TargetCamera camera_;
};

// SDL_AssertState HandleAssertion(const SDL_AssertData* data, void*) {
//   LogE("Assertion '", data->condition, "' failed in ", data->function, " ",
//        data->filename, ":", data->linenum);
//   return SDL_ASSERTION_BREAK;
// }

class ClearRenderer : public SceneRenderer {
 public:
  ClearRenderer(Scene* scene) : SceneRenderer(scene, "ClearRenderer") {
    RenderBefore("GuiRenderer");
  }

  void CreateResources() override {
    // const auto t0        = std::chrono::high_resolution_clock::now();
    // const auto positions = MarchingCubes(level_, 16, 16, 16, 5);
    // const auto t1        = std::chrono::high_resolution_clock::now();
    // LogI("Marching Cubes took ",
    //      std::chrono::duration_cast<std::chrono::milliseconds>(t1 -
    //      t0).count(), "ms");
    // LogI("Marching Cubes returned ", positions.size(), " vertices");
  }

  void Render() override {
    // SDL_assert(false);
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

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        camera_controller_(this),
        clear_renderer_(this),
        gui_renderer_(this),
        gui_controller_(this) {}

 private:
  CameraController camera_controller_;
  ClearRenderer clear_renderer_;
  GuiRenderer gui_renderer_;
  GuiController gui_controller_;
};

int main() {
  Init();

  Window window("RocketReaper3D", 1280, 720);
  GameScene scene;
  window.PushScene(&scene);
  Run();

  Quit();
}