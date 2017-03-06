#include "application.hpp"
#include "down_cast.hpp"
#include "editor_scene.hpp"
#include "path_editing_controller.hpp"

PathEditingController::PathEditingController(Scene* editor_scene) :
    SceneController(editor_scene, "PathEditingController"),
    m_selection(-1),
    m_attached_to_cursor(false)
{
    m_path.emplace_back(0.0f);
}

void PathEditingController::Update(Uint32 delta_time)
{
}

bool PathEditingController::ProcessEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_MOUSEMOTION:
        {
            if (m_attached_to_cursor)
            {
                const glm::vec3 projection_space_coords
                {
                    event.motion.x / (0.5f * (app()->window_width () - 1)) - 1.0f,
                    -event.motion.y / (0.5f * (app()->window_height() - 1)) + 1.0f,
                    0.0f
                };

                auto camera_controller = scene()->GetController<CameraController>("CameraController");
                const glm::vec3 view_space_coords = camera_controller->Unproject(projection_space_coords);
                const glm::vec3 world_space_coords = camera_controller->ConvertPointFromCameraSpaceToWorldSpace(view_space_coords);
                m_path[m_selection] = world_space_coords;
                m_path[m_selection].y = 0.0f;
                
                UpdatePath();
                return true;
            }
        }
    
       case SDL_MOUSEBUTTONDOWN:
       {
           if (m_attached_to_cursor)
           {
               m_attached_to_cursor = false;
               return true;
           }
       }

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_a)
            {
                m_path.emplace_back();
                m_selection = m_path.size() - 1;
                m_attached_to_cursor = true;
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                if (m_attached_to_cursor)
                {
                    m_path.erase(std::next(m_path.begin(), m_selection));
                    m_selection = -1;
                    m_attached_to_cursor = false;
                }
            }
            break;
    }

    return false;
}

void PathEditingController::UpdatePath()
{
    if (m_path.size() > 2)
    {
        LevelDescription level_description = down_cast<EditorScene*>(scene())->level_description();
        level_description.path = m_path;
        down_cast<EditorScene*>(scene())->SetLevelDescription(level_description);
    }
}
