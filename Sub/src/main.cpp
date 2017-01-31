#include "application.hpp"
#include "game_scene.hpp"
#include "level_renderer.hpp"
#include "camera_controller.hpp"
#include "perlin_noise.hpp"
#include "gui.hpp"
#include "gui_renderer.hpp"
#include "gui_controller.hpp"

#include <iostream>

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
	InitApp("APPLICATION_NAME");
    InitGui("$resources$/default_skin/skin.tb.txt");
    gui()->LoadFont("$resources$/vera.ttf", "Vera");
    gui()->SetDefaultFont("Vera", 4.0f);
    
    {
        GameScene game_scene;
        app()->PushScene(&game_scene);
        app()->Run();
    }

    ReleaseGui();
	QuitApp();
	return 0;
}
