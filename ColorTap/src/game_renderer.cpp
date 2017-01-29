#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

#include "application.hpp"
#include "game_renderer.hpp"
#include "game_scene.hpp"
#include "graphics_device.hpp"

namespace
{
	Uint8 CellColors[NUM_CELL_COLORS][3] =
	{
		{ 255,   0, 255 },
		{   0, 255,   0 },
		{   0,   0, 255 },
		{ 255, 255,   0 },
	};
	Uint8 FailedColor[3] = { 255, 0, 0 };
	Uint8 EmptyColor[3] = { 50, 50, 50 };
}

GameRenderer::GameRenderer(GameScene* game_scene) :
	SceneRenderer(game_scene, "GameRenderer"),
	m_controller(game_scene->controller())
{
    m_shader_program = LoadShaderProgram(app()->GetFullResourcePath("sample"));

    const float vertices[] =
    {
         0.0f,  0.0f,
         1.0f,  0.0f,
         0.0f,  1.0f,
         1.0f,  1.0f,
    };
    VertexBufferDescription vb_desc;
    vb_desc.size_in_bytes = sizeof(vertices);
    vb_desc.vertex_size_in_bytes = sizeof(float) * 2;
    m_vertex_buffer = std::make_unique<VertexBuffer>(app()->graphics_device(), vb_desc, vertices);
    
    VertexSourceDescription vs_desc;
    vs_desc.shader_program = m_shader_program.get();
    vs_desc.vertex_buffers = { m_vertex_buffer.get() };
    vs_desc.vertex_attributes = { { "a_Position", VertexAttributeType::FLOAT32_VECTOR2, 0, 0 } };
    m_vertex_source = std::make_unique<VertexSource>(app()->graphics_device(), vs_desc);
}

void GameRenderer::Render()
{
//    m_shader_program->SetUniform("Intensity", 1.0f);
//
//    DrawItem item;
//    item.shader_program = m_shader_program.get();
//    item.vertex_source = m_vertex_source.get();
//    item.primitive_topology = PrimitiveTopology::TRIANGLE_STRIP;
//    item.base_vertex = 0;
//    item.vertex_count = 4;
//    app()->graphics_device()->Draw(item);

//    SDL_Rect r = { 50, 50, 50, 50 };
//    Uint8 color[] = { 255, 0, 255 };
//    DrawRect(&r, color);

    int window_width;
    int window_height;
    SDL_GetWindowSize(app()->window(), &window_width, &window_height);

    //SDL_SetRenderDrawBlendMode(renderer(), SDL_BLENDMODE_BLEND);

    double remaining_time = std::fmod(m_controller->color_progress(), 1.0 / NUM_CELL_COLORS);
    double intensity = 1.0 - std::pow(remaining_time * NUM_CELL_COLORS, 3.0);

    Uint8 current_color[3];
    memcpy(current_color, CellColors[m_controller->GetCurrentColorId()], 3);
    //SDL_SetRenderDrawColor(renderer(), current_color[0], current_color[1], current_color[2], intensity * 255);

    SDL_Rect border_rect;
    CalculateLeftBorderRect(&border_rect);
    DrawRect(&border_rect, current_color, intensity);

    CalculateRightBorderRect(&border_rect);
    DrawRect(&border_rect, current_color, intensity);

    CalculateTopBorderRect(&border_rect);
    DrawRect(&border_rect, current_color, intensity);

    CalculateBottomBorderRect(&border_rect);
    DrawRect(&border_rect, current_color, intensity);


    for (int y = 0; y < NUM_ROWS; ++y)
    {
        for (int x = 0; x < NUM_COLUMNS; ++x)
        {
            const Cell& cell = m_controller->GetCell(x, y);
            SDL_Rect cell_rect = m_controller->GetCellRect(x, y);

            Uint8 color[3];

            switch (cell.state)
            {
                case CellState::EMPTY:
                    memcpy(color, EmptyColor, 3);
                    DrawRect(&cell_rect, color);
                    break;

                case CellState::FAILED:
                    memcpy(color, FailedColor, 3);
                    DrawRect(&cell_rect, color);
                    break;

                case CellState::ACTIVE:
                    memcpy(color, CellColors[cell.color_id], 3);
                    DrawRect(&cell_rect, color);

                    double progress = static_cast<double>(cell.duration) / cell.max_duration;
                    int adjusted_width = (1.0 - progress) * cell_rect.w;
                    int adjusted_height = (1.0 - progress) * cell_rect.h;

                    SDL_Rect adjusted_rect;
                    adjusted_rect.x = cell_rect.x + (cell_rect.w - adjusted_width) / 2;
                    adjusted_rect.y = cell_rect.y + (cell_rect.h - adjusted_height) / 2;
                    adjusted_rect.w = adjusted_width;
                    adjusted_rect.h = adjusted_height;

                    memcpy(color, EmptyColor, 3);
                    DrawRect(&adjusted_rect, color);
                    break;
            }
        }
    }
}

void GameRenderer::DrawRect(const SDL_Rect *rect, const Uint8 color[3])
{
    DrawRect(rect, color, 1.0f);
}

void GameRenderer::DrawRect(const SDL_Rect *rect, const Uint8 color[3], float alpha)
{
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(app()->window(), &window_width, &window_height);
    glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f);
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(), glm::vec3(rect->w, rect->h, 1.0f));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(), glm::vec3(rect->x, rect->y, 0.0f));
    glm::mat4 transform_matrix = projection_matrix * translation_matrix * scaling_matrix;
    
    const float COLOR_NORMALIZE_FACTOR = 1.0f / 255.0f;
    
    m_shader_program->SetUniform("TransformMatrix", transform_matrix);
    m_shader_program->SetUniform("Color", COLOR_NORMALIZE_FACTOR * glm::vec4(color[0], color[1], color[2], alpha * 255.0f));

    DrawItem item;
    item.shader_program = m_shader_program.get();
    item.vertex_source = m_vertex_source.get();
    item.primitive_topology = PrimitiveTopology::TRIANGLE_STRIP;
    item.base_vertex = 0;
    item.vertex_count = 4;
    app()->graphics_device()->Draw(item);
}

void GameRenderer::CalculateLeftBorderRect(SDL_Rect* rect)
{
	rect->x = 0;
	rect->y = m_controller->field_offset_y();
	rect->w = BORDER_WIDTH;
	rect->h = m_controller->field_height();
}

void GameRenderer::CalculateTopBorderRect(SDL_Rect* rect)
{
	rect->x = 0;
	rect->y = m_controller->field_offset_y() - BORDER_WIDTH;
	rect->w = m_controller->window_width();
	rect->h = BORDER_WIDTH;
}

void GameRenderer::CalculateRightBorderRect(SDL_Rect* rect)
{
	rect->x = m_controller->window_width() - BORDER_WIDTH;
	rect->y = m_controller->field_offset_y();
	rect->w = BORDER_WIDTH;
	rect->h = m_controller->field_height();
}

void GameRenderer::CalculateBottomBorderRect(SDL_Rect* rect)
{
	rect->x = 0;
	rect->y = m_controller->field_offset_y() + m_controller->field_height();
	rect->w = m_controller->window_width();
	rect->h = BORDER_WIDTH;
}

