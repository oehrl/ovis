#include <ovis/engine/engine.hpp>
#include <ovis/engine/window.hpp>

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  ovis::Init();

  ovis::Window window("Resume", 1280, 720);
  window.resource_manager()->AddSearchPath(ovis::ExtractDirectory(argv[0]) +
                                           "/resources/");

  ovis::Run();

  ovis::Quit();
}