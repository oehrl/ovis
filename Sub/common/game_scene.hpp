#pragma once

#include "gui_scene.hpp"
#include "level_renderer.hpp"
#include "camera_controller.hpp"

class GameScene :
    public GuiScene
{
public:
    GameScene();
    
    virtual void OnUpdate(Uint32 delta_time) override;
    
    inline LevelRenderer* level_renderer()
    {
        return &m_level_renderer;
    }

private:
    LevelRenderer m_level_renderer;
    CameraController m_camera_conroller;
    
    virtual bool BeforeEventProcessing(const SDL_Event& event) override;
};
