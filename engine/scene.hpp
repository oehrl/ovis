#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL.h"

class SceneController;
class SceneRenderer;

class Scene
{
    friend class SceneController;
    friend class SceneRenderer;

public:
    Scene(const std::string& name, bool hide_previous = true);
    virtual ~Scene();

    inline const std::string& name() const { return m_name; }
    inline bool is_paused() const { return m_is_paused; }
    inline bool hides_previous() const { return m_hides_previous; }

    SceneController* GetController(const std::string& controller_name) const;
    SceneRenderer* GetRenderer(const std::string& renderer_name) const;

    void Update(Uint32 delta_time);
    void Render();

    void Resume();
    void Pause();

    bool ProcessEvent(const SDL_Event& event);

private:
    void AddController(SceneController* controller);
    void RemoveController(SceneController* controller);
    void AddRenderer(SceneRenderer* renderer);
    void RemoveRenderer(SceneRenderer* renderer);
    void SortRenderers();

    virtual void OnUpdate(Uint32 delta_time);
    virtual void OnResume();
    virtual void OnPause();
    virtual bool BeforeEventProcessing(const SDL_Event& /*event*/) { return false; }
    virtual bool AfterEventProcessing(const SDL_Event& /*event*/) { return false; }

    std::string m_name;
    std::unordered_map<std::string, SceneController*> m_controllers;
    std::unordered_map<std::string, SceneRenderer*> m_renderers;
    std::vector<SceneRenderer*> m_render_order;
    bool m_renderers_sorted;
    bool m_is_paused;
    bool m_hides_previous;
};
