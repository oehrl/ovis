#pragma once

#include <memory>
#include <map>

#include <tb_widgets.h>
#include <tb_window.h>

#include "scene_controller.hpp"

class GuiController :
    public SceneController
{
    friend class GuiRenderer;
    
public:
    GuiController(Scene* scene, const std::string& layout);
    virtual ~GuiController() override;
    
    virtual void Update(Uint32 delta_time) override;
    virtual bool ProcessEvent(const SDL_Event& event) override;
    
    tb::TBWindow* CreateWindow(
        const std::string& window_name,
        const std::string& content_layout_filename
    );
    tb::TBWidget* GetWidgetById(tb::TBID id);
    
    inline tb::TBWidget* root_widget() const
    {
        return m_root_widget.get();
    }
    
private:
    class RootWidget;
    
    std::unique_ptr<tb::TBWidget> m_root_widget;
    std::map<std::string, tb::TBWindow*> m_windows;
};
