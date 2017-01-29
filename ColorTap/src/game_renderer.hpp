#pragma once

#include "scene_renderer.hpp"
#include "shader_program.hpp"
#include "uniform_buffer.hpp"
#include "vertex_buffer.hpp"
#include "vertex_source.hpp"

class GameScene;
class GameController;

class GameRenderer :
	public SceneRenderer
{
public:
	GameRenderer(GameScene* game_scene);

	void Render() override;

private:
	void CalculateLeftBorderRect(SDL_Rect* rect);
	void CalculateRightBorderRect(SDL_Rect* rect);
	void CalculateTopBorderRect(SDL_Rect* rect);
	void CalculateBottomBorderRect(SDL_Rect* rect);
    void DrawRect(const SDL_Rect* rect, const Uint8 color[3]);
    void DrawRect(const SDL_Rect* rect, const Uint8 color[3], float alpha);

	GameController* m_controller;
    
    std::unique_ptr<ShaderProgram> m_shader_program;
    std::unique_ptr<UniformBuffer> m_uniform_buffer;
    std::unique_ptr<VertexBuffer> m_vertex_buffer;
    std::unique_ptr<VertexSource> m_vertex_source;
};
