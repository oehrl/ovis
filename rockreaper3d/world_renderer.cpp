#include <rockreaper3d/world_renderer.hpp>
#include <ovis/core/log.hpp>

namespace rr3d {

WorldRenderer::WorldRenderer(ovis::Scene* scene, World* world)
    : ovis::SceneRenderer(scene, "WorldRenderer"), world_(world) {
  assert(world != nullptr);

  draw_item_.depth_buffer_state.test_enabled = true;
}

void WorldRenderer::CreateResources() {
  ovis::ShaderProgramDescription sp_desc;
  sp_desc.vertex_shader_source =
      "#define M_PI 3.1415926535897932384626433832795\n"
      "uniform mat4 u_LocalToProjectionMatrix;\n"
      "uniform vec3 u_Exents;\n"
      "attribute vec3 a_Position;\n"
      "attribute vec2 a_Normal;\n"
      "varying vec3 vs_Normal;\n"
      "void main() {\n"
      "  float theta = a_Normal[0] * 2.0 * M_PI;\n"
      "  float phi = a_Normal[1] * M_PI;\n"
      "  float sin_theta = sin(theta);\n"
      "  float cos_theta = cos(theta);\n"
      "  float sin_phi = sin(phi);\n"
      "  float cos_phi = cos(phi);\n"
      "  vs_Normal = vec3(cos_theta * sin_phi, cos_phi, sin_theta * "
      "sin_phi);\n"
      "  gl_Position = u_LocalToProjectionMatrix * vec4(u_Exents * "
      "a_Position, 1.0);\n"
      "}\n";
  sp_desc.fragment_shader_source =
      "precision mediump float;\n"
      "varying vec3 vs_Normal;\n"
      "void main() {\n"
      "  float intensity = dot(vs_Normal, vec3(0.0, 1.0, 0.0));\n"
      "  gl_FragColor = vec4(intensity, intensity, intensity, 1.0);\n"
      "}\n";
  sp_ = std::make_unique<ovis::ShaderProgram>(context(), sp_desc);
  draw_item_.shader_program = sp_.get();
}

void WorldRenderer::Render() {
  sp_->SetUniform("LocalToProjectionMatrix",
                  scene()->camera().CalculateViewProjectionMatrix());
  context()->Clear();
  context()->Draw(draw_item_);
}

void WorldRenderer::UpdateVertexBuffer() {
  (void)world_;
  // struct Vertex {
  //   std::int16_t x;
  //   std::int16_t y;
  //   std::int16_t z;
  //   std::uint8_t theta;
  //   std::uint8_t phi;
  // };
  // static_assert(sizeof(Vertex) == 8, "");

  // const auto t2 = std::chrono::high_resolution_clock::now();

  // const auto& triangles   = world_->triangles();
  // const float half_width  = 0.5f * world_->width();
  // const float half_height = 0.5f * world_->height();
  // const float half_depth  = 0.5f * world_->depth();

  // std::vector<Vertex> vertices;
  // vertices.reserve(triangles.size() * 3);
  // for (const auto triangle : triangles) {
  //   const glm::vec3 normal = ovis::CalculateNormalizedNormal(triangle);

  //   const float theta = std::atan2(normal.z, normal.x) + glm::pi<float>();
  //   const float phi   = std::acos(normal.y);
  //   assert(theta >= 0 && theta <= 2 * glm::pi<float>());
  //   assert(phi >= 0 && phi <= glm::pi<float>());

  //   const std::uint8_t normal_theta =
  //       static_cast<std::uint8_t>(255 * 0.5 * theta / glm::pi<float>());

  //   const std::uint8_t normal_phi =
  //       static_cast<std::uint8_t>(255 * phi / glm::pi<float>());

  //   for (auto triangle_vertex : triangle.vertices) {
  //     vertices.push_back({
  //         static_cast<std::int16_t>(32768 * triangle_vertex.x / half_width),
  //         static_cast<std::int16_t>(32768 * triangle_vertex.y / half_height),
  //         static_cast<std::int16_t>(32768 * triangle_vertex.z / half_depth),
  //         normal_theta, normal_phi});
  //   }
  // }

  // const auto t3 = std::chrono::high_resolution_clock::now();
  // ovis::LogI(
  //     "Vertex Processing took ",
  //     std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count(),
  //     "ms");

  // ovis::VertexBufferDescription vb_desc;
  // vb_desc.size_in_bytes        = vertices.size() * sizeof(Vertex);
  // vb_desc.vertex_size_in_bytes = sizeof(Vertex);
  // vb_ =
  //     std::make_unique<ovis::VertexBuffer>(context(), vb_desc,
  //     vertices.data());

  // ovis::VertexInputDescription vi_desc;
  // vi_desc.vertex_attributes = {
  //     {"Position", ovis::VertexAttributeType::INT16_NORM_VECTOR3, 0, 0},
  //     {"Normal", ovis::VertexAttributeType::UINT8_NORM_VECTOR2, 6, 0}};
  // vi_desc.shader_program = sp_.get();
  // vi_desc.vertex_buffers = {vb_.get()};

  // vi_ = std::make_unique<ovis::VertexInput>(context(), vi_desc);

  // draw_item_.vertex_input = vi_.get();
  // draw_item_.count        = vertices.size();

  // sp_->SetUniform("Extents", glm::vec3{half_width, half_height, half_depth});
}

}  // namespace rr3d