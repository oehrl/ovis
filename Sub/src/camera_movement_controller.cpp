#include <glm/gtx/string_cast.hpp>
#include <log.hpp>
#include <application.hpp>

#include "camera_movement_controller.hpp"
#include "editor_scene.hpp"

namespace
{
    const Uint32 TIME_THRESHOLD = 100;
    const float FRICTION = std::pow(0.05f, 1.0f/1000.0f);
    const float MOVEMENT_FACTOR = 100.0f;
    const float VELOCITY_TRHESHOLD = 0.01f / MOVEMENT_FACTOR;
}

CameraMovementController::CameraMovementController(EditorScene* editor_scene) :
    SceneController(editor_scene, "CameraMovementController"),
    m_velocity(0.0f, 0.0f),
    m_zoom_factor(1.0f)
{
}

void CameraMovementController::Update(Uint32 delta_time_in_milliseconds)
{
    if (glm::length(m_velocity) > 0.0f)
    {
        const float pow_friction_dt = std::pow(FRICTION, delta_time_in_milliseconds);
        const glm::vec2 distance = m_velocity * (pow_friction_dt - 1.0f) / std::log(FRICTION);
        m_velocity *= pow_friction_dt;
        if (glm::length(m_velocity) <= VELOCITY_TRHESHOLD)
        {
            m_velocity = glm::vec2(0.0f, 0.0f);
        }
        
        const glm::vec3 offset =
        {
            -distance.x * MOVEMENT_FACTOR * m_zoom_factor,
            0.0f,
            distance.y * MOVEMENT_FACTOR * m_zoom_factor
        };
        editor_scene()->camera_controller()->Move(offset);
    }
}

bool CameraMovementController::ProcessEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_FINGERDOWN:
            m_fingers.insert(event.tfinger.fingerId);
            if (m_fingers.size() == 2)
            {
                m_velocity = glm::vec2(0.0f, 0.0f);
                m_touch_events.clear();
            }
            return m_fingers.size() == 2;
    
        case SDL_FINGERUP:
            if (m_fingers.size() == 2)
            {
                while (m_touch_events.size() > 0 &&
                       event.tfinger.timestamp - m_touch_events.front().timestamp > TIME_THRESHOLD)
                {
                    m_touch_events.pop_front();
                }
                
                if (m_touch_events.size() > 0)
                {
                    std::unordered_map<SDL_FingerID, glm::vec4> touch_paths;
                    for (const auto& touch_event : m_touch_events)
                    {
                        auto it = touch_paths.find(touch_event.fingerId);
                        if (it == touch_paths.end())
                        {
                            touch_paths[touch_event.fingerId] =
                            {
                                touch_event.x,
                                touch_event.y,
                                touch_event.x,
                                touch_event.y,
                            };
                        }
                        else
                        {
                            it->second.z = touch_event.x;
                            it->second.w = touch_event.y;
                        }
                    }
                    
                    glm::vec2 distance(0.0f, 0.0f);
                    for (const auto& touch_path : touch_paths)
                    {
                        distance +=
                            glm::vec2(touch_path.second.z, touch_path.second.w) -
                            glm::vec2(touch_path.second.x, touch_path.second.y);
                    }
                    m_velocity = distance / static_cast<float>(event.tfinger.timestamp - m_touch_events.front().timestamp);
                }
                else
                {
                    m_velocity = glm::vec2(0.0f, 0.0f);
                }
            }
            m_fingers.erase(event.tfinger.fingerId);
            return m_fingers.size() == 1;
            
        case SDL_FINGERMOTION:
            if (m_fingers.size() == 2)
            {
                const glm::vec3 offset =
                {
                    -event.tfinger.dx * MOVEMENT_FACTOR * m_zoom_factor,
                    0.0f,
                    event.tfinger.dy * MOVEMENT_FACTOR * m_zoom_factor
                };
                editor_scene()->camera_controller()->Move(offset);
                m_touch_events.push_back(event.tfinger);
                //LogI(event.tfinger.dx, ",", event.tfinger.dy);
            }
            return true;
        
        case SDL_MULTIGESTURE:
            m_zoom_factor *= 1.0f - event.mgesture.dDist;
            editor_scene()->camera_controller()->SetWidth(
                app()->window_width() * m_zoom_factor
            );
            editor_scene()->camera_controller()->SetHeight(
                app()->window_height() * m_zoom_factor
            );
            return true;
            

        default:
            return false;
    }
}

EditorScene* CameraMovementController::editor_scene() const
{
    return static_cast<EditorScene*>(scene());
}
