#define NK_IMPLEMENTATION
#include <SDL2/SDL_assert.h>

#include <ovis/core/resource_manager.hpp>

#include <ovis/graphics/graphics_context.hpp>

#include <ovis/engine/base/imgui_render_pass.hpp>
#include <ovis/engine/scene.hpp>

namespace ovis {

ImGuiRenderPass::ImGuiRenderPass(ImGuiContext* context)
    : ovis::RenderPass("ImGui"), context_(context) {}

ImGuiRenderPass::~ImGuiRenderPass() {}

void ImGuiRenderPass::CreateResources() {
  shader_program_ = resource_manager()->Load<ovis::ShaderProgram>("ui.shader");

  unsigned char* pixels;
  int width;
  int height;
  int bytes_per_pixel;
  ImGui::GetIO().Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

  ovis::Texture2DDescription font_texture_desc;
  font_texture_desc.width = width;
  font_texture_desc.height = height;
  font_texture_desc.format = ovis::TextureFormat::RGBA_UINT8;
  font_texture_desc.filter = ovis::TextureFilter::BILINEAR;
  font_texture_desc.mip_map_count = 1;
  font_texture_ =
      std::make_unique<ovis::Texture2D>(context(), font_texture_desc, pixels, width * height * bytes_per_pixel);

  ImGui::GetIO().Fonts->TexID = font_texture_.get();
}

void ImGuiRenderPass::Render(ovis::Scene* scene) {
  ImGui::SetCurrentContext(context_);
  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();

  shader_program_->SetUniform("HalfScreenSize",
                              0.5f * glm::vec2(ImGui::GetIO().DisplaySize.x,
                                               ImGui::GetIO().DisplaySize.y));
  for (int i = 0; i < draw_data->CmdListsCount; ++i) {
    auto command_list = draw_data->CmdLists[i];

    if (!vertex_buffer_ || !vertex_input_ ||
        vertex_buffer_->description().size_in_bytes <
            command_list->VtxBuffer.size_in_bytes()) {
      ovis::VertexBufferDescription vb_desc;
      vb_desc.size_in_bytes = command_list->VtxBuffer.size_in_bytes();
      vb_desc.vertex_size_in_bytes = sizeof(ImDrawVert);
      vertex_buffer_ = std::make_unique<ovis::VertexBuffer>(context(), vb_desc);

      ovis::VertexInputDescription vi_desc;
      auto position_location =
          shader_program_->GetAttributeLocation("Position");
      if (position_location) {
        vi_desc.vertex_attributes.push_back(
            {*position_location, 0, 0,
             ovis::VertexAttributeType::FLOAT32_VECTOR2});
      }
      auto texture_coordinates_location =
          shader_program_->GetAttributeLocation("TextureCoordinates");
      if (texture_coordinates_location) {
        vi_desc.vertex_attributes.push_back(
            {*texture_coordinates_location, 8, 0,
             ovis::VertexAttributeType::FLOAT32_VECTOR2});
      }
      auto color_location = shader_program_->GetAttributeLocation("Color");
      if (color_location) {
        vi_desc.vertex_attributes.push_back(
            {*color_location, 16, 0,
             ovis::VertexAttributeType::UINT8_NORM_VECTOR4});
      }
      vi_desc.vertex_buffers = {vertex_buffer_.get()};
      vertex_input_ = std::make_unique<ovis::VertexInput>(context(), vi_desc);
    }

    if (!index_buffer_ || index_buffer_->description().size_in_bytes <
                              command_list->IdxBuffer.size_in_bytes()) {
      ovis::IndexBufferDescription vi_desc;
      vi_desc.size_in_bytes = command_list->IdxBuffer.size_in_bytes();
      vi_desc.index_format = ovis::IndexFormat::UINT16;
      index_buffer_ = std::make_unique<ovis::IndexBuffer>(context(), vi_desc);
    }

    vertex_buffer_->Write(0, command_list->VtxBuffer.size_in_bytes(),
                          command_list->VtxBuffer.Data);
    index_buffer_->Write(0, command_list->IdxBuffer.size_in_bytes(),
                         command_list->IdxBuffer.Data);

    for (int command_index = 0; command_index < command_list->CmdBuffer.Size;
         ++command_index) {
      const auto& command = command_list->CmdBuffer[command_index];

      shader_program_->SetTexture(
          "Texture", reinterpret_cast<ovis::Texture2D*>(command.TextureId));

      ovis::DrawItem draw_item;
      draw_item.vertex_input = vertex_input_.get();
      draw_item.index_buffer = index_buffer_.get();
      draw_item.shader_program = shader_program_.get();
      draw_item.start = command.IdxOffset;
      draw_item.count = command.ElemCount;
      draw_item.base_vertex = command.VtxOffset;
      draw_item.primitive_topology = ovis::PrimitiveTopology::TRIANGLE_LIST;
      draw_item.blend_state.enabled = true;
      draw_item.blend_state.source_color_factor =
          ovis::SourceBlendFactor::SOURCE_ALPHA;
      draw_item.blend_state.destination_color_factor =
          ovis::DestinationBlendFactor::ONE_MINUS_SOURCE_ALPHA;
      // draw_item.scissor_rect.emplace();
      // draw_item.scissor_rect->left = command.ClipRect.x;
      // draw_item.scissor_rect->top = command.ClipRect.y;
      // draw_item.scissor_rect->width = command.ClipRect.z -
      // command.ClipRect.x; draw_item.scissor_rect->height = command.ClipRect.w
      // - command.ClipRect.y;
      context()->Draw(draw_item);
    }
  }
}

}  // namespace ovis
