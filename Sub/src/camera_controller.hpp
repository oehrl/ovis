#pragma once

#include <glm/vec3.hpp>

#include "scene_controller.hpp"

class CameraController :
    public SceneController
{
public:
    CameraController(Scene* scene);
    
    virtual void Update(Uint32 delta_time) override;
    virtual bool ProcessEvent(const SDL_Event& event) override;
    
    inline float yaw() const { return m_yaw; }
    inline float pitch() const { return m_pitch; }
    inline glm::vec3 forward() const { return m_forward; }
    inline glm::vec3 position() const { return m_position; }
    
private:
    float m_yaw;
    float m_pitch;
    glm::vec3 m_forward;
    glm::vec3 m_position;
    float m_moving;
};
