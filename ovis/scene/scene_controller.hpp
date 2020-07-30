#pragma once

#include <chrono>
#include <set>
#include <string>
#include <SDL2/SDL_events.h>
#include <ovis/core/class.hpp>

namespace ovis {
class Scene;

class SceneController {
  MAKE_NON_COPY_OR_MOVABLE(SceneController);

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
};

}  // namespace ovis
