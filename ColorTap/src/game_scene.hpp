#pragma once

#include "game_controller.hpp"
#include "game_renderer.hpp"
#include "scene.hpp"

class GameScene :
	public Scene
{
public:
	GameScene();

	inline GameController* controller() { return &m_game_controller; } 

private:
	GameController m_game_controller;
	GameRenderer m_game_renderer;
};