#include <tb_core.h>
#include <tb_widgets.h>

#include <SDL.h>

#include "application.hpp"

#include "gui_controller.hpp"

class GuiController::RootWidget :
    public tb::TBWidget
{
public:
    RootWidget(GuiController* gui_controller)
    {
    }
};
asd
GuiController::GuiController(Scene* scene) :
    SceneController(scene, "GuiController")
{
    int window_width;
    int window_height;
    SDL_GetWindowSize();
}
