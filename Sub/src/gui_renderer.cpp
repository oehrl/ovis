#include <tb_widgets.h>
#include <renderers/tb_renderer_batcher.h>

#include "application.hpp"
#include "graphics_device.hpp"
#include "gui_controller.hpp"
#include "gui_renderer.hpp"
#include "texture2d.hpp"
#include "vertex_buffer.hpp"
#include "vertex_source.hpp"
#include "scene.hpp"
#include "shader_program.hpp"

namespace
{
    class InternalBitmap :
        public tb::TBBitmap
    {
        friend class InternalRenderer;
    
    public:
        InternalBitmap(std::unique_ptr<Texture2D>&& texture) :
            m_texture(std::move(texture))
        {
        }
    
        virtual int Width() override
        {
            return static_cast<int>(m_texture->description().width);
        }
        
        virtual int Height() override
        {
            return static_cast<int>(m_texture->description().height);
        }
        
        virtual void SetData(tb::uint32* data) override
        {
            m_texture->Write(
                0,
                0, 0,
                Width(), Height(),
                data
            );
        }
        
    private:
        std::unique_ptr<Texture2D> m_texture;
    };

    class InternalRenderer :
        public tb::TBRendererBatcher
    {
    public:
        InternalRenderer()
        {
            m_shader_program = LoadShaderProgram(GetFullResourcePath("gui_shader"));
            
            VertexBufferDescription vb_desc;
            vb_desc.vertex_size_in_bytes = sizeof(Vertex);
            vb_desc.size_in_bytes = sizeof(Vertex) * VERTEX_BATCH_SIZE;
            m_vertex_buffer = std::make_unique<VertexBuffer>(graphics_device(), vb_desc);
            
            VertexSourceDescription vs_desc;
            vs_desc.shader_program = m_shader_program.get();
            vs_desc.vertex_buffers = { m_vertex_buffer.get() };
            vs_desc.vertex_attributes =
            {
                { "Position",  VertexAttributeType::FLOAT32_VECTOR2,    0,  0 },
                { "TexCoords", VertexAttributeType::FLOAT32_VECTOR2,    0,  8 },
                { "Color",     VertexAttributeType::UINT8_NORM_VECTOR4, 0, 16 }
            };
            m_vertex_source = std::make_unique<VertexSource>(graphics_device(), vs_desc);
            
            m_draw_item.shader_program = m_shader_program.get();
            m_draw_item.vertex_source = m_vertex_source.get();
            m_draw_item.index_buffer = nullptr;
            m_draw_item.primitive_topology = PrimitiveTopology::TRIANGLE_LIST;
            m_draw_item.start = 0;
            m_draw_item.depth_buffer_state = DepthBufferState::DISABLED;
            m_draw_item.alpha_blending_enabled = true;
        }
    
        virtual tb::TBBitmap* CreateBitmap(int width, int height, tb::uint32* data) override
        {
            Texture2DDescription texture_description;
            texture_description.width = width;
            texture_description.height = height;
            texture_description.format = TextureFormat::RGBA_UINT8;
            texture_description.filter = TextureFilter::POINT;
            texture_description.mip_map_count = 1;
            return new InternalBitmap(std::make_unique<Texture2D>(graphics_device(), texture_description, data));
        }
        
        virtual void RenderBatch(Batch* batch) override
        {
            m_vertex_buffer->Write(
                0,
                batch->vertex_count * sizeof(Vertex),
                batch->vertex
            );
            
            Texture2D* texture = nullptr;
            if (batch->bitmap != nullptr)
            {
                texture = static_cast<InternalBitmap*>(batch->bitmap)->m_texture.get();
            }
            m_shader_program->SetTexture("TextureSampler", texture);
            m_shader_program->SetUniform("ClipRect", m_clip_rect); // TODO: implement actual clipping
            m_shader_program->SetUniform(
                "ViewportSize",
                glm::vec2(
                    app()->window_width(),
                    app()->window_height()
                )
            );
            
            m_draw_item.count = batch->vertex_count;
            graphics_device()->Draw(m_draw_item);
        }
        
        virtual void SetClipRect(const tb::TBRect& rect) override
        {
            m_clip_rect.x = rect.x;
            m_clip_rect.y = rect.y;
            m_clip_rect.z = rect.w;
            m_clip_rect.w = rect.h;
        }
        
    private:
        std::unique_ptr<VertexBuffer> m_vertex_buffer;
        std::unique_ptr<VertexSource> m_vertex_source;
        std::unique_ptr<ShaderProgram> m_shader_program;
        DrawItem m_draw_item;
        glm::vec4 m_clip_rect;
    };
    
    std::unique_ptr<InternalRenderer> g_internal_renderer;
}

GuiRenderer::GuiRenderer(Scene* scene) :
    SceneRenderer(scene, "GuiRenderer")
{
    if (!tb_initialized())
    {
        InitTB();
    }
}

void GuiRenderer::Render()
{
    GuiController* controller = static_cast<GuiController*>(
        scene()->GetController("GuiController")
    );
    
    if (controller != nullptr)
    {
        tb::g_renderer->BeginPaint(
            app()->window_width(),
            app()->window_height()
        );
        controller->root_widget()->InvokePaint(tb::TBWidget::PaintProps());
        tb::g_renderer->EndPaint();
        controller->root_widget()->Invalidate();
    }
}

bool GuiRenderer::tb_initialized()
{
    return g_internal_renderer != nullptr;
}

void register_freetype_font_renderer();

void GuiRenderer::InitTB()
{
    if (!tb_initialized())
    {
        g_internal_renderer = std::make_unique<InternalRenderer>();
        SDL_assert_release(tb::tb_core_init(g_internal_renderer.get()));
        SDL_assert_release(tb::g_tb_skin->Load(GetFullResourcePath("default_skin/skin.tb.txt").c_str()));
        register_freetype_font_renderer();
    }
}
