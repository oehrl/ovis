#include "application.hpp"
#include "scene.hpp"
#include "level_renderer.hpp"
#include "camera_controller.hpp"
#include "perlin_noise.hpp"

#include <iostream>

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
	InitApp("APPLICATION_NAME");

    {
        Scene test_scene("TestScene");
        LevelRenderer level_renderer(&test_scene);
        CameraController camera_controller(&test_scene);
        
        app()->PushScene(&test_scene);

        app()->Run();
    }

	QuitApp();
	return 0;
}
