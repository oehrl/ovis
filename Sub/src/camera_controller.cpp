#include "camera_controller.hpp"

#include <cmath>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <application.hpp>

CameraController::CameraController(Scene* scene) :
    SceneController(scene, "CameraController"),
    m_field_of_view(glm::pi<float>() * 0.5f),
    m_near_plane(0.1f),
    m_far_plane(1000.0f),
    m_width(static_cast<float>(app()->window_width())),
    m_height(static_cast<float>(app()->window_height())),
    m_projection_type(ProjectionType::PERSPECTIVE),
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_roll(0.0f),
    m_forward(0.0f, 0.0f, 1.0f),
    m_position(0.0f, 20.0f, 0.0f),
    m_moving(0.0f)
{
}

void CameraController::Update(Uint32 /*delta_time*/)
{
//    m_position += m_moving * m_forward * 10.0f * (delta_time / 1000.0f);

    const glm::mat4 m_inverse_rotation = glm::yawPitchRoll(m_yaw, m_pitch, m_roll);
    m_view_matrix = glm::transpose(m_inverse_rotation);
    m_view_matrix = glm::translate(m_view_matrix, -m_position);
    m_inverse_view_matrix = glm::inverse(m_view_matrix);
    
    if (m_projection_type == ProjectionType::PERSPECTIVE)
    {
        m_projection_matrix = glm::perspectiveFovLH(
            m_field_of_view,
            m_width,
            m_height,
            m_near_plane,
            m_far_plane
        );
    }
    else
    {
        const float half_width = 0.5f * m_width;
        const float half_height = 0.5f * m_height;
        m_projection_matrix = glm::orthoLH(
            -half_width,
             half_width,
            -half_height,
             half_height,
            m_near_plane,
            m_far_plane
        );
    }
    m_inverse_projection_matrix = glm::inverse(m_projection_matrix);
    
    m_view_projection_matrix = m_projection_matrix * m_view_matrix;
}

bool CameraController::ProcessEvent(const SDL_Event& /*event*/)
{
//    if (event.type == SDL_FINGERMOTION)
//    {
//        m_yaw -= event.tfinger.dx * 5;
//        m_pitch -= event.tfinger.dy * 5;
//        
//        m_forward.x = std::cos(m_pitch) * std::sin(-m_yaw);
//        m_forward.y = std::sin(m_pitch);
//        m_forward.z = std::cos(m_pitch) * std::cos(-m_yaw);
//        
//        return false;
//    }
//    else if (event.type == SDL_MOUSEBUTTONDOWN)
//    {
//        m_moving = 1.0f;
//        return false;
//    }
//    else if (event.type == SDL_MOUSEBUTTONUP)
//    {
//        m_moving = 0.0f;
//        return false;
//    }
//    else
//    {
//        return false;
//    }
}
