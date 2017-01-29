#include "application.hpp"
#include "game_scene.hpp"
#include "file.hpp"

#include <iostream>
#include <glm/vec4.hpp>

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
	InitApp("ColorTap");

    {
        GameScene game_scene;
        app()->PushScene(&game_scene);

        app()->Run();
    }

	QuitApp();
	return 0;
}
