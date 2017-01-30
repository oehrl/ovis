#pragma once

#include "scene_renderer.hpp"

class GuiRenderer :
    public SceneRenderer
{
    friend class GuiController;

public:
    GuiRenderer(Scene* scene);
    
    virtual void Render() override;
    
private:
    static bool tb_initialized();
    static void InitTB();
};
