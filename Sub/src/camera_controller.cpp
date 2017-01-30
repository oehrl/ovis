#include "camera_controller.hpp"

#include <cmath>

CameraController::CameraController(Scene* scene) :
    SceneController(scene, "CameraController"),
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_forward(0.0f, 0.0f, 1.0f),
    m_position(0.0f, 20.0f, 0.0f),
    m_moving(0.0f)
{
}

void CameraController::Update(Uint32 delta_time)
{
    m_position += m_moving * m_forward * 10.0f * (delta_time / 1000.0f);
}

bool CameraController::ProcessEvent(const SDL_Event& event)
{
    if (event.type == SDL_FINGERMOTION)
    {
        m_yaw -= event.tfinger.dx * 5;
        m_pitch -= event.tfinger.dy * 5;
        
        m_forward.x = std::cos(m_pitch) * std::sin(-m_yaw);
        m_forward.y = std::sin(m_pitch);
        m_forward.z = std::cos(m_pitch) * std::cos(-m_yaw);
        
        return true;
    }
    else if (event.type == SDL_FINGERDOWN)
    {
        m_moving = 1.0f;
        return true;
    }
    else if (event.type == SDL_FINGERUP)
    {
        m_moving = 0.0f;
        return true;
    }
    else
    {
        return false;
    }
}
