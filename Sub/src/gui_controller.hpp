#pragma once

#include <memory>

#include "scene_controller.hpp"

namespace tb
{
    class TBWidget;
}

class GuiController :
    public SceneController
{
    friend class GuiRenderer;
    
public:
    GuiController(Scene* scene, const std::string& layout);
    virtual ~GuiController() override;
    
    virtual void Update(Uint32 delta_time) override;
    
private:
    class RootWidget;
    
    std::unique_ptr<RootWidget> m_root_widget;
    
    tb::TBWidget* root_widget() const;
};
