#pragma once

#include <chrono>

#include <set>
#include <string>

#include <SDL2/SDL_events.h>

#include <ovis/core/class.hpp>

#if OVIS_ENABLE_BUILT_IN_PROFILING == 1
#include <ovis/core/profiling.hpp>
#endif

namespace ovis {
class Scene;

class SceneController {
  MAKE_NON_COPY_OR_MOVABLE(SceneController);
  friend class Scene;

 public:
  SceneController(Scene* scene, const std::string& name);
  virtual ~SceneController();

  inline Scene* scene() const { return m_scene; }
  inline std::string name() const { return m_name; }

  virtual void Update(std::chrono::microseconds delta_time);
  virtual bool ProcessEvent(const SDL_Event& event);

 private:
  Scene* m_scene;
  std::string m_name;

#if OVIS_ENABLE_BUILT_IN_PROFILING
  CPUTimeProfiler update_profiler_;
#endif

  inline void UpdateWrapper(std::chrono::microseconds delta_time) {
#if OVIS_ENABLE_BUILT_IN_PROFILING
    update_profiler_.BeginMeasurement();
#endif
    Update(delta_time);
#if OVIS_ENABLE_BUILT_IN_PROFILING
    update_profiler_.EndMeasurement();
#endif
  }
};

}  // namespace ovis
