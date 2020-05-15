#include <SDL.h>
#if OVIS_EMSCRIPTEN
#include <emscripten.h>
#endif
#include <ovis/engine/engine.hpp>
#include <ovis/engine/window.hpp>

namespace ovis {

namespace {

bool ProcessEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      return false;
    }

    // TODO: only post events to the according window
    for (auto window : Window::all_windows()) {
      window->SendEvent(event);
    }
  }

  return true;
}

bool Update() {
  using namespace std::chrono;
  static auto time_point_of_last_update = high_resolution_clock::now();

  auto now = high_resolution_clock::now();
  auto delta_time =
      duration_cast<microseconds>(now - time_point_of_last_update);
  time_point_of_last_update += delta_time;

  if (!ProcessEvents()) {
    return false;
  }

  for (auto window : Window::all_windows()) {
    window->Update(delta_time);
  }
  for (auto window : Window::all_windows()) {
    window->Render();
  }

  return true;
}

#ifdef OVIS_EMSCRIPTEN
void EmscriptenUpdate() {
  Update();
}
#endif  // OVIS_EMSCRIPTEN

}  // namespace

void Init() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

void Run() {
#if OVIS_EMSCRIPTEN
  emscripten_set_main_loop(&EmscriptenUpdate, 0, true);
#else
  while (Update())
    ;
#endif
}

void Quit() {
  SDL_Quit();
}

}  // namespace ovis
