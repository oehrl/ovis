#pragma once

#include "scene_renderer.hpp"

class GuiRenderer :
    public SceneRenderer
{
public:
    GuiRenderer(Scene* scene);
    
    virtual void Render() override;
};
