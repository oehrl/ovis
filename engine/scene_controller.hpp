//
// Created by Simon Oehrl on 31/05/16.
//

#pragma once

#include <set>
#include <string>

#include "SDL.h"

class Scene;

class SceneController
{
public:
    SceneController(Scene* scene, const std::string& name);
    virtual ~SceneController();

    inline Scene* scene() const { return m_scene; }
    inline std::string name() const { return m_name; }

    virtual void Update(Uint32 delta_time) = 0;
    virtual bool ProcessEvent(const SDL_Event& event);

private:
    Scene* m_scene;
    std::string m_name;
};
