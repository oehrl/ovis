#pragma once

#include <random>
#include <vector>
#include <unordered_set>

#include <glm/vec3.hpp>

#include "editor_mode.hpp"
#include "gui_scene.hpp"
#include "game_scene.hpp"
#include "level_renderer.hpp"
#include "camera_controller.hpp"
#include "camera_movement_controller.hpp"

class EditorScene :
    public GuiScene
{
public:
    EditorScene();
    
    inline CameraController* camera_controller()
    {
        return &m_camera_controller;
    }
    
private:
    GameScene m_game_scene;
    CameraController m_camera_controller;
    LevelRenderer m_level_renderer;
    CameraMovementController m_camera_movement_controller;
    std::unique_ptr<EditorMode> m_mode;
    

    virtual bool BeforeEventProcessing(const SDL_Event& event) override;
    virtual bool AfterEventProcessing(const SDL_Event& event) override;
    virtual void OnUpdate(Uint32 delta_time) override;
    
    void StartGame();
    void UpdatePath();
};
