#include "game_scene.hpp"
#include "gui.hpp"
#include "application.hpp"

GameScene::GameScene() :
    GuiScene("GameScene", "$resources$/layout.txt"),
    m_level_renderer(this),
    m_camera_conroller(this)
{
}
    
void GameScene::OnUpdate(Uint32 /*delta_time*/)
{
    std::string text = std::to_string((int)app()->fps());
    gui_controller()->GetWidgetById("fps")->SetText(text.c_str());
}

bool GameScene::BeforeEventProcessing(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            app()->PopScene();
            return true;
        }
    }
    return false;
}
