#include <iostream>

#include <tb_core.h>
#include <tb_font_renderer.h>
#include <tb_system.h>
#include <tb_widgets.h>
#include <tb_widgets_reader.h>
#include <animation/tb_animation.h>

#include <SDL.h>

#include "application.hpp"

#include "gui_controller.hpp"
#include "gui_renderer.hpp"

class GuiController::RootWidget :
    public tb::TBWidget
{
public:
    RootWidget(GuiController* gui_controller)
    {
    }
};

GuiController::GuiController(Scene* scene, const std::string& layout) :
    SceneController(scene, "GuiController")
{
    int window_width;
    int window_height;
    SDL_GetWindowSize(app()->window(), &window_width, &window_height);
    
    if (!GuiRenderer::tb_initialized())
    {
        GuiRenderer::InitTB();
    }
    
    m_root_widget = std::make_unique<RootWidget>(this);
    m_root_widget->SetRect(tb::TBRect(
        0,
        0,
        app()->window_width(),
        app()->window_height()
    ));
    
    
    
    tb::g_font_manager->AddFontInfo(GetFullResourcePath("vera.ttf").c_str(), "Vera");
    
    tb::TBFontDescription fd;
    fd.SetID("Vera");
    fd.SetSize(tb::g_tb_skin->GetDimensionConverter()->DpToPx(12));
    tb::g_font_manager->SetDefaultFontDescription(fd);

//    tb::TBFontFace *font = tb::g_font_manager->CreateFontFace(tb::g_font_manager->GetDefaultFontDescription());
//    if (font)
//    {
//        font->RenderGlyphs(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~•·åäöÅÄÖ");
//    }
    
    m_root_widget->SetFontDescription(tb::g_font_manager->GetDefaultFontDescription());
    SDL_assert_release(tb::g_widgets_reader->LoadFile(m_root_widget.get(), layout.c_str()));
}

GuiController::~GuiController()
{
}

void GuiController::Update(Uint32 delta_time)
{
    tb::TBAnimationManager::Update();
    m_root_widget->InvokeProcessStates();
    m_root_widget->InvokeProcess();
}

tb::TBWidget* GuiController::root_widget() const
{
    return m_root_widget.get();
}


void TBDebugOut(const char *str)
{
    std::cout << str << std::endl;
}

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
//    float hdpi;
//    float vdpi;
//    SDL_GetDisplayDPI(0, nullptr, &hdpi, &vdpi);
//    SDL_assert(hdpi == vdpi);
//    SDL_assert(static_cast<int>(hdpi) == vdpi);
    // SDL_GetDisplayDPI seems broken
    return static_cast<int>(326);
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

tb::TBFile* tb::TBFile::Open(const char* filename, tb::TBFile::TBFileMode mode)
{
    SDL_RWops* file = SDL_RWFromFile(filename, SDLFile::GetOpenModeString(mode));
    if (file == nullptr)
    {
        return nullptr;
    } else
    {
        return new SDLFile(file);
    }
}
