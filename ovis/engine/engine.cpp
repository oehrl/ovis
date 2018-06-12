#include <SDL.h>
#if OVIS_EMSCRIPTEN
#include <emscripten.h>
#endif
#include <ovis/engine/engine.hpp>
#include <ovis/engine/window.hpp>

namespace ovis {

namespace {

void ProcessEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // TODO: only post events to the according window
    for (auto window : Window::all_windows()) {
      window->SendEvent(event);
    }
  }
}

void Update() {
  using namespace std::chrono;
  static auto time_point_of_last_update = high_resolution_clock::now();

  auto now = high_resolution_clock::now();
  auto delta_time =
      duration_cast<microseconds>(now - time_point_of_last_update);
  time_point_of_last_update += delta_time;

  ProcessEvents();

  for (auto window : Window::all_windows()) {
    window->Update(delta_time);
  }
  for (auto window : Window::all_windows()) {
    window->Render();
  }
}

}  // namespace

void Init() {
  SDL_Init(SDL_INIT_VIDEO);
}

void Run() {
#if OVIS_EMSCRIPTEN
  emscripten_set_main_loop(&Update, 0, true);
#else
  while (true) {
    Update();
  }
#endif
}

void Quit() {
  SDL_Quit();
}

}  // namespace ovis
