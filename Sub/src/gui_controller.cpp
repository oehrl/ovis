#include <iostream>
#include <unordered_set>

#include <tb_core.h>
#include <tb_font_renderer.h>
#include <tb_msg.h>
#include <tb_widgets.h>
#include <tb_widgets_reader.h>
#include <animation/tb_animation.h>

#include <SDL.h>

#include "application.hpp"

#include "gui.hpp"
#include "gui_controller.hpp"

namespace
{
    const std::unordered_set<std::string> IGNORED_GUI_CLASSES =
    {
        "TBLayout"
    };
}

class GuiController::RootWidget :
    public tb::TBWidget
{
public:
    RootWidget(GuiController* /*gui_controller*/)/* :
        m_gui_controller(gui_controller)*/
    {
    }
    
    virtual bool OnEvent(const tb::TBWidgetEvent& event) override
    {
        SDL_Event sdl_event;
        SDL_zero(sdl_event);
        
        sdl_event.type = gui()->event_type();
        sdl_event.user.code = event.type;
        sdl_event.user.data1 = const_cast<tb::TBWidgetEvent*>(&event);
        
        SDL_PushEvent(&sdl_event);
        
        return false;
    }
    
private:
    //GuiController* m_gui_controller;
};

GuiController::GuiController(Scene* scene, const std::string& layout) :
    SceneController(scene, "GuiController")
{
    SDL_assert(gui() != nullptr);
    
    int window_width;
    int window_height;
    SDL_GetWindowSize(app()->window(), &window_width, &window_height);
    
    m_root_widget = std::make_unique<RootWidget>(this);
    m_root_widget->SetRect(tb::TBRect(
        0,
        0,
        app()->window_width(),
        app()->window_height()
    ));
    
    m_root_widget->SetFontDescription(tb::g_font_manager->GetDefaultFontDescription());
    SDL_assert_release(tb::g_widgets_reader->LoadFile(m_root_widget.get(), layout.c_str()));
}

GuiController::~GuiController()
{
}

void GuiController::Update(Uint32 /*delta_time*/)
{
    SDL_assert(gui() != nullptr);
    
    std::string text = std::to_string((int)app()->fps());
    //GetWidgetById("fps")->SetText(text.c_str());

    tb::TBMessageHandler::ProcessMessages();
    tb::TBAnimationManager::Update();
    m_root_widget->InvokeProcessStates();
    m_root_widget->InvokeProcess();
}

bool GuiController::ProcessEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION)
    {
        m_root_widget->InvokePointerMove(
            event.motion.x,
            event.motion.y,
            tb::TB_MODIFIER_NONE,
            event.motion.which == SDL_TOUCH_MOUSEID
        );
        
        return false;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int x = event.button.x;
        int y = event.button.y;
        m_root_widget->InvokePointerDown(
            x,
            y,
            event.button.clicks,
            tb::TB_MODIFIER_NONE,
            event.button.which == SDL_TOUCH_MOUSEID
        );
        
        tb::TBWidget* hit_widget = m_root_widget->GetWidgetAt(x, y, true);
        return
            hit_widget != nullptr &&
            IGNORED_GUI_CLASSES.count(hit_widget->GetClassName()) == 0;
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        int x = event.button.x;
        int y = event.button.y;
        
        m_root_widget->InvokePointerUp(
            x,
            y,
            tb::TB_MODIFIER_NONE,
            event.button.which == SDL_TOUCH_MOUSEID
        );
        
        tb::TBWidget* hit_widget = m_root_widget->GetWidgetAt(x, y, true);
        return
            hit_widget != nullptr &&
            IGNORED_GUI_CLASSES.count(hit_widget->GetClassName()) == 0;
    }
    else
    {
        return false;
    }
}

tb::TBWindow* GuiController::CreateWindow(
    const std::string& window_name,
    const std::string& content_layout_filename
)
{
    SDL_assert(m_windows.count(window_name) == 0);
    auto& window = m_windows[window_name] = new tb::TBWindow();
    SDL_assert_release(tb::g_widgets_reader->LoadFile(
        window,
        content_layout_filename.c_str()
    ));
    m_root_widget->AddChild(window);
    return window;
}

tb::TBWidget* GuiController::GetWidgetById(tb::TBID id)
{
    return m_root_widget->GetWidgetByID(id);
}
