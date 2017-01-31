#include "gui_scene.hpp"

GuiScene::GuiScene(
    const std::string& scene_name,
    const std::string& gui_layout_filename
) :
    Scene(scene_name),
    m_gui_controller(this, gui_layout_filename),
    m_gui_renderer(this)
{
}
