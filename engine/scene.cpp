//
// Created by Simon Oehrl on 05/05/16.
//

#include <algorithm>
#include "scene.hpp"
#include "scene_controller.hpp"
#include "scene_renderer.hpp"

Scene::Scene(const std::string& name, bool hide_previous) :
    m_name(name),
    m_is_paused(true),
    m_hides_previous(hide_previous)
{
}

Scene::~Scene()
{
}

SceneController* Scene::GetController(const std::string& controller_name) const
{
    auto controller = m_controllers.find(controller_name);
    if (controller == m_controllers.end())
    {
        return nullptr;
    }
    else
    {
        return controller->second;
    }
}

SceneRenderer* Scene::GetRenderer(const std::string& renderer_name) const
{
    auto renderer = m_renderers.find(renderer_name);
    if (renderer == m_renderers.end())
    {
        return nullptr;
    }
    else
    {
        return renderer->second;
    }
}

void Scene::Update(Uint32 delta_time)
{
    OnUpdate(delta_time);
    for (auto controller : m_controllers)
    {
        controller.second->Update(delta_time);
    }
}

void Scene::Render()
{
    for (auto renderer : m_renderers)
    {
        renderer.second->Render();
    }
}

void Scene::Pause()
{
    if (!m_is_paused)
    {
        m_is_paused = true;
        OnPause();
    }
}

bool Scene::ProcessEvent(const SDL_Event& event)
{
    for (const auto& scene_controller : m_controllers)
    {
        if (scene_controller.second->ProcessEvent(event))
        {
            return true;
        }
    }
    return false;
}

void Scene::Resume()
{
    if (m_is_paused)
    {
        m_is_paused = false;
        OnResume();
    }
}

void Scene::AddController(SceneController* controller)
{
    SDL_assert(
        m_controllers.find(controller->name()) == m_controllers.end()
    );
    m_controllers.insert(std::make_pair(
        controller->name(),
        controller
    ));
}

void Scene::RemoveController(SceneController* controller)
{
    SDL_assert(
        m_controllers.find(controller->name()) != m_controllers.end() &&
        m_controllers.find(controller->name())->second == controller
    );
    m_controllers.erase(m_controllers.find(controller->name()));
}

void Scene::AddRenderer(SceneRenderer* renderer)
{
    SDL_assert(
        m_renderers.find(renderer->name()) == m_renderers.end()
    );
    m_renderers.insert(std::make_pair(
        renderer->name(),
        renderer
    ));
}

void Scene::RemoveRenderer(SceneRenderer* renderer)
{
    SDL_assert(
        m_renderers.find(renderer->name()) != m_renderers.end() &&
        m_renderers.find(renderer->name())->second == renderer
    );
    m_renderers.erase(m_renderers.find(renderer->name()));
}

void Scene::OnUpdate(Uint32 /*delta_time*/)
{
}

void Scene::OnResume()
{
}

void Scene::OnPause()
{
}
