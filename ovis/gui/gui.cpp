#if 0

#include <SDL.h>
#include <tb_font_renderer.h>
#include <tb_system.h>
#include <tb_skin.h>
#include <renderers/tb_renderer_batcher.h>

#include "gui.hpp"
#include "graphics_device.hpp"
#include "log.hpp"
#include "texture2d.hpp"
#include "vertex_buffer.hpp"
#include "vertex_source.hpp"
#include "shader_program.hpp"

void TBDebugOut(const char *str)
{
    LogD(str);
}

namespace tb
{
    double tb::TBSystem::GetTimeMS()
    {
        return SDL_GetTicks();
    }

    void tb::TBSystem::RescheduleTimer(double fire_time)
    {
        // What should i do here?
    }

    int tb::TBSystem::GetLongClickDelayMS()
    {
        return 200; // Standard for iOS (probably, read it somewhere)
    }

    int tb::TBSystem::GetPanThreshold()
    {
        return 5 * GetDPI() / 120; // Stolen from tb_system_android.cpp
    }

    int tb::TBSystem::GetPixelsPerLine()
    {
        return 40 * GetDPI() / 120; // Stolen from tb_system_android.cpp
    }

    int tb::TBSystem::GetDPI()
    {
    //    SDL_assert(SDL_GetNumVideoDisplays() == 1);
    //    SDL_assert(hdpi == vdpi);
    //    SDL_assert(static_cast<int>(hdpi) == vdpi);
        // SDL_GetDisplayDPI seems broken
#ifdef __IPHONEOS__
        return static_cast<int>(326);
#else
        float hdpi;
        float vdpi;
        SDL_GetDisplayDPI(0, nullptr, &hdpi, &vdpi);
        return static_cast<int>(hdpi);
#endif
    }

    /** Empty the contents of the clipboard. */
    void tb::TBClipboard::Empty()
    {
        SetText("");
    }

    /** Return true if the clipboard currently contains text. */
    bool tb::TBClipboard::HasText()
    {
        return SDL_HasClipboardText() == SDL_TRUE;
    }

    bool tb::TBClipboard::SetText(const char* text)
    {
        return SDL_SetClipboardText(text) == 0;
    }

    bool tb::TBClipboard::GetText(TBStr& text)
    {
        const char* clipboard_text = SDL_GetClipboardText();
        if (clipboard_text == nullptr)
        {
            return false;
        } else
        {
            return text.Set(clipboard_text);
        }
    }

    namespace
    {
        class SDLFile :
            public tb::TBFile
        {
            friend tb::TBFile* tb::TBFile::Open(const char* filename, tb::TBFile::TBFileMode mode);
            
        public:
            SDLFile(SDL_RWops* file) :
                m_file(file)
            {
                SDL_assert(m_file != nullptr);
            }
            
            virtual ~SDLFile() override
            {
                SDL_RWclose(m_file);
            }
            
            virtual long Size() override
            {
                return SDL_RWsize(m_file);
            }
            
            virtual size_t Read(void* buf, size_t elemSize, size_t count) override
            {
                return SDL_RWread(m_file, buf, elemSize, count);
            }
            
        private:
            SDL_RWops* m_file;
            
            static const char* GetOpenModeString(TBFileMode mode)
            {
                SDL_assert(mode == TBFileMode::MODE_READ);
                return "r";
            }
        };
    }

    tb::TBFile* tb::TBFile::Open(const char* filename, tb::TBFile::TBFileMode mode)
    {
        static const std::string RESOURCES_STRING = "$resources$";
        std::string processed_filename = filename;
        auto r_str = processed_filename.find(RESOURCES_STRING);
        if (r_str != processed_filename.npos)
        {
            processed_filename.replace(r_str, RESOURCES_STRING.length(), app()->resource_path());
        }

        SDL_RWops* file = SDL_RWFromFile(processed_filename.c_str(), SDLFile::GetOpenModeString(mode));
        if (file == nullptr)
        {
            return nullptr;
        } else
        {
            return new SDLFile(file);
        }
    }
}

namespace
{
    class InternalBitmap :
        public tb::TBBitmap
    {
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
        
        inline Texture2D* texture() const
        {
            return m_texture.get();
        }
        
    private:
        std::unique_ptr<Texture2D> m_texture;
    };
}

class Gui::InternalRenderer :
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
            texture = static_cast<InternalBitmap*>(batch->bitmap)->texture();
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

Gui::Gui() :
    m_tb_renderer(std::make_unique<InternalRenderer>()),
    m_event_type(SDL_RegisterEvents(1))
{
    SDL_assert_release(tb::tb_core_init(m_tb_renderer.get()));
    
    void register_freetype_font_renderer();
    register_freetype_font_renderer();
}

Gui::~Gui()
{
    tb::tb_core_shutdown();
}

void Gui::LoadSkin(const std::string& skin_filename)
{
    SDL_assert_release(tb::g_tb_skin->Load(skin_filename.c_str()));
}

void Gui::LoadFont(const std::string& font_filename, const std::string& font_name)
{
    tb::g_font_manager->AddFontInfo(font_filename.c_str(), font_name.c_str());
}

void Gui::SetDefaultFont(const std::string& font_name, float font_size_in_dp)
{
    const float font_size_in_px =
        tb::g_tb_skin->GetDimensionConverter()->DpToPxF(font_size_in_dp);

    tb::TBFontDescription fd;
    fd.SetID(font_name.c_str());
    fd.SetSize(static_cast<Uint32>(font_size_in_px + 0.5f)); // Round to nearest integer
    tb::g_font_manager->SetDefaultFontDescription(fd);
}
#endif
