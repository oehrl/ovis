#include "ovis/scene/sub_system.hpp"
#include <cassert>

namespace ovis {

SubSystem::SubSystem(const std::string& name) : name_(name) {
  all_subsystems_.push_back(this);
}

SubSystem::~SubSystem() {
  auto it = std::find(all_subsystems_.begin(), all_subsystems_.end(), this);
  assert(it != all_subsystems_.end());
  std::swap(*it, all_subsystems_.back());
  all_subsystems_.pop_back();
}

SubSystem* SubSystem::GetByName(const std::string& name) {
  for (const auto& sub_system : all_subsystems_) {
    if (sub_system->name() == name) {
      return sub_system;
    }
  }
  return nullptr;
}

}  // namespace ovis