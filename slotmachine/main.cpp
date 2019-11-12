#include <cmath>
#include <iostream>
#include <random>
#include <SDL_assert.h>
#include <glm/gtx/string_cast.hpp>
#include <ovis/gui/gui_controller.hpp>
#include <ovis/gui/gui_renderer.hpp>
#include <ovis/math/marching_cubes.hpp>
#include <ovis/core/file.hpp>
#include <ovis/core/log.hpp>
#include <ovis/core/range.hpp>
#include <ovis/graphics/cubemap.hpp>
#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/render_target_configuration.hpp>
#include <ovis/graphics/render_target_texture2d.hpp>
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

class WheelsController : public SceneController {
 public:
  WheelsController(Scene* scene)
      : SceneController(scene, "WheelsController"),
        generator_(random_device_()) {}

  inline float GetWheelPosition(int index) const {
    return wheel_positions_[index];
  }

  inline float GetWheelSpeed(int wheel_index) {
    if (wheel_index < static_cast<int>(current_state_)) {
      return 0.0f;
    } else {
      return WHEEL_SPEED;
    }
  }

  void Update(std::chrono::microseconds delta_time) override {
    last_frame_time_ = delta_time.count() / 1000000.0f;

    for (int i : IRange(3)) {
      wheel_positions_[i] += GetWheelSpeed(i) * last_frame_time_;
    }

    if (timer_value_ <= delta_time.count()) {
      timer_value_ = 0;
      if (current_state_ == State::ALL_SPINNING) {
        current_state_ = State::FIRST_SET;
        wheel_positions_[0] = std::round(wheel_positions_[0]);
        StartTimer();
      } else if (current_state_ == State::FIRST_SET) {
        current_state_ = State::SECOND_SET;
        wheel_positions_[1] = std::round(wheel_positions_[1]);
        StartTimer();
      } else if (current_state_ == State::SECOND_SET) {
        current_state_ = State::ALL_SET;
        wheel_positions_[2] = std::round(wheel_positions_[2]);
        StartTimer();
      }
    } else {
      timer_value_ -= delta_time.count();
    }

    for (auto i : IRange(3)) {
      wheel_positions_[i] = fmod(wheel_positions_[i], ICON_COUNT);
    }
  }

  bool ProcessEvent(const SDL_Event& event) override {
    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
      if (current_state_ == State::ALL_SET) {
        current_state_ = State::ALL_SPINNING;
        StartTimer();
        return true;
      }
    }
    // if (event.type == SDL_KEYDOWN &&
    //     event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
    //   for (int i : IRange(3)) {
    //     wheel_positions_[i] += GetWheelSpeed(i) * last_frame_time_;
    //   }
    // }

    return false;
  }

  void StartTimer() {
    std::uniform_int_distribution dist{1000000, 3000000};
    timer_value_ = dist(generator_);
  }

  float last_frame_time() const { return last_frame_time_; }

 private:
  float wheel_positions_[3] = {0.0f, 0.0f, 0.0f};
  static constexpr float WHEEL_SPEED = 20.0f;  // Icons per second
  static constexpr int ICON_COUNT = 4;

  enum class State {
    ALL_SPINNING = 0,
    FIRST_SET = 1,
    SECOND_SET = 2,
    ALL_SET = 3
  };
  State current_state_ = State::ALL_SET;

  std::random_device random_device_;
  std::mt19937_64 generator_;

  int timer_value_ = 0;
  float last_frame_time_ = 0.0f;
};

class IconsRenderer : public SceneRenderer {
 public:
  IconsRenderer(Scene* scene, WheelsController* wheels_controller)
      : SceneRenderer(scene, "IconsRenderer"),
        wheels_controller_(wheels_controller) {}

  void CreateResources() override {
    ovis::SimpleMeshDescription mesh_desc;
    mesh_desc.primitive_topology = ovis::PrimitiveTopology::TRIANGLE_STRIP;
    mesh_desc.vertex_count = 4;
    mesh_ = std::make_unique<SpriteMesh>(context(), mesh_desc);
    mesh_->vertices()[0].position = glm::vec2(-0.5f, 0.5f);
    mesh_->vertices()[1].position = glm::vec2(-0.5f, -0.5f);
    mesh_->vertices()[2].position = glm::vec2(0.5f, 0.5f);
    mesh_->vertices()[3].position = glm::vec2(0.5f, -0.5f);
    mesh_->UpdateVertexBuffer();

    sprite_shader_program_ =
        scene()->resource_manager()->Load<ovis::ShaderProgram>("sprite.shader");

    vertical_blur_shader_program_ =
        scene()->resource_manager()->Load<ShaderProgram>(
            "vertical_blur.shader");

    icons_ = {
        scene()->resource_manager()->Load<Texture2D>("icons/beer.texture2d"),
        scene()->resource_manager()->Load<Texture2D>("icons/push_up.texture2d"),
        scene()->resource_manager()->Load<Texture2D>("icons/shot.texture2d"),
        scene()->resource_manager()->Load<Texture2D>("icons/squat.texture2d"),
    };

    RenderTargetTexture2DDescription render_target_description;

    LogD("Scene size: ", scene()->size().x, "x", scene()->size().y);
    render_target_description.texture_description.width = scene()->size().x;
    render_target_description.texture_description.height = scene()->size().y;
    render_target_description.texture_description.mip_map_count = 1;
    render_target_description.texture_description.filter = TextureFilter::POINT;
    render_target_description.texture_description.format =
        TextureFormat::RGBA_UINT8;
    render_target_ = std::make_unique<RenderTargetTexture2D>(
        context(), render_target_description);

    ovis::RenderTargetConfigurationDescription rtc_description;
    rtc_description.color_attachments = {render_target_.get()};
    render_target_configuration_ =
        std::make_unique<RenderTargetConfiguration>(context(), rtc_description);
  }

  void Render() override {
    render_target_configuration_->ClearColor({0.0f, 0.0f, 0.0f, 1.0f});

    const auto view_projection_matrix =
        scene()->camera().CalculateViewProjectionMatrix();

    for (auto i : IRange(3)) {
      RenderWheel(i, view_projection_matrix);
    }

    vertical_blur_shader_program_->SetUniform(
        "InverseTextureHeight",
        1.0f / render_target_->texture()->description().height);
    vertical_blur_shader_program_->SetTexture("Texture",
                                              render_target_->texture());

    DrawItem draw_configuration;
    draw_configuration.shader_program = vertical_blur_shader_program_.get();
    mesh_->Draw(draw_configuration);
  }

  void RenderWheel(int wheel_index, const glm::mat4& view_projection_matrix) {
    const float x_offset = (wheel_index - 1) * 420.0f;
    const float wheel_position =
        wheels_controller_->GetWheelPosition(wheel_index);
    const float icon_size = 256;
    const float margin = 20;
    const float total_size = icons_.size() * (icon_size + margin);
    const float wheel_speed = wheels_controller_->GetWheelSpeed(wheel_index);

    constexpr float DESIRED_FPS = 480.0f;
    const float frame_time = wheels_controller_->last_frame_time();

    // Should optimally be power of two!
    const float num_images =
        wheel_speed > 0 ? round(frame_time * DESIRED_FPS) : 1.0f;
    LogD("Num images: ", num_images);
    const float factor = 1.0f / num_images;
    for (auto i : IRange(static_cast<int>(num_images))) {
      for (const auto icon : IndexRange(icons_.begin(), icons_.end())) {
        Transform sprite_transform;
        sprite_transform.SetScale(icon_size);

        const float wheel_position_offset =
            static_cast<float>(i) / num_images *
            wheels_controller_->GetWheelSpeed(wheel_index) * frame_time;

        float position = (static_cast<float>(icon.index()) -
                          (wheel_position - wheel_position_offset)) *
                         (icon_size + margin);
        if (position < total_size * -0.5) {
          position += total_size;
        }
        if (position > total_size * 0.5) {
          position -= total_size;
        }
        sprite_transform.Translate(glm::vec3(x_offset, position, 0.0f));

        sprite_shader_program_->SetUniform(
            "Transform",
            view_projection_matrix * sprite_transform.CalculateMatrix());
        sprite_shader_program_->SetTexture("SpriteTexture", icon.value().get());

        DrawItem draw_configuration;
        draw_configuration.render_target_configuration =
            render_target_configuration_.get();
        draw_configuration.shader_program = sprite_shader_program_.get();
        draw_configuration.blend_state.enabled = true;
        draw_configuration.blend_state.color_function = BlendFunction::ADD;
        draw_configuration.blend_state.source_color_factor =
            SourceBlendFactor::CONSTANT_COLOR;
        draw_configuration.blend_state.constant_color[0] = factor;
        draw_configuration.blend_state.constant_color[1] = factor;
        draw_configuration.blend_state.constant_color[2] = factor;
        draw_configuration.blend_state.constant_color[3] = 1.0f;
        draw_configuration.blend_state.destination_color_factor =
            DestinationBlendFactor::ONE;
        mesh_->Draw(draw_configuration);
      }
    }
  }

 private:
  using SpriteMesh = SimpleMesh<VertexAttribute::POSITION2D>;
  WheelsController* wheels_controller_;
  std::unique_ptr<SpriteMesh> mesh_;
  ovis::ResourcePointer<ovis::ShaderProgram> sprite_shader_program_;
  ovis::ResourcePointer<ovis::ShaderProgram> vertical_blur_shader_program_;
  std::vector<ovis::ResourcePointer<ovis::Texture2D>> icons_;
  std::unique_ptr<ovis::RenderTargetTexture2D> render_target_;
  std::unique_ptr<ovis::RenderTargetConfiguration> render_target_configuration_;
};

class GameScene : public Scene {
 public:
  GameScene()
      : Scene("GameScene", {1280, 720}),
        wheels_controller_(this),
        icons_renderer_(this, &wheels_controller_) {
    camera().SetProjectionType(ProjectionType::ORTHOGRAPHIC);
    camera().SetVerticalFieldOfView(720);
    camera().SetNearClipPlane(0.0f);
    camera().SetFarClipPlane(1.0f);
  }

 private:
  WheelsController wheels_controller_;
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