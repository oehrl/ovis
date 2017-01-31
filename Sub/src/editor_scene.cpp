#include <tb_geometry.h>

#include "editor_scene.hpp"

EditorScene::EditorScene() :
    GuiScene("EditorScene", "$resources$/editor_layout.tb.txt")
{
    tb::TBWindow* window = gui_controller()->CreateWindow(
        "TabWindow",
        "$resources$/editor_tab_window.tb.txt"
    );
    auto parent_rect = gui_controller()->root_widget()->GetRect();
    window->SetSize(parent_rect.w * 0.5, parent_rect.h);
    window->SetSettings(tb::WINDOW_SETTINGS_NONE);
}
