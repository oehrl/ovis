#include "game_scene.hpp"

GameScene::GameScene() :
    GuiScene("GameScene", "$resources$/layout.txt"),
    m_level_renderer(this),
    m_camera_conroller(this)
{
}
