#pragma once

#include <string>
#include <vector>
#include <memory>

#include "SDL.h"

class Scene;
class GraphicsDevice;
class Gui;

class Application final
{
	friend class Scene;
	friend void InitApp(const std::string&);

public:
	~Application();

	SDL_Window* window() const { return m_window; }
	//SDL_Renderer* renderer() const { return nullptr; }
    inline GraphicsDevice* graphics_device() const { return m_graphics_device.get(); }
    inline Gui* gui() const { return m_gui.get(); }
    
    inline std::string resource_path() const
    {
        return m_resource_path;
    }
    
    inline std::string GetFullResourcePath(const std::string& resource_name)
    {
        return resource_path() + resource_name;
    }
    
    inline Uint32 window_width()
    {
        return m_window_width;
    }
    
    inline Uint32 window_height()
    {
        return m_window_height;
    }
    
    inline double fps()
    {
        return m_frames_per_second;
    }
    
    void PushScene(Scene* scene);
    void ReplaceScene(Scene* scene);
    void PopScene();

    void Run();
    void Quit();

private:
	Application(const std::string& name);
    void Init();
    
	bool ProcessEvents();

    std::string m_name;
	SDL_Window* m_window;
    Uint32 m_window_width;
    Uint32 m_window_height;
	//SDL_Renderer* m_renderer;
    std::unique_ptr<GraphicsDevice> m_graphics_device;
    std::unique_ptr<Gui> m_gui;
	bool m_quit;
    std::string m_resource_path;
    double m_frames_per_second;

    std::vector<Scene*> m_scene_stack;
};

void InitApp(const std::string& name);
void QuitApp();
Application* app();
    
inline std::string GetFullResourcePath(const std::string& resource_name)
{
    return app()->resource_path() + resource_name;
}
