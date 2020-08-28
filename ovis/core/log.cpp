#include <iostream>

#include <ovis/core/log.hpp>

namespace ovis {

size_t Log::AddListener(LogListener listener) {
  auto insert_position =
      std::find_if(log_listeners_.begin(), log_listeners_.end(), [](const auto& listener) { return !listener; });
  auto position = log_listeners_.insert(insert_position, listener);
  return position - log_listeners_.begin();
}

void Log::RemoveListener(size_t id) {
  SDL_assert(id < log_listeners_.size());
  log_listeners_[id] = nullptr;
}

void ConsoleLogger(LogLevel, const std::string& text) {
  std::cout << text << std::endl;
}

std::vector<LogListener> Log::log_listeners_;

}  // namespace ovis