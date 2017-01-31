//
// Created by Simon Oehrl on 31/05/16.
//

#pragma once

#include <string>
#include <set>

#include "SDL.h"

class Scene;

class SceneRenderer
{
    friend class Scene;

public:
    SceneRenderer(Scene* scene, const std::string& name);
    virtual ~SceneRenderer();

    inline Scene* scene() const { return m_scene; }
    inline SDL_Renderer* renderer() const { return m_renderer; }
    inline std::string name() const { return m_name; }

    virtual void Render() = 0;
    
protected:
    void RenderBefore(const std::string& renderer_name);
    void RenderAfter(const std::string& renderer_name);

private:
    Scene* m_scene;
    SDL_Renderer* m_renderer;
    std::string m_name;
    std::set<std::string> m_render_before_list;
    std::set<std::string> m_render_after_list;
};
