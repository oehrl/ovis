#include "ovis/gui/gui_renderer.hpp"
#include "ovis/core/array_view.hpp"
#include "ovis/core/log.hpp"
#include "ovis/graphics/graphics_context.hpp"

namespace ovis {

GuiRenderer::GuiRenderer(Scene* scene)
    : SceneRenderer(scene, "GuiRenderer"), context_(ImGui::CreateContext()) {}

GuiRenderer::~GuiRenderer() {
  ImGui::DestroyContext(context_);
}

void GuiRenderer::CreateResources() {
  LogD("Context: ", context());

  VertexBufferDescription vb_desc;
  vb_desc.size_in_bytes        = 10000 * sizeof(ImDrawVert);
  vb_desc.vertex_size_in_bytes = sizeof(ImDrawVert);
  vertices_ = std::make_unique<VertexBuffer>(context(), vb_desc);

  ShaderProgramDescription shader_desc;
  shader_desc.vertex_shader_source =
      "in vec2 a_Position;\n"
      "in vec2 a_TexCoords;\n"
      "in vec4 a_Color;\n"
      "out vec2 vs_TexCoords;\n"
      "out vec4 vs_Color;\n"
      "uniform vec2 u_InverseViewportSize;\n"
      "void main() {\n"
      "  gl_Position = vec4(a_Position * u_InverseViewportSize * 2.0 - 1.0, "
      "0.0, 1.0);\n"
      "  gl_Position.y = -gl_Position.y;\n"
      "  vs_TexCoords = a_TexCoords;\n"
      "  vs_Color = a_Color;\n"
      "}\n";
  shader_desc.fragment_shader_source =
      "in vec2 vs_TexCoords;\n"
      "in vec4 vs_Color;\n"
      // "#ifndef OVIS_EMSCRIPTEN\n"
      // "#define texture2D texture\n"
      // "out vec4 fs_color;\n"
      // "#endif\n"
      "uniform sampler2D u_Texture;\n"
      "void main() {\n"
      "  gl_FragColor = vs_Color * texture2D(u_Texture, vs_TexCoords);\n"
      "}\n";
  shader_ = std::make_unique<ShaderProgram>(context(), shader_desc);

  VertexInputDescription vi_desc;
  vi_desc.shader_program    = shader_.get();
  vi_desc.vertex_buffers    = {vertices_.get()};
  vi_desc.vertex_attributes = {
      {"Position", VertexAttributeType::FLOAT32_VECTOR2, 0, 0},
      {"TexCoords", VertexAttributeType::FLOAT32_VECTOR2, 8, 0},
      {"Color", VertexAttributeType::UINT8_NORM_VECTOR4, 16, 0}};
  vertex_input_ = std::make_unique<VertexInput>(context(), vi_desc);

  IndexBufferDescription ib_desc;
  ib_desc.size_in_bytes = 10000;
  ib_desc.index_format  = IndexFormat::UINT16;
  indices_              = std::make_unique<IndexBuffer>(context(), ib_desc);

  ImGui::GetIO().DisplaySize.x = scene()->size().x;
  ImGui::GetIO().DisplaySize.y = scene()->size().y;
  unsigned char* pixels;
  int width;
  int height;
  ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
  Texture2DDescription tex_desc;
  tex_desc.width         = width;
  tex_desc.height        = height;
  tex_desc.format        = TextureFormat::RGBA_UINT8;
  tex_desc.filter        = TextureFilter::POINT;
  tex_desc.mip_map_count = 1;
  font_atlas_texture_ =
      std::make_unique<Texture2D>(context(), tex_desc, pixels);
  ImGui::GetIO().Fonts->SetTexID(font_atlas_texture_.get());
  ImGui::NewFrame();
  // LogD("ImGui::NewFrame()");
}

void GuiRenderer::Render() {
  ImGui::SetCurrentContext(context_);
  ImGui::Render();
  auto draw_data = ImGui::GetDrawData();

  DrawItem draw_item;
  draw_item.primitive_topology              = PrimitiveTopology::TRIANGLE_LIST;
  draw_item.shader_program                  = shader_.get();
  draw_item.blend_state.enabled             = true;
  draw_item.blend_state.source_color_factor = SourceBlendFactor::SOURCE_ALPHA;
  draw_item.blend_state.destination_color_factor =
      DestinationBlendFactor::ONE_MINUS_SOURCE_ALPHA;
  draw_item.enable_scissoring = true;
  shader_->SetUniform("InverseViewportSize", 1.0f / glm::vec2(scene()->size()));

  for (int i = 0; i < draw_data->CmdListsCount; ++i) {
    auto draw_list = draw_data->CmdLists[i];
    UpdateVertexBuffer(draw_list->VtxBuffer);
    UpdateIndexBuffer(draw_list->IdxBuffer);

    draw_item.index_buffer = indices_.get();
    draw_item.vertex_input = vertex_input_.get();

    std::size_t index_offset = 0;
    for (const auto& draw_command : draw_list->CmdBuffer) {
      if (draw_command.UserCallback) {
        LogD("User callback: ", draw_command.UserCallback);
        draw_command.UserCallback(draw_list, &draw_command);
      } else {
        // TODO: set texture
        auto clip_rect = draw_command.ClipRect;

        draw_item.scissor_rect = {
            static_cast<int>(clip_rect.x),
            static_cast<int>(scene()->size().y - clip_rect.w),
            static_cast<int>(clip_rect.z - clip_rect.x),
            static_cast<int>(clip_rect.w - clip_rect.y)};
        draw_item.count = draw_command.ElemCount;
        draw_item.start = index_offset;
        shader_->SetTexture("Texture",
                            static_cast<Texture2D*>(draw_command.TextureId));
      }
      index_offset += draw_command.ElemCount;
      context()->Draw(draw_item);
      // LogD("Drawing ", draw_command.ElemCount, " elements");
    }
  }

  ImGui::NewFrame();
}

void GuiRenderer::UpdateVertexBuffer(const ImVector<ImDrawVert>& vertices) {
  const std::size_t vertices_size_in_bytes =
      vertices.size() * sizeof(ImDrawVert);
  if (vertices_size_in_bytes > vertices_->description().size_in_bytes) {
    // TODO: create larger buffer
    LogD("Vertex buffer needs resize: actual(",
         vertices_->description().size_in_bytes, ") needed(",
         vertices_size_in_bytes, ")");
    // assert(false);
  } else {
    vertices_->Write(0, vertices_size_in_bytes, vertices.Data);
  }
}

void GuiRenderer::UpdateIndexBuffer(const ImVector<ImDrawIdx>& indices) {
  static_assert(sizeof(ImDrawIdx) == 2);
  const std::size_t indices_size_in_bytes = indices.size() * sizeof(ImDrawIdx);
  if (indices_size_in_bytes > indices_->description().size_in_bytes) {
    // TODO: create larger buffer
    LogD("Index buffer needs resize: actual(",
         indices_->description().size_in_bytes, ") needed(",
         indices_size_in_bytes, ")");
    // assert(false);
  } else {
    indices_->Write(0, indices_size_in_bytes, indices.Data);
  }
}

}  // namespace ovis