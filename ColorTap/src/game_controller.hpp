#pragma once

#include "scene_controller.hpp"

class GameScene;

enum class CellState : Uint32
{
	EMPTY,
	ACTIVE,
	FAILED
};

struct Cell
{
	CellState state;
	Uint32 duration;
	Uint32 max_duration;
	Uint32 color_id;
};

const Uint32 NUM_ROWS = 3;
const Uint32 NUM_COLUMNS = 3;
const Uint32 BORDER_WIDTH = 10;
const Uint32 NUM_CELL_COLORS = 4;
const Uint32 CELL_SPACING = 4;

class GameController :
	public SceneController
{
public:
	GameController(GameScene* game_scene);

	void Update(Uint32 delta_time) override;
	inline const Cell& GetCell(int x, int y) const { return m_field[x][y]; }
	SDL_Rect GetCellRect(int x, int y) const;
	Uint32 GetCurrentColorId() const;

	inline double color_progress() const { return m_color_progress; }
	inline Uint32 window_width() const { return m_window_width; }
	inline Uint32 window_height() const { return m_window_height; }
	inline Uint32 tile_size() const { return m_tile_size; }
	inline Uint32 field_width() const { return m_field_width; }
	inline Uint32 field_height() const { return m_field_height; }
	inline Uint32 field_offset_x() const { return m_field_offset_x; }
	inline Uint32 field_offset_y() const { return m_field_offset_y; }

	bool ProcessEvent(const SDL_Event& event) override;

private:
	void CalculateSizes();
	void SpawnCell(Uint32 initial_age);

	Uint32 m_points;
	double m_color_progress;
	Uint32 m_level_time;
	Uint32 m_round_time;
	Uint32 m_current_spawn_delay;
	Cell m_field[NUM_COLUMNS][NUM_ROWS];

	Uint32 m_window_width;
	Uint32 m_window_height;
	Uint32 m_tile_size;
	Uint32 m_field_width;
	Uint32 m_field_height;
	Uint32 m_field_offset_x;
	Uint32 m_field_offset_y;
};