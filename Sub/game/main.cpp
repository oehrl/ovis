#include "application.hpp"
#include "game_scene.hpp"
#include "gui.hpp"

#include <iostream>

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
	InitApp("Sub", "Ovis");
    
    gui()->LoadSkin("$resources$/default_skin/skin.tb.txt");
    gui()->LoadFont("$resources$/vera.ttf", "Vera");
    gui()->LoadFont("$resources$/fontawesome-webfont.ttf", "FontAwesome");
    gui()->SetDefaultFont("Vera", 10.0f);
    
    {
        GameScene start_scene;
        app()->PushScene(&start_scene);
        app()->Run();
    }

	QuitApp();
	return 0;
}
