#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <ovis/rendering2d/sprite_component.hpp>
#include <ovis/rendering2d/sprite_renderer.hpp>

#include <ovis/graphics/graphics_context.hpp>
#include <ovis/graphics/render_target_configuration.hpp>
#include <ovis/graphics/shader_program.hpp>
#include <ovis/engine/base/transform2d_component.hpp>
#include <ovis/engine/viewport.hpp>

namespace ovis {

SpriteRenderer::SpriteRenderer() : RenderPass("SpriteRenderer") {}

void SpriteRenderer::CreateResources() {
  shader_program_ = resource_manager()->Load<ovis::ShaderProgram>("sprite.shader");

  struct Vertex {
    glm::vec2 position;
    glm::vec2 texture_coordinates;
  };

  Vertex vertices[] = {{{-0.5f, 0.5f}, {0.0f, 1.0f}},
                       {{-0.5f, -0.5f}, {0.0f, 0.0f}},
                       {{0.5f, 0.5f}, {1.0f, 1.0f}},
                       {{0.5f, -0.5f}, {1.0f, 0.0f}}};

  VertexBufferDescription vb_desc;
  vb_desc.vertex_size_in_bytes = 4 * sizeof(float);
  vb_desc.size_in_bytes = 4 * vb_desc.vertex_size_in_bytes;
  vertex_buffer_ = std::make_unique<ovis::VertexBuffer>(context(), vb_desc, vertices);

  VertexInputDescription vi_desc;
  vi_desc.vertex_buffers = {vertex_buffer_.get()};
  vi_desc.vertex_attributes = {
      {*shader_program_->GetAttributeLocation("Position"), 0, 0, VertexAttributeType::FLOAT32_VECTOR2},
      {*shader_program_->GetAttributeLocation("TextureCoordinates"), 8, 0, VertexAttributeType::FLOAT32_VECTOR2}};
  vertex_input_ = std::make_unique<VertexInput>(context(), vi_desc);
}

void SpriteRenderer::Render(Scene* scene) {
  viewport()->GetDefaultRenderTargetConfiguration()->ClearColor(0, glm::vec4(1.0, 1.0, 0.0, 1.0));

  scene->camera().SetProjectionType(ProjectionType::ORTHOGRAPHIC);
  scene->camera().SetNearClipPlane(0.0f);
  scene->camera().SetNearClipPlane(1.0f);
  scene->camera().SetVerticalFieldOfView(100.0f);
  scene->camera().SetAspectRadio(1.0f);

  // const glm::mat4 projection_matrix = scene->camera().CalculateProjectionMatrix();
  // LogV("{}", glm::to_string(projection_matrix));
  const glm::mat4 projection_matrix = glm::scale(glm::vec3(1.0f / 512, 1.0f / 512, 1.0f));

  DrawItem draw_item;
  draw_item.shader_program = shader_program_.get();
  draw_item.vertex_input = vertex_input_.get();
  draw_item.primitive_topology = PrimitiveTopology::TRIANGLE_STRIP;
  draw_item.count = 4;
  draw_item.render_target_configuration = viewport()->GetDefaultRenderTargetConfiguration();

  const auto objects_with_sprites = scene->GetSceneObjectsWithComponent("Sprite");

  for (SceneObject* object : objects_with_sprites) {
    SpriteComponent* sprite = object->GetComponent<SpriteComponent>("Sprite");
    const glm::mat4 size_matrix = glm::scale(glm::vec3(sprite->size(), 1.0f));
    const glm::vec4 color = sprite->color();

    Transform2DComponent* transform = object->GetComponent<Transform2DComponent>("Transform2D");
    const glm::mat4 world_view_projection =
        transform ? projection_matrix * transform->transform()->CalculateMatrix() * size_matrix
                  : projection_matrix * size_matrix;

    shader_program_->SetUniform("WorldViewProjection", world_view_projection);
    shader_program_->SetUniform("Color", color);
    context()->Draw(draw_item);
  }
}

}  // namespace ovis
