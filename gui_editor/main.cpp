#include <application.hpp>
#include <gui.hpp>

#include "editor_scene.hpp"

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
    InitApp("Sub", "Ovis");

    gui()->LoadSkin("$resources$/default_skin/skin.tb.txt");
    gui()->LoadFont("$resources$/vera.ttf", "Vera");
    gui()->LoadFont("$resources$/fontawesome-webfont.ttf", "FontAwesome");
    gui()->SetDefaultFont("Vera", 10.0f);
    
    EditorScene editor_scene;
    app()->PushScene(&editor_scene);
    app()->Run();

    QuitApp();
    return 0;
}
