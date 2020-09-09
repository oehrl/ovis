#include <SDL2/SDL.h>
#if OVIS_EMSCRIPTEN
#include <emscripten.h>
#endif
#include <ovis/core/profiling.hpp>
#include <ovis/engine/engine.hpp>
#include <ovis/engine/lua.hpp>
#include <ovis/engine/module.hpp>
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
  auto delta_time = duration_cast<microseconds>(now - time_point_of_last_update);
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

  ProfilingLog::default_log()->AdvanceFrame();

  return true;
}

#ifdef OVIS_EMSCRIPTEN
void EmscriptenUpdate() {
  Update();
}
#endif  // OVIS_EMSCRIPTEN

static std::map<std::type_index, std::unique_ptr<Module>> loaded_modules;

}  // namespace

namespace detail {

Module* AddModule(std::type_index type, std::unique_ptr<Module> module) {
  const auto loaded_module = loaded_modules.insert(std::make_pair(type, std::move(module)));
  if (!loaded_module.second) {
    ovis::LogE("Module '{}' already loaded!", type.name());
  }
  return loaded_module.second ? loaded_module.first->second.get() : nullptr;
}

Module* GetModule(std::type_index type) {
  const auto module = loaded_modules.find(type);
  if (module == loaded_modules.end()) {
    ovis::LogW("Module '{}' not loaded!", type.name());
    return nullptr;
  } else {
    return module->second.get();
  }
}

}  // namespace detail

void Init() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  Lua::SetupEnvironment();
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
