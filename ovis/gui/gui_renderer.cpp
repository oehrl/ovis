#if 0
#include <tb_widgets.h>

#include "application.hpp"
#include "gui.hpp"
#include "gui_controller.hpp"
#include "gui_renderer.hpp"
#include "scene.hpp"

GuiRenderer::GuiRenderer(Scene* scene) :
    SceneRenderer(scene, "GuiRenderer")
{
    SDL_assert(gui() != nullptr);
}

void GuiRenderer::Render()
{
    SDL_assert(gui() != nullptr);
    
    GuiController* controller = static_cast<GuiController*>(
        scene()->GetController("GuiController")
    );
    
    if (controller != nullptr)
    {
        tb::g_renderer->BeginPaint(
            app()->window_width(),
            app()->window_height()
        );
        controller->root_widget()->InvokePaint(tb::TBWidget::PaintProps());
        tb::g_renderer->EndPaint();
        controller->root_widget()->Invalidate();
    }
}
#endif
