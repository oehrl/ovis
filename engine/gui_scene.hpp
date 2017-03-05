#pragma once

#include "scene.hpp"
#include "gui_controller.hpp"
#include "gui_renderer.hpp"

class GuiScene :
    public Scene
{
public:
    GuiScene(
        const std::string& scene_name,
        const std::string& gui_layout_filename,
        bool hide_previous = true
    );
    
    inline GuiController* gui_controller()
    {
        return &m_gui_controller;
    }
    
    inline GuiRenderer* gui_renderer()
    {
        return &m_gui_renderer;
    }

private:
    GuiController m_gui_controller;
    GuiRenderer m_gui_renderer;
};
