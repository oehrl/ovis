#if 0

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
#include "log.hpp"
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
        sdl_event.user.data1 = new tb::TBWidgetEvent(event); // TODO: super ugly, cleanup in Application::ProcessEvents
        // TODO: implement ring buffer
        
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
    SDL_assert_release(tb::g_widgets_reader->LoadFile(m_root_widget.get(), app()->ParseString(layout).c_str()));
}

GuiController::~GuiController()
{
}

void GuiController::Update(Uint32 /*delta_time*/)
{
    tb::TBMessageHandler::ProcessMessages();
    tb::TBAnimationManager::Update();
    m_root_widget->InvokeProcessStates();
    m_root_widget->InvokeProcess();
}

bool GuiController::ProcessEvent(const SDL_Event& event)
{
    static const auto get_special_key = [](SDL_Keycode keycode)
    {
        switch (keycode)
        {
            case SDLK_UP:        return tb::SPECIAL_KEY::TB_KEY_UP;
            case SDLK_DOWN:      return tb::SPECIAL_KEY::TB_KEY_DOWN;
            case SDLK_LEFT:      return tb::SPECIAL_KEY::TB_KEY_LEFT;
            case SDLK_RIGHT:     return tb::SPECIAL_KEY::TB_KEY_RIGHT;
            
            case SDLK_PAGEUP:    return tb::SPECIAL_KEY::TB_KEY_PAGE_UP;
            case SDLK_PAGEDOWN:  return tb::SPECIAL_KEY::TB_KEY_DOWN;
            case SDLK_HOME:      return tb::SPECIAL_KEY::TB_KEY_HOME;
            case SDLK_END:       return tb::SPECIAL_KEY::TB_KEY_END;
            
            case SDLK_TAB:       return tb::SPECIAL_KEY::TB_KEY_TAB;
            case SDLK_INSERT:    return tb::SPECIAL_KEY::TB_KEY_INSERT;
            case SDLK_BACKSPACE: return tb::SPECIAL_KEY::TB_KEY_BACKSPACE;
            case SDLK_DELETE:    return tb::SPECIAL_KEY::TB_KEY_DELETE;
            
            case SDLK_RETURN:    return tb::SPECIAL_KEY::TB_KEY_ENTER;
            case SDLK_ESCAPE:    return tb::SPECIAL_KEY::TB_KEY_ESC;
            
            case SDLK_F1:        return tb::SPECIAL_KEY::TB_KEY_F1;
            case SDLK_F2:        return tb::SPECIAL_KEY::TB_KEY_F2;
            case SDLK_F3:        return tb::SPECIAL_KEY::TB_KEY_F3;
            case SDLK_F4:        return tb::SPECIAL_KEY::TB_KEY_F4;
            case SDLK_F5:        return tb::SPECIAL_KEY::TB_KEY_F5;
            case SDLK_F6:        return tb::SPECIAL_KEY::TB_KEY_F6;
            case SDLK_F7:        return tb::SPECIAL_KEY::TB_KEY_F7;
            case SDLK_F8:        return tb::SPECIAL_KEY::TB_KEY_F8;
            case SDLK_F9:        return tb::SPECIAL_KEY::TB_KEY_F9;
            case SDLK_F10:       return tb::SPECIAL_KEY::TB_KEY_F10;
            case SDLK_F11:       return tb::SPECIAL_KEY::TB_KEY_F11;
            case SDLK_F12:       return tb::SPECIAL_KEY::TB_KEY_F12;

            default: return tb::SPECIAL_KEY::TB_KEY_UNDEFINED;
        }
    };
    static const auto get_modifiers = [](Uint16 modifiers)
    {
        tb::MODIFIER_KEYS modifier_keys = tb::TB_MODIFIER_NONE;
        if (modifiers & KMOD_SHIFT)
        {
            modifier_keys |= tb::MODIFIER_KEYS::TB_SHIFT;
        }
        if (modifiers & KMOD_ALT)
        {
            modifier_keys |= tb::MODIFIER_KEYS::TB_ALT;
        }
        if (modifiers & KMOD_CTRL)
        {
            modifier_keys |= tb::MODIFIER_KEYS::TB_CTRL;
        }
        if (modifiers & KMOD_GUI)
        {
            modifier_keys |= tb::MODIFIER_KEYS::TB_SUPER;
        }
        return modifier_keys;
    };

    switch (event.type)
    {
        case SDL_MOUSEMOTION:
        {
            m_root_widget->InvokePointerMove(
                event.motion.x,
                event.motion.y,
                tb::TB_MODIFIER_NONE,
                event.motion.which == SDL_TOUCH_MOUSEID
            );
            
            return false;
        }
            
        case SDL_MOUSEBUTTONDOWN:
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
        
        case SDL_MOUSEBUTTONUP:
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
        
        case SDL_TEXTINPUT:
        {
            bool result = false;
            for (const char* current = event.text.text; *current != '\0'; ++current)
            {
                if (m_root_widget->InvokeKey(
                        *current,
                        tb::SPECIAL_KEY::TB_KEY_UNDEFINED,
                        tb::TB_MODIFIER_NONE,
                        true
                    ))
                {
                    result = true;
                }
                if (m_root_widget->InvokeKey(
                        *current,
                        tb::SPECIAL_KEY::TB_KEY_UNDEFINED,
                        tb::TB_MODIFIER_NONE,
                        false
                    ))
                {
                    result = true;
                }
            }
            return result;
        }
        
        case SDL_KEYDOWN:
        {
            auto special_key = get_special_key(event.key.keysym.sym);
            if (special_key != tb::SPECIAL_KEY::TB_KEY_UNDEFINED)
            {
                return m_root_widget->InvokeKey(
                    event.key.keysym.sym,
                    get_special_key(event.key.keysym.sym),
                    get_modifiers(event.key.keysym.mod),
                    true
                );
            }
            else
            {
                return false;
            }
        }
        
        case SDL_KEYUP:
        {
            auto special_key = get_special_key(event.key.keysym.sym);
            if (special_key != tb::SPECIAL_KEY::TB_KEY_UNDEFINED)
            {
                return m_root_widget->InvokeKey(
                    event.key.keysym.sym,
                    get_special_key(event.key.keysym.sym),
                    get_modifiers(event.key.keysym.mod),
                    false
                );
            }
            else
            {
                return false;
            }
        }
        
        default:
        {
            return false;
        }
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
#endif
