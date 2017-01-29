#pragma once

#include <string>
#include <vector>
#include <memory>

#include "SDL.h"

class Scene;
class GraphicsDevice;

class Application final
{
	friend class Scene;
	friend void InitApp(const std::string&);

public:
	~Application();

	SDL_Window* window() const { return m_window; }
	//SDL_Renderer* renderer() const { return nullptr; }
    GraphicsDevice* graphics_device() const { return m_graphics_device.get(); }
    
    inline std::string resource_path() const
    {
        return m_resource_path;
    }
    
    inline std::string GetFullResourcePath(const std::string& resource_name)
    {
        return resource_path() + resource_name;
    }
    
    void PushScene(Scene* scene);
    void ReplaceScene(Scene* scene);
    void PopScene();

    void Run();
    void Quit();

private:
	Application(const std::string& name);

	bool ProcessEvents();

	SDL_Window* m_window;
	//SDL_Renderer* m_renderer;
    std::unique_ptr<GraphicsDevice> m_graphics_device;
	bool m_quit;
    std::string m_resource_path;

    std::vector<Scene*> m_scene_stack;
};

void InitApp(const std::string& name);
void QuitApp();
Application* app();
inline GraphicsDevice* graphics_device()
{
    return app()->graphics_device();
}
