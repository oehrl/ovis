#include "application.hpp"
#include "game_scene.hpp"
#include "editor_scene.hpp"
#include "gui.hpp"

#include <iostream>

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
	InitApp("APPLICATION_NAME");
    InitGui("$resources$/default_skin/skin.tb.txt");
    gui()->LoadFont("$resources$/vera.ttf", "Vera");
    gui()->SetDefaultFont("Vera", 4.0f);
    
    {
        EditorScene start_scene;
        app()->PushScene(&start_scene);
        app()->Run();
    }

    ReleaseGui();
	QuitApp();
	return 0;
}
