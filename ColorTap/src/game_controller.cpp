#include <vector>
#include <sstream>
#include <string>

#include "SDL_messagebox.h"

#include "application.hpp"
#include "game_controller.hpp"
#include "game_scene.hpp"

namespace
{
	const Uint32 INITIAL_ROUND_TIME = 8000;
	const Uint32 FAILED_DURATION = 250;
	const Uint32 LEVEL_DURATION = 10000;


    template <typename T>
    std::string to_string(T value)
    {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
}

GameController::GameController(GameScene* game_scene) :
	SceneController(game_scene, "GameController"),
	m_round_time(INITIAL_ROUND_TIME),
	m_current_spawn_delay(INITIAL_ROUND_TIME / 8),
	m_color_progress(0.0),
	m_level_time(0),
	m_points(0)
{
	CalculateSizes();
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			m_field[x][y].state = CellState::EMPTY;
			m_field[x][y].duration = 0;
			m_field[x][y].max_duration = 0;
			m_field[x][y].color_id = 0;
		}
	}
}

void GameController::Update(Uint32 delta_time)
{
	CalculateSizes();

	m_level_time += delta_time;

	if (m_level_time >= LEVEL_DURATION)
	{
		//std::cout << "newlevel" << m_level_time << ">=" << LEVEL_DURATION << std::endl;
		m_level_time -= LEVEL_DURATION;
		if (m_round_time > 1000)
		{
			m_round_time -= 500;
		}
	}

	m_color_progress += static_cast<double>(delta_time) / m_round_time;
	while (m_color_progress >= 1.0)
	{
		m_color_progress -= 1.0;
	}

	for (int y = 0; y < NUM_ROWS; ++y)
	{
		for (int x = 0; x < NUM_COLUMNS; ++x)
		{
			m_field[x][y].duration += delta_time;

			if (m_field[x][y].duration >= m_field[x][y].max_duration)
			{
				switch (m_field[x][y].state)
				{
					case CellState::ACTIVE:
						m_field[x][y].state = CellState::FAILED;
						m_field[x][y].duration = m_field[x][y].duration - m_field[x][y].max_duration;
						m_field[x][y].max_duration = FAILED_DURATION;
						break;

					case CellState::FAILED:
						m_field[x][y].state = CellState::EMPTY;
						m_field[x][y].duration = m_field[x][y].duration - m_field[x][y].max_duration;
						m_field[x][y].max_duration = 0;
						break;
				}
			}
		}
	}

	if (delta_time >= m_current_spawn_delay)
	{
		Uint32 initial_age = delta_time - m_current_spawn_delay;
		SpawnCell(initial_age);
		m_current_spawn_delay = INITIAL_ROUND_TIME / 16 + rand() % (INITIAL_ROUND_TIME/8 + 1) - initial_age;
	}
	else
	{
		m_current_spawn_delay -= delta_time;
	}
}

SDL_Rect GameController::GetCellRect(int x, int y) const
{
	SDL_Rect r;
	r.x = x * (m_tile_size + CELL_SPACING) + CELL_SPACING + m_field_offset_x;
	r.y = y * (m_tile_size + CELL_SPACING) + CELL_SPACING + m_field_offset_y;
	r.w = m_tile_size;
	r.h = m_tile_size;
	return r;
}

Uint32 GameController::GetCurrentColorId() const
{
	return static_cast<Uint32>(m_color_progress * NUM_CELL_COLORS);
}


bool GameController::ProcessEvent(const SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		SDL_Point point;
		point.x = event.button.x;
		point.y = event.button.y;

		for (int y = 0; y < NUM_ROWS; ++y)
		{
			for (int x = 0; x < NUM_COLUMNS; ++x)
			{
				SDL_Rect rect = GetCellRect(x, y);
				if (SDL_PointInRect(&point, &rect))
				{
					Cell& cell = m_field[x][y];
					if (cell.state == CellState::ACTIVE)
					{
						if (cell.color_id == GetCurrentColorId())
						{
							cell.state = CellState::EMPTY;
							cell.duration = 0;
							cell.max_duration = 0;
							m_points += 1;
						}
						else
						{
							m_field[x][y].state = CellState::FAILED;
							m_field[x][y].duration = 0;
							m_field[x][y].max_duration = FAILED_DURATION;

							std::string points_string = to_string(m_points);

							SDL_MessageBoxButtonData okay_button;
							okay_button.buttonid = 0;
							okay_button.text = "Okay";
							okay_button.flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;

							SDL_MessageBoxData msgbox_data;
							msgbox_data.message = points_string.c_str();
							msgbox_data.flags = SDL_MESSAGEBOX_INFORMATION;
							msgbox_data.colorScheme = nullptr;
							msgbox_data.buttons = &okay_button;
							msgbox_data.numbuttons = 1;
							msgbox_data.title = "Points";
							msgbox_data.window = app()->window();

							int button_id;
							SDL_ShowMessageBox(&msgbox_data, &button_id);
							app()->PopScene();
						}

						//m_color_progress = static_cast<double>(rand()) / RAND_MAX;
					}
				}
			}
		}
	}
	return false;
}

void GameController::SpawnCell(Uint32 initial_age)
{
	std::vector<Cell*> free_cells;
	free_cells.reserve(3*4);


	for (int y = 0; y < NUM_ROWS; ++y)
	{
		for (int x = 0; x < NUM_COLUMNS; ++x)
		{
			if (m_field[x][y].state == CellState::EMPTY)
			{
				free_cells.push_back(&m_field[x][y]);
			}
		}
	}

	if (free_cells.size() > 0)
	{
		int cell_index = rand() % free_cells.size();
		free_cells[cell_index]->state    = CellState::ACTIVE;
		free_cells[cell_index]->duration = initial_age;
		free_cells[cell_index]->max_duration = m_round_time * 2;
		free_cells[cell_index]->color_id = rand() % NUM_CELL_COLORS;
	}
}

void GameController::CalculateSizes()
{
	int window_width;
	int window_height;
	SDL_GetWindowSize(app()->window(), &window_width, &window_height);
	m_window_width = static_cast<Uint32>(window_width);
	m_window_height = static_cast<Uint32>(window_width);

	m_tile_size = (window_width - 2 * BORDER_WIDTH - (NUM_COLUMNS + 1) * CELL_SPACING) / NUM_COLUMNS;
	m_field_offset_x = BORDER_WIDTH;
	m_field_offset_y = (window_height - m_tile_size * NUM_ROWS) / 2;
	m_field_width = m_tile_size * NUM_COLUMNS + (NUM_COLUMNS + 1) * CELL_SPACING;
	m_field_height = m_tile_size * NUM_ROWS + (NUM_COLUMNS + 1) * CELL_SPACING;
}


