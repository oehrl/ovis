//
// Created by Simon Oehrl on 31/05/16.
//

#include <algorithm>

#include "application.hpp"
#include "scene.hpp"
#include "scene_renderer.hpp"

SceneRenderer::SceneRenderer(Scene* scene, const std::string& name) :
    m_scene(scene),
    //m_renderer(app()->renderer()),
    m_name(name)
{
    scene->AddRenderer(this);
    // Log::Debug("Renderer '", name, "' added to scene '", scene->name(), "'");
}

SceneRenderer::~SceneRenderer()
{
    scene()->RemoveRenderer(this);
    // Log::Debug("Renderer '", name(), "' removed from scene '", scene()->name(), "'");
}

void SceneRenderer::RenderBefore(const std::string& renderer_name)
{
    m_render_before_list.insert(renderer_name);
    m_scene->m_renderers_sorted = false;
}

void SceneRenderer::RenderAfter(const std::string& renderer_name)
{
    m_render_after_list.insert(renderer_name);
    m_scene->m_renderers_sorted = false;
}
