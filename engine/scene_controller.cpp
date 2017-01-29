//
// Created by Simon Oehrl on 31/05/16.
//

#include <algorithm>

#include "scene.hpp"
#include "scene_controller.hpp"

SceneController::SceneController(Scene* scene, const std::string& name) :
    m_scene(scene),
    m_name(name)
{
    scene->AddController(this);
    // TODO: Debug string
    //Log::Debug("Scene controller '", name, "' added to scene '", scene->name(), "'");
}

SceneController::~SceneController()
{
    scene()->RemoveController(this);
    // TODO: Debug String
    // Log::Debug("Scene controller '", name(), "' removed from scene '", scene()->name(), "'");
}

bool SceneController::ProcessEvent(const SDL_Event& /*event*/)
{
    return false;
}
