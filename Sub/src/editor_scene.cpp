#include <tb_geometry.h>

#include "editor_scene.hpp"
#include "gui.hpp"
#include "log.hpp"

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

bool EditorScene::BeforeEventProcessing(const SDL_Event& event)
{
    if (event.type == gui()->event_type())
    {
        const tb::TBWidgetEvent* tb_event = reinterpret_cast<const tb::TBWidgetEvent*>(event.user.data1);
        switch (tb_event->type)
        {
            case tb::EVENT_TYPE_CLICK:
                LogV("EVENT_TYPE_CLICK");
                break;

            default:
                break;
        }
    }
    return false;
}
