#include "game_scene.hpp"

GameScene::GameScene() :
	Scene("GameScene"),
	m_game_controller(this),
	m_game_renderer(this)
{

}