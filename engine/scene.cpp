//
// Created by Simon Oehrl on 05/05/16.
//

#include <algorithm>
#include <map>

#include "scene.hpp"
#include "scene_controller.hpp"
#include "scene_renderer.hpp"

Scene::Scene(const std::string& name, bool hide_previous) :
    m_name(name),
    m_renderers_sorted(false),
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
    if (!m_renderers_sorted)
    {
        SortRenderers();
    }
    
    for (auto renderer : m_render_order)
    {
        renderer->Render();
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
    if (BeforeEventProcessing(event))
    {
        return true;
    }
    
    for (const auto& scene_controller : m_controllers)
    {
        if (scene_controller.second->ProcessEvent(event))
        {
            return true;
        }
    }
    
    return AfterEventProcessing(event);
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
    m_renderers_sorted = false;
}

void Scene::RemoveRenderer(SceneRenderer* renderer)
{
    SDL_assert(
        m_renderers.find(renderer->name()) != m_renderers.end() &&
        m_renderers.find(renderer->name())->second == renderer
    );
    m_renderers.erase(m_renderers.find(renderer->name()));
    m_renderers_sorted = false;
}

void Scene::SortRenderers()
{
    // First depends on second beeing already rendered
    std::multimap<std::string, std::string> dependencies;
    std::set<std::string> renderers_left;
    
    for (auto name_renderer_pair : m_renderers)
    {
        renderers_left.insert(name_renderer_pair.first);
        
        for (auto render_before : name_renderer_pair.second->m_render_before_list)
        {
            dependencies.insert(std::make_pair(
                render_before,
                name_renderer_pair.first
            ));
        }
        
        for (auto render_after : name_renderer_pair.second->m_render_after_list)
        {
            dependencies.insert(std::make_pair(
                name_renderer_pair.first,
                render_after
            ));
        }
    }
    
    m_render_order.resize(0);
    m_render_order.reserve(m_renderers.size());
    while (renderers_left.size() > 0)
    {
        auto next = std::find_if(
            renderers_left.begin(),
            renderers_left.end(),
            [&dependencies](const std::string& value)
        {
            return dependencies.count(value) == 0;
        });
        
        SDL_assert(next != renderers_left.end());
        
        m_render_order.push_back(m_renderers[*next]);
        for (auto i = dependencies.begin(), e = dependencies.end(); i != e;)
        {
            if (i->second == *next)
            {
                i = dependencies.erase(i);
            }
            else
            {
                ++i;
            }
        }
        renderers_left.erase(next);
    }
    
    m_renderers_sorted = true;
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
