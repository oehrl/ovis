#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL_events.h>
#include <ovis/core/class.hpp>

namespace ovis {

class SubSystem {
  MAKE_NON_COPY_OR_MOVABLE(SubSystem);

 public:
  SubSystem(const std::string& name);
  virtual ~SubSystem();

  inline static const std::vector<SubSystem*> all_sub_systems() {
    return all_subsystems_;
  }

  static SubSystem* GetByName(const std::string& name);

  inline std::string name() const { return name_; }

  virtual void BeforeEventProcessing() {}
  virtual void AfterEventProcessing() {}

  virtual void BeforeUpdate() {}
  virtual void AfterUpdate() {}

  virtual void BeforeRendering() {}
  virtual void AfterRendering() {}

 private:
  static std::vector<SubSystem*> all_subsystems_;

  std::string name_;
};

}  // namespace ovis
