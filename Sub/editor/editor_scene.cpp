#include <algorithm>
#include <tb_geometry.h>
#include <tb_inline_select.h>
#include <glm/gtx/string_cast.hpp>
#include <range.hpp>

#include "editor_scene.hpp"
#include "gui.hpp"
#include "log.hpp"
#include "spline.hpp"
#include "file_dialogs.hpp"

EditorScene::EditorScene() :
    GuiScene("EditorScene", "$resources$/editor_layout.tb.txt"),
    m_camera_controller(this),
    m_level_renderer(this),
    m_camera_movement_controller(this)
{
    tb::TBWindow* window = gui_controller()->CreateWindow(
        "TabWindow",
        "$resources$/editor_tab_window.tb.txt"
    );
    auto parent_rect = gui_controller()->root_widget()->GetRect();
    window->SetSize(parent_rect.w * 0.2, parent_rect.h);
    window->SetSettings(tb::WINDOW_SETTINGS_NONE);
    
    m_camera_controller.SetProjectionType(ProjectionType::ORTHOGRAPHIC);
    m_camera_controller.Move({ 10.0f, 500.0f, 0.0f });
    m_camera_controller.SetPitch(glm::radians(90.0f));
}

bool EditorScene::BeforeEventProcessing(const SDL_Event& event)
{
    if (event.type == gui()->event_type())
    {
//        const tb::TBWidgetEvent* tb_event = reinterpret_cast<const tb::TBWidgetEvent*>(event.user.data1);
//        const tb::TBID target_id = tb_event->target->GetID();
//        
//        switch (tb_event->type)
//        {
//            case tb::EVENT_TYPE_CLICK:
//                if (target_id == tb::TBID("test-map"))
//                {
//                    StartGame();
//                    return true;
//                }
//                else if (target_id == TBIDC("add-node"))
//                {
//                    m_mode = EditorMode::ADD_NODE;
//                    m_path.push_back(m_path.back());
//                    return true;
//                }
//                break;
//        }
    }
    else if (event.type == SDL_KEYDOWN)
    {
//        if (event.key.keysym.sym == SDLK_r &&
//            event.key.keysym.mod & KMOD_GUI)
//        {
//            StartGame();
//            return true;
//        }
//        else if (event.key.keysym.sym == SDLK_ESCAPE &&
//                 m_mode != EditorMode::NONE)
//        {
//            if (m_mode == EditorMode::ADD_NODE)
//            {
//                m_path.pop_back();
//                UpdatePath();
//            }
//            m_mode = EditorMode::NONE;
//        }
    }
    return false;
}


bool EditorScene::AfterEventProcessing(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_s &&
                (event.key.keysym.mod & KMOD_GUI) != 0)
            {
                ShowSaveFileDialog([](const std::string& filename)
                {
                });
                return true;
            }
            break;
    
//        case SDL_MOUSEBUTTONDOWN:
//        {
//            if (m_mode == EditorMode::ADD_NODE)
//            {
//                m_path.push_back(m_path.back());
//            }
//        }
//    
//        case SDL_MOUSEMOTION:
//        {
//            if (m_mode == EditorMode::ADD_NODE)
//            {
//                const glm::vec3 projection_space_coords
//                {
//                     event.motion.x / (0.5f * (app()->window_width () - 1)) - 1.0f,
//                    -event.motion.y / (0.5f * (app()->window_height() - 1)) + 1.0f,
//                    0.0f
//                };
//                const glm::vec3 view_space_coords = camera_controller()->Unproject(projection_space_coords);
//                const glm::vec3 world_space_coords = camera_controller()->ConvertPointFromCameraSpaceToWorldSpace(view_space_coords);
//                m_path.back() = world_space_coords;
//                m_path.back().y = 0.0f;
//                
//                UpdatePath();
//                return true;
//            }
//            else
//            {
//                return false;
//            }
//        }
    }
    return false;
}

void EditorScene::OnUpdate(Uint32)
{
    gui_controller()->GetWidgetById(TBIDC("fps"))->SetText(
        std::to_string(static_cast<Uint32>(app()->fps())).c_str()
    );
}

void EditorScene::StartGame()
{
    app()->PushScene(&m_game_scene);
}

void EditorScene::UpdatePath()
{
    m_level_renderer.SetLevelDescription(m_level_description);
//    const int n = m_path.size();
//    std::vector<float> ts;
//    ts.reserve(n);
//    {
//        float current_distance = 0.0f;
//        for (auto i : IRange(n - 1))
//        {
//            ts.push_back(current_distance);
//            current_distance += glm::distance(m_path[i], m_path[i + 1]);
//        }
//        if (current_distance > ts.back())
//        {
//            ts.push_back(current_distance);
//        }
//    }
//    
//    {
//        std::vector<float> t = { -1, 0, 3 };
//        std::vector<float> v = { 0.5f, 0, 3 };
//        Spline<float, float> spline(t.begin(), t.end(), v.begin());
//        LogD(spline(-1));
//        LogD(spline(0));
//        LogD(spline(3));
//    }
//    
//    Spline<float, glm::vec3> spline(ts.begin(), ts.end(), m_path.begin());
//    
//    
//    const std::size_t VERTICES_PER_RING = 6;
//    const float RADIUS = 50.0f;
//    LevelDescription level_description;
//    level_description.path = m_path;
//    level_description.texture_tiling = 1;
//    level_description.texture_scaling = 1;
//    
//    std::size_t num_segments = fmax(2, ts.back() / 20);
//    level_description.path.clear();
//    level_description.path.reserve(num_segments);
//    for (auto i : IRange(num_segments))
//    {
//        const float t = ts.back() * i / (num_segments - 1);
//        level_description.path.push_back(spline(t));
//    }
//    level_description.vertex_offsets = std::vector<float>(level_description.path.size() * VERTICES_PER_RING, RADIUS);
//    
//    m_level_renderer.SetLevelDescription(level_description);
}
