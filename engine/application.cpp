#include <memory>
#include <iostream>

#include "application.hpp"
#include "graphics_device.hpp"
#include "scene.hpp"

Application::~Application()
{
    //SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Application::PushScene(Scene* scene)
{
    if (m_scene_stack.size() > 0 &&
        scene->hides_previous())
    {
        m_scene_stack.back()->Pause();
    }
    m_scene_stack.push_back(scene);
    m_scene_stack.back()->Resume();
}

void Application::ReplaceScene(Scene* scene)
{
    SDL_assert(
        m_scene_stack.size() > 0
    );
    m_scene_stack.back()->Pause();
    m_scene_stack.pop_back();
    m_scene_stack.push_back(scene);
    m_scene_stack.back()->Resume();
}

void Application::PopScene()
{
    SDL_assert(
        m_scene_stack.size() > 0
    );
    m_scene_stack.back()->Pause();
    bool previous_was_hidden = m_scene_stack.back()->hides_previous();
    m_scene_stack.pop_back();
    if (m_scene_stack.size() > 0 &&
        previous_was_hidden)
    {
        m_scene_stack.back()->Resume();
    }
}

void Application::Run()
{
    Uint32 last_time = SDL_GetTicks();
    Uint32 last_frame_time = last_time;
    Uint32 frame_count = 0;

    while (ProcessEvents())
    {
        Uint32 current_time = SDL_GetTicks();

        // while (current_time - m_time >= m_update_interval &&
        //        m_scene_stack.size() > 0)
        // {
        //     m_time += m_update_interval;

        //     while (m_input.size() > 0 &&
        //            m_input.top().time_stamp <= m_time)
        //     {
        //         for (const auto& scene : reverse(m_scene_stack))
        //         {
        //             if (scene->ProcessInput(m_input.top()) ||
        //                 scene->hides_previous())
        //             {
        //                 break;
        //             }
        //         }
        //         m_input.Pop();
        //     }

        //     if (m_scene_stack.size() > 0)
        //     {
        //         m_scene_stack.back()->Update(m_update_interval);
        //     }
        // }
        
        while (current_time - last_frame_time >= 1000)
        {
            last_frame_time += 1000;
            m_frames_per_second = 1.0 * frame_count;
            frame_count = 0;
        }

        if (m_scene_stack.size() == 0)
        {
            Quit();
        }
        else
        {
            // Update scene
            m_scene_stack.back()->Update(current_time - last_time);

            // Rendering
            m_graphics_device->Clear();
            if (m_scene_stack.size() > 0)
            {
                auto first_scene_to_render = --m_scene_stack.end();
                while (first_scene_to_render != m_scene_stack.begin() &&
                       !(*first_scene_to_render)->hides_previous())
                {
                    --first_scene_to_render;
                }

                for (auto it = first_scene_to_render; it != m_scene_stack.end(); ++it)
                {
                    (*it)->Render();
                }
            }
            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cout << error << std::endl;
            }
            
            SDL_GL_SwapWindow(m_window);
            ++frame_count;
        }

        last_time = current_time;
    }
}

void Application::Quit()
{
    m_quit = true;
}

Application::Application(const std::string& name) :
    m_quit(false),
    m_resource_path(SDL_GetBasePath())
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    
    m_window =  SDL_CreateWindow(
        name.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        1136,
        SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
    );
    SDL_assert(
        m_window != nullptr
    );
    
    int window_width;
    int window_height;
    SDL_GetWindowSize(m_window, &window_width, &window_height);
    m_window_width = window_width;
    m_window_height = window_height;
    
    m_graphics_device = std::make_unique<GraphicsDevice>(m_window);
}

bool Application::ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
             m_quit = true;
        }

        std::vector<Scene*> scene_stack_copy = m_scene_stack;
        for (auto it = scene_stack_copy.rbegin(), e = scene_stack_copy.rend(); it != e; ++it)
        {
            if ((*it)->ProcessEvent(event))
            {
                break;
            }
        }
    }
    return !m_quit;
}

namespace
{
    std::unique_ptr<Application> application;
}

void InitApp(const std::string& name)
{
    SDL_assert(
        !application
    );
    application.reset(new Application(name));
}

void QuitApp()
{
    application.reset();
}

Application* app()
{
    return application.get();
}
