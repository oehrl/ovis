#include "application.hpp"
#include "scene.hpp"

extern "C" int main(int /*argc*/, char* /*argv*/[])
{
	InitApp("APPLICATION_NAME");

    {
        Scene test_scene("TestScene");
        app()->PushScene(&test_scene);

        app()->Run();
    }

	QuitApp();
	return 0;
}
