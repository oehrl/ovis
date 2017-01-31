#pragma once

#include "gui_scene.hpp"
#include "level_renderer.hpp"
#include "camera_controller.hpp"

class GameScene :
    public GuiScene
{
public:
    GameScene();

private:
    LevelRenderer m_level_renderer;
    CameraController m_camera_conroller;
};
