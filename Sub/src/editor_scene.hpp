#pragma once

#include "gui_scene.hpp"

class EditorScene :
    public GuiScene
{
public:
    EditorScene();
    
private:
    virtual bool BeforeEventProcessing(const SDL_Event& event) override;
};
