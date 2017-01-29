#pragma once

#include <memory>

#include "scene_controller.hpp"

class GuiController :
    public SceneController
{
public:
    GuiController(Scene* scene);
    
    virtual void Update(Uint32 delta_time) override;
    
private:
    class RootWidget;
    
    std::unique_ptr<RootWidget> m_root_widget;
};
