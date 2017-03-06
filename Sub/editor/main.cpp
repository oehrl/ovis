#include "application.hpp"
#include "game_scene.hpp"
#include "editor_scene.hpp"
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
#ifdef __IPHONE_OS__
        GameScene start_scene;
#else
        EditorScene start_scene;
#endif
        app()->PushScene(&start_scene);
        app()->Run();
    }

	QuitApp();
	return 0;
}
