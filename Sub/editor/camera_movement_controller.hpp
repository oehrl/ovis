#pragma once

#include <deque>
#include <unordered_set>
#include <glm/vec2.hpp>
#include <scene_controller.hpp>

class EditorScene;

class CameraMovementController :
    public SceneController
{
public:
    CameraMovementController(EditorScene* editor_scene);
    
    virtual void Update(Uint32 delta_time_in_milliseconds) override;
    virtual bool ProcessEvent(const SDL_Event& event) override;
    
    EditorScene* editor_scene() const;
    
private:
    glm::vec2 m_velocity;
    std::deque<SDL_TouchFingerEvent> m_touch_events;
    std::unordered_set<Sint64> m_fingers;
    float m_zoom_factor;
};
